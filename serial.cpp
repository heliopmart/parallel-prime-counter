#include "serial.h"
using namespace std;

int _calculateAndPrintTime(auto start, auto end, string task_name) {
    auto diff = end - start;
    auto ms = chrono::duration_cast<chrono::milliseconds>(diff).count();
    cout << "Tempo de execucao (" << task_name << "): " << ms << " ms" << endl;
    return ms;
}

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    
    // 2 é primo por definição.
    if (n == 2) {
        return true;
    }
    
    // numeros pares não são primos por definição
    if (n % 2 == 0) {
        return false;
    }

   
    /*
        ! Usaremos a seguinte propriedade matemática:
        
        Se um número n não é primo, ele pode ser fatorado em a * b.
        Se a e b fossem ambos maiores que a raiz quadrada de n, então a * b seria maior que n. Isso é impossível.
        Portanto, pelo menos um dos divisores (a ou b) deve ser menor ou igual à raiz quadrada de n.

    */

    // i += 2 pula os divisores pares
    // i <= raiz(n);
    for (int i = 3; i <= sqrt(n); i += 2) { 
        if (n % i == 0) {
            return false; 
        }
    }

    return true;
}

int countPrimeNumbers(Matrix* mat) {
    int count = 0; // Um contador local, simples e seguro.

    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            // Condição correta para números PRIMOS
            if (isPrime(mat->data[i][j])) {
                count++;
            }
        }
    }
    
    return count; // Retorna o resultado final.
}

int singleThread(Matrix* mat) {
    cout << "Orquestrando a criacao da matriz..." << endl;

    // variavel que contabiliza os numeros pares
    int even_count = 0;
    
    // verifica os numeros primos
    auto start = chrono::steady_clock::now();
    even_count = countPrimeNumbers(mat);
    int time = _calculateAndPrintTime(start, chrono::steady_clock::now(), "Contagem de numeros primos");

    cout << "Quantidade de numeros primos: " << even_count << endl;

    return time; // Finaliza o programa com sucesso
}