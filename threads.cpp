#include "threads.h"

using namespace std;

// Uma tarefa simples: processar um bloco de linhas.
struct Task {
    int start_row;
    int end_row;
};

// Estrutura para todos os dados compartilhados entre as threads
struct SharedData {
    queue<Task> task_queue;          // A fila de tarefas
    CRITICAL_SECTION queue_cs;       // O "cadeado" para proteger a fila
    HANDLE hTaskSemaphore;           // O semáforo que conta as tarefas disponíveis
    
    const Matrix* mat;
    atomic<bool> stop_all{false};    // Flag global para encerrar tudo

    atomic<int> tasks_completed{0};  // Contador de tarefas concluídas
    atomic<long long> total_result{0}; 
};

// Função para imprimir o tempo, continua útil.
int _calculateAndPrintTime(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end, const string& task_name) {
    auto diff = end - start;
    auto ms = chrono::duration_cast<chrono::milliseconds>(diff).count();
    cout << "Tempo de execucao (" << task_name << "): " << ms << " ms" << endl;
    return ms;
}

bool isPrime(int n);

/**
 * @brief Esta é a nossa "função trabalhadora". Cada thread executará este código.
 * * @param mat Ponteiro para a matriz (const, pois não a modificamos).
 * @param start_row A linha inicial que esta thread deve processar.
 * @param end_row A linha final (exclusiva) que esta thread deve processar.
 * @param result Onde a thread armazenará sua contagem parcial de números pares.
 */
DWORD WINAPI dynamic_worker(void* param) {
    SharedData* shared_data = (SharedData*)param;
    while (true) {
        // Verifica se há tarefas disponíveis, dormindo eficientemente se não houver.
        WaitForSingleObject(shared_data->hTaskSemaphore, INFINITE);

        // Se acordou, checa se é um sinal para parar ou se realmente há trabalho.
        if (shared_data->stop_all.load(memory_order_acquire)) {
            break;
        }

        // Pega uma tarefa da fila de forma segura.
        Task current_task;
        
        EnterCriticalSection(&shared_data->queue_cs); // Tranca a fila
        if (!shared_data->task_queue.empty()) {
            // pega a tarefa do front da fila FIFO
            current_task = shared_data->task_queue.front();
            // remove a tarefa da fila
            shared_data->task_queue.pop();
        }
        LeaveCriticalSection(&shared_data->queue_cs); // Destranca a fila

        // Processa a tarefa que pegou.
        long long count = 0;
        for (int i = current_task.start_row; i < current_task.end_row; ++i) {
            for (int j = 0; j < shared_data->mat->cols; ++j) {
                if (isPrime(shared_data->mat->data[i][j])) {
                    count++;
                }
            }
        }

        shared_data->total_result.fetch_add(count, memory_order_release);
        shared_data->tasks_completed.fetch_add(1, memory_order_release);
    }
    return 0;
}

int startThreads(Matrix* mat, int n_threads, int chunk_size = 0) {
    // Criação da Matrix
    // int rows = 10000;
    // int cols = 10000;
    // Matrix mat = createInitializedMatrix(rows, cols, 1672516721);

    int rows = mat->rows;

    // Instancia as Threads
    vector<HANDLE> thread_handles(n_threads);
    SharedData shared_data; // Apenas uma instância dos dados compartilhados

    // Inicializar os objetos de sincronização e os dados compartilhados
    shared_data.mat = mat;
    InitializeCriticalSection(&shared_data.queue_cs); // sessão crítica para a fila

    // Cria um semáforo que começa em 0 e pode ir até o número de linhas (um limite alto que nunca será alcançado) 
    shared_data.hTaskSemaphore = CreateSemaphore(NULL, 0, rows, NULL);

    // Cria as threads. Elas começarão e irão direto dormir no semáforo.
    for (int i = 0; i < n_threads; i++) {
        thread_handles[i] = CreateThread(nullptr, 0, dynamic_worker, (void*)&shared_data, 0, nullptr);
    }
    cout << n_threads << " threads trabalhadoras criadas e prontas para receber tarefas." << endl;

    //  Criando as Tarefas
    cout << "\n[MAIN] Gerando fila de tarefas..." << endl;
    auto start_task_gen = chrono::steady_clock::now();

    
    int total_tasks = 0;
    // Basicamente cria um vetor de todos os numeros que precisam ser processados [{start_row, end_row}, {start_row, end_row}, ...]
    for (int i = 0; i < rows; i += chunk_size) {
        // Vetor Task
        Task new_task;
        new_task.start_row = i;
        new_task.end_row = min(i + chunk_size, rows);
        
        // Tranca a fila, adiciona a tarefa e destranca
        EnterCriticalSection(&shared_data.queue_cs);
        shared_data.task_queue.push(new_task);
        LeaveCriticalSection(&shared_data.queue_cs);

        // Sinaliza ao semáforo que uma nova tarefa está disponível.
        // Isso permitirá que uma das threads aguardando no semáforo acorde e processe a nova tarefa. porque add 1 ao contador do semáforo
        ReleaseSemaphore(shared_data.hTaskSemaphore, 1, NULL);
        total_tasks++;
    }
    _calculateAndPrintTime(start_task_gen, chrono::steady_clock::now(), "Geracao de Tarefas");
    cout << "[MAIN] " << total_tasks << " tarefas foram criadas e enfileiradas." << endl;
    
    auto start_task_controller = chrono::steady_clock::now();

    // Aqui podemos fazer outras coisas
    while (shared_data.tasks_completed.load(memory_order_acquire) < total_tasks) {
        cout << "\r[MAIN] Progresso: " << shared_data.tasks_completed.load() << "/" << total_tasks << " tarefas concluidas.";
    }
    cout << "\n[MAIN] Todas as tarefas foram concluídas!" << endl;
        
    // Encerrar as threads de forma limpa.
    cout << "[MAIN] Encerrando threads trabalhadoras..." << endl;
    shared_data.stop_all.store(true, memory_order_release);
    // Acorda todas as threads que possam estar dormindo no semáforo para que elas leiam a flag 'stop_all'
    ReleaseSemaphore(shared_data.hTaskSemaphore, n_threads, NULL); 
    
    WaitForMultipleObjects(n_threads, thread_handles.data(), TRUE, INFINITE);

    int time_taken = _calculateAndPrintTime(start_task_controller, chrono::steady_clock::now(), "Thread Control _ ");

    // Limpar os recursos de sincronização
    DeleteCriticalSection(&shared_data.queue_cs);
    CloseHandle(shared_data.hTaskSemaphore);
    for (auto h : thread_handles) {
        CloseHandle(h);
    }

    cout << "[MAIN] Programa encerrado com sucesso." << endl;

    cout << "Resultado final (numeros primos): " << shared_data.total_result.load() << endl;
    cout << "Resultado final (tarefas): " << shared_data.tasks_completed.load() << " tarefas processadas." << endl;

    // ... (liberação da matriz) ...
    return time_taken;
}
