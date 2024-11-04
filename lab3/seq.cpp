#include <iostream>
#include <vector>
#include <ctime>  // Для измерения времени
#include <omp.h>
#include "windows.h" 

struct Point {
    double x, y, z;
};

void transform_points(std::vector<Point>& points, double offset_x, double offset_y, double offset_z) {
    for (auto& point : points) {
        point.x += offset_x;
        point.y += offset_y;
        point.z += offset_z;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int num_points = 1000000; // Количество точек
    std::vector<Point> points(num_points); // Создание вектора точек


    for (int i = 0; i < num_points; ++i) {
        points[i] = { double(i), double(i), double(i) }; // Каждая точка инициализируется значениями i
    }

    double offset_x = 1.0, offset_y = 2.0, offset_z = 3.0;

    double start_time = omp_get_wtime(); // Засекаем начальное время

    transform_points(points, offset_x, offset_y, offset_z);

    double end_time = omp_get_wtime(); // Засекаем конечное время

    double elapsed_time = double(end_time - start_time);// Время выполнения в секундах

    std::cout << "Sequential Time: " << elapsed_time << " seconds\n";

    return 0;
}
