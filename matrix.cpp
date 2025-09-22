#include "matrix.h" // Inclui as declarações que prometemos implementar

// Usar o namespace aqui no .cpp é seguro e aceitável
using namespace std;

void setMatrixDimensions(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
}

void setMatrixData(Matrix* mat, unsigned int seed) {
    if (mat->rows <= 0 || mat->cols <= 0) {
        cerr << "Erro: As dimensões da matriz devem ser positivas." << endl;
        return;
    }
    mt19937 engine(seed);
    uniform_int_distribution<int> dist(0, 9999999);

    mat->data.resize(mat->rows);
    for (int i = 0; i < mat->rows; ++i) {
        mat->data[i].resize(mat->cols);
    }
    
    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            mat->data[i][j] = dist(engine);
        }
    }
}

void _printMatrix(const Matrix* mat) {
    cout << fixed << setprecision(2);
    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            cout << mat->data[i][j] << "\t";
        }
        cout << endl;
    }
}

Matrix createInitializedMatrix(int rows, int cols, unsigned int seed) {
    Matrix mat;
    
    setMatrixDimensions(&mat, rows, cols);
    setMatrixData(&mat, seed);
    
    return mat;
}