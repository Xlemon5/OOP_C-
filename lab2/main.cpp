#include <iostream>
#include <iomanip>
#include <vector>
#include <omp.h>
#include "lib.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    int rows, cols;

    cout << "������� ����������� �������" << endl;
    cin >> rows >> cols;

    vector< vector<int> > matrix;

    inputMatrix(matrix, rows, cols);
    //printMatrix(matrix);

    cout << "------------------------" << endl;
    // ���������������� ����������
    double time1 = omp_get_wtime();
    findRowSum(matrix);
    findColSum(matrix);

    double res1 = (omp_get_wtime() - time1);
    //cout << "����� ���������� ��� �����������������: " << fixed << setprecision(10) << (omp_get_wtime() - time1) << " ms" << endl;

    /*
    cout << "����������: " << endl;
    cout << "�� �������: " << endl;
    printVector(findRowSum(matrix));
    cout << "�� ��������: " << endl;
    printVector(findColSum(matrix));
    */

    cout << "------------------------" << endl;
    // ������������ ����������
    double time2 = omp_get_wtime();
    findRowSumParal(matrix);
    findColSumParal(matrix);
    
    cout << "����� ���������� ��� �����������������: " << fixed << setprecision(10) << res1 << " ms" << endl;
    cout << "����� ���������� � ������������������:  " << fixed << setprecision(10) << (omp_get_wtime() - time2) << " ms" << endl;

   
    

    return 0;
}
