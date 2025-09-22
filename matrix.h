#ifndef MATRIX_H // "Include Guards" para evitar inclusão múltipla
#define MATRIX_H

#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <iostream>
#include <chrono>

// Definição da estrutura da Matriz
struct Matrix {
    std::vector<std::vector<int>> data;
    int rows = 0;
    int cols = 0;
};

// --- PROTÓTIPOS DAS FUNÇÕES ---
// Declara que essas funções existem, mas o código delas está em outro lugar (no .cpp)

void setMatrixDimensions(Matrix* mat, int rows, int cols);
void setMatrixData(Matrix* mat, unsigned int seed);
void _printMatrix(const Matrix* mat);

// Esta será nossa nova função "construtora"
Matrix createInitializedMatrix(int rows, int cols, unsigned int seed);

#endif // MATRIX_H