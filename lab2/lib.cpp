#include "lib.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <omp.h>  

using namespace std;

#define NUM_THREADS 8

void inputMatrix(vector< vector<int> >& matrix, int rows, int cols) {
    int min = 0, max = 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);

    matrix.resize(rows, vector<int>(cols));

    // Заполнение матрицы случайными числами
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
}

void printMatrix(const vector< vector<int> >& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            cout << setw(4) << elem << " ";
        }
        cout << endl;
    }
}

void printVector(const std::vector<int>& vec) {
    for (const int& element : vec) {
        cout << element << " ";
    }
    cout << endl;
}

vector<int> findRowSum(const vector< vector<int> >& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<int> rowSum(rows, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rowSum[i] += matrix[i][j];
        }
    }
    return rowSum;
}

vector<int> findColSum(const vector< vector<int> >& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<int> colSum(cols, 0);
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            colSum[j] += matrix[i][j];
        }
    }
    return colSum;
}

vector<int> findRowSumParal(const vector< vector<int> >& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<int> rowSum(rows, 0);

    // Установка количества потоков
    omp_set_num_threads(NUM_THREADS);

    // Параллелизация внешнего цикла с статическим распределением итераций
#pragma omp parallel for schedule(static)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rowSum[i] += matrix[i][j];
        }
    }

    return rowSum;
}


vector<int> findColSumParal(const vector< vector<int> >& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<int> colSum(cols, 0);

    // Параллелизация внешнего цикла с динамическим распределением итераций
#pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            colSum[j] += matrix[i][j];
        }
    }

    return colSum;
}