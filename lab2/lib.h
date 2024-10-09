
#ifndef LIB_H
#define LIB_H

#include <vector>

// ������� ��� ���������� ������� ���������� �������
void inputMatrix(std::vector< std::vector<int> >& matrix, int rows, int cols);

// ������� ��� ������ �������
void printMatrix(const std::vector< std::vector<int> >& matrix);

// ������� ��� ������ ������� ����� �����
void printVector(const std::vector<int>& vec);

// ���������������� ���������� ����� �� �������
std::vector<int> findRowSum(const std::vector< std::vector<int> >& matrix);

// ���������������� ���������� ����� �� ��������
std::vector<int> findColSum(const std::vector< std::vector<int> >& matrix);

// ������������ ���������� ����� �� �������
std::vector<int> findRowSumParal(const std::vector< std::vector<int> >& matrix);

// ������������ ���������� ����� �� ��������
std::vector<int> findColSumParal(const std::vector< std::vector<int> >& matrix);

#endif // LIB_H
