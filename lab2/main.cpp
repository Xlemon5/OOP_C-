#include <iostream>
#include <iomanip>
#include <vector>
#include <omp.h>
#include "lib.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    int rows, cols;

    cout << "Введите размерность матрицы" << endl;
    cin >> rows >> cols;

    vector< vector<int> > matrix;

    inputMatrix(matrix, rows, cols);
    //printMatrix(matrix);

    cout << "------------------------" << endl;
    // Последовательное вычисление
    double time1 = omp_get_wtime();
    findRowSum(matrix);
    findColSum(matrix);

    double res1 = (omp_get_wtime() - time1);
    //cout << "Время выполнения без распараллеливания: " << fixed << setprecision(10) << (omp_get_wtime() - time1) << " ms" << endl;

    /*
    cout << "Результаты: " << endl;
    cout << "По строкам: " << endl;
    printVector(findRowSum(matrix));
    cout << "По столбцам: " << endl;
    printVector(findColSum(matrix));
    */

    cout << "------------------------" << endl;
    // Параллельное вычисление
    double time2 = omp_get_wtime();
    findRowSumParal(matrix);
    findColSumParal(matrix);
    
    cout << "Время выполнения без распараллеливания: " << fixed << setprecision(10) << res1 << " ms" << endl;
    cout << "Время выполнения с распараллеливанием:  " << fixed << setprecision(10) << (omp_get_wtime() - time2) << " ms" << endl;

   
    

    return 0;
}
