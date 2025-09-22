#include "threads.h"
#include "serial.h"

using namespace std;

struct MatrixConfig {
    int rows;
    int cols;
    int seed = 0;
};

struct ThreadConfig {
    int n_threads;
    int chunk_size = 100;
};

struct AppConfig {
    int timeThread;
    int timeSingleThread;
};

void configureMatrix(MatrixConfig& matConfig) {
    cout << "Defina o numero de linhas da matriz: ";
    cin >> matConfig.rows;
    cout << "Defina o numero de colunas da matriz: ";
    cin >> matConfig.cols;
    cout << "Defina a semente para inicializacao da matriz: ";
    cin >> matConfig.seed;
    return;
}

void configureThreads(ThreadConfig& threadConfig) {
    cout << "Defina o numero de threads: ";
    cin >> threadConfig.n_threads;
    cout << "Defina o tamanho do chunk (submatrizes) (100 para automatico): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer do cin
    
    string chunk_input;
    getline(cin, chunk_input);
    if (chunk_input.empty()) {
        // ? 100 é o valor padrão
        threadConfig.chunk_size = 100;
    } else {
        try {
            threadConfig.chunk_size = stoi(chunk_input); // str to int
            if (threadConfig.chunk_size <= 0) {
                cout << "Tamanho de chunk invalido. Usando padrao de 100." << endl;
                threadConfig.chunk_size = 100;
            }
        } catch (const invalid_argument&) {
            cout << "Entrada invalida. Usando valor padrao de 100." << endl;
            threadConfig.chunk_size = 100;
        }
    }

    return;
}

void processMatrix(MatrixConfig matConfig, Matrix& mat) {
    if (matConfig.rows <= 0 || matConfig.cols <= 0 || matConfig.seed < 0) {
        cerr << "Configuração inválida da matriz." << endl;
        return;
    }
    
    mat = createInitializedMatrix(matConfig.rows, matConfig.cols, matConfig.seed);
    return;
}

void compareResults(AppConfig& appConfig, ThreadConfig threadConfig) {
    cout << "Comparando resultados..." << endl;
    cout << "Tempo Single Thread: " << appConfig.timeSingleThread << " ms" << endl;
    cout << "Tempo Multi Thread: " << appConfig.timeThread << " ms" << endl;
    
    // Comparação bruta de eficiencia
    double speedup = static_cast<double>(appConfig.timeSingleThread) / appConfig.timeThread;
    // Calculo de eficiencia por numero de threads
    double efficiency = (speedup / threadConfig.n_threads) * 100.0;
    cout << "Speedup: " << speedup << "x" << endl;
    cout << "Eficiência: " << efficiency << "%" << endl;
    return;
}

void startSingleThreadProcessing(AppConfig& appConfig, Matrix& mat) {
    if (mat.rows == 0 || mat.cols == 0 || mat.data.empty()) {
        cerr << "Matriz não inicializada. Por favor, processe a matriz primeiro." << endl;
        return;
    }
    cout << "Iniciando processamento single thread..." << endl;
    int time_single_thread = singleThread(&mat);
    appConfig.timeSingleThread = time_single_thread;
    return;
}

void startMultiThreadProcessing(AppConfig& appConfig, ThreadConfig& threadConfig, Matrix& mat) {
    if (mat.rows == 0 || mat.cols == 0 || mat.data.empty()) {
        cerr << "Matriz não inicializada. Por favor, processe a matriz primeiro." << endl;
        return;
    }

    cout << "Iniciando processamento com " << threadConfig.n_threads << " threads e chunk size de " << threadConfig.chunk_size << endl;
    
    int time_threads = startThreads(&mat, threadConfig.n_threads, threadConfig.chunk_size);
    appConfig.timeThread = time_threads;
    return;
}

int main(){

    Matrix mat;
    MatrixConfig matConfig;
    ThreadConfig threadConfig;
    AppConfig appConfig;

    while(true){
        cout << string(50, '=') << endl;
        cout << "1 - Definir Matriz" << endl;
        cout << "2 - Processar Matrix" << endl;
        cout << "3 - Configure Threads" << endl;
        cout << "4 - Iniciar Processamento em single Thread" << endl;
        cout << "5 - Iniciar Processamento em multi Thread" << endl;
        cout << "6 - Comparar Resultados" << endl;
        cout << "7 - Sair" << endl;
        cout << "Escolha uma opcao: ";
        
        int choice;
        cin >> choice;

        cout << string(50, '_') << endl;

        switch (choice) {
            case 1:
                configureMatrix(matConfig);
                break;
            case 2:
                processMatrix(matConfig, mat);
                break;
            case 3:
                configureThreads(threadConfig);
                break;
            case 4:
                startSingleThreadProcessing(appConfig, mat);
                break;
            case 5:
                startMultiThreadProcessing(appConfig, threadConfig, mat);
                break;
            case 6:
                compareResults(appConfig, threadConfig);
                break;
            case 7:
                exit(0);
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    }
}