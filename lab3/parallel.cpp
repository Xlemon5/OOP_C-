#include <iostream>
#include <vector>
#include <omp.h>  
#define NUM_THREADS 8

struct Point {
    double x, y, z;
};


void transform_points_parallel(std::vector<Point>& points, double offset_x, double offset_y, double offset_z) {
    int num_points = points.size();
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for
    for (int i = 0; i < num_points; ++i) {
        points[i].x += offset_x;
        points[i].y += offset_y;
        points[i].z += offset_z;
    }
#pragma omp barrier
}

int main() {
    const int num_points = 1000000; 
    std::vector<Point> points(num_points); 

    for (int i = 0; i < num_points; ++i) {
        points[i] = { double(i), double(i), double(i) }; // Каждая точка инициализируется значениями i
    }


    double offset_x = 1.0, offset_y = 2.0, offset_z = 3.0;


    double start_time = omp_get_wtime(); // Засекаем начальное время

    transform_points_parallel(points, offset_x, offset_y, offset_z);

    double end_time = omp_get_wtime(); // Засекаем конечное время


    double elapsed_time = end_time - start_time; // Время выполнения в секундах

    std::cout << "Parallel Time: " << elapsed_time << " seconds\n";

    return 0;
}
