
#ifndef LIB_H
#define LIB_H

#include <vector>

// Функция для заполнения матрицы случайными числами
void inputMatrix(std::vector< std::vector<int> >& matrix, int rows, int cols);

// Функция для печати матрицы
void printMatrix(const std::vector< std::vector<int> >& matrix);

// Функция для вывода вектора целых чисел
void printVector(const std::vector<int>& vec);

// Последовательное вычисление суммы по строкам
std::vector<int> findRowSum(const std::vector< std::vector<int> >& matrix);

// Последовательное вычисление суммы по столбцам
std::vector<int> findColSum(const std::vector< std::vector<int> >& matrix);

// Параллельное вычисление суммы по строкам
std::vector<int> findRowSumParal(const std::vector< std::vector<int> >& matrix);

// Параллельное вычисление суммы по столбцам
std::vector<int> findColSumParal(const std::vector< std::vector<int> >& matrix);

#endif // LIB_H
