#include <iostream>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <vector>

using namespace std;

static const int M = 4; // число строк
static const int N = 5; // число столбцов

// Ядро для суммирования по строкам.
__global__ void sumRowsKernel(const int* d_matrix, int* d_rowSums, int M, int N) {
    extern __shared__ int sdata[];
    int row = blockIdx.x;
    int tid = threadIdx.x;
    int idx = row * N + tid;

    // Инициализируем локальный массив
    sdata[tid] = (tid < N) ? d_matrix[idx] : 0;
    __syncthreads();

    // Редукция по блоку
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // Записываем результат в выходной массив
    if (tid == 0) {
        d_rowSums[row] = sdata[0];
    }
}

// Ядро для суммирования по столбцам.
__global__ void sumColsKernel(const int* d_matrix, int* d_colSums, int M, int N) {
    extern __shared__ int sdata[];
    int col = blockIdx.x;
    int tid = threadIdx.x;
    int idx = tid * N + col;

    // Инициализируем локальный массив
    sdata[tid] = (tid < M) ? d_matrix[idx] : 0;
    __syncthreads();

    // Редукция по блоку
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // Записываем результат в выходной массив
    if (tid == 0) {
        d_colSums[col] = sdata[0];
    }
}

int main() {
    // Создаем матрицу на хосте
    vector<int> h_matrix(M * N);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            h_matrix[i * N + j] = i + j + 1; // произвольные данные
        }
    }

    // Выделяем память на девайсе
    int* d_matrix;
    int* d_rowSums;
    int* d_colSums;

    cudaMalloc((void**)&d_matrix, M * N * sizeof(int));
    cudaMalloc((void**)&d_rowSums, M * sizeof(int));
    cudaMalloc((void**)&d_colSums, N * sizeof(int));

    // Создаем события для измерения времени
    cudaEvent_t startTotal, stopTotal;
    cudaEvent_t startRows, stopRows;
    cudaEvent_t startCols, stopCols;

    cudaEventCreate(&startTotal);
    cudaEventCreate(&stopTotal);
    cudaEventCreate(&startRows);
    cudaEventCreate(&stopRows);
    cudaEventCreate(&startCols);
    cudaEventCreate(&stopCols);

    // Запускаем общее измерение времени
    cudaEventRecord(startTotal, 0);

    // Копируем данные с хоста на устройство
    cudaMemcpy(d_matrix, h_matrix.data(), M * N * sizeof(int), cudaMemcpyHostToDevice);

    // Суммирование по строкам:
    // Каждый блок - одна строка, количество потоков - ближайшая степень двойки >= N
    int threadsPerBlockRows = 1;
    while (threadsPerBlockRows < N) threadsPerBlockRows <<= 1;
    cudaEventRecord(startRows, 0);
    sumRowsKernel<<<M, threadsPerBlockRows, threadsPerBlockRows * sizeof(int)>>>(d_matrix, d_rowSums, M, N);
    cudaEventRecord(stopRows, 0);

    // Суммирование по столбцам:
    // Каждый блок - один столбец, количество потоков - ближайшая степень двойки >= M
    int threadsPerBlockCols = 1;
    while (threadsPerBlockCols < M) threadsPerBlockCols <<= 1;
    cudaEventRecord(startCols, 0);
    sumColsKernel<<<N, threadsPerBlockCols, threadsPerBlockCols * sizeof(int)>>>(d_matrix, d_colSums, M, N);
    cudaEventRecord(stopCols, 0);

    // Синхронизируемся, чтобы убедиться, что все ядра завершились
    cudaDeviceSynchronize();

    // Копируем результаты назад на хост
    vector<int> h_rowSums(M), h_colSums(N);
    cudaMemcpy(h_rowSums.data(), d_rowSums, M * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_colSums.data(), d_colSums, N * sizeof(int), cudaMemcpyDeviceToHost);

    // Завершаем общее измерение времени
    cudaEventRecord(stopTotal, 0);
    cudaEventSynchronize(stopTotal);

    // Вычисляем время в миллисекундах
    float totalTime = 0.0f, rowsTime = 0.0f, colsTime = 0.0f;
    cudaEventElapsedTime(&totalTime, startTotal, stopTotal);
    cudaEventElapsedTime(&rowsTime, startRows, stopRows);
    cudaEventElapsedTime(&colsTime, startCols, stopCols);

    // Вывод результатов
    cout << "Исходная матрица:" << endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << h_matrix[i * N + j] << "\t";
        }
        cout << endl;
    }

    cout << "\nСуммы по строкам:" << endl;
    for (int i = 0; i < M; ++i) {
        cout << "Строка " << i << ": " << h_rowSums[i] << endl;
    }

    cout << "\nСуммы по столбцам:" << endl;
    for (int j = 0; j < N; ++j) {
        cout << "Столбец " << j << ": " << h_colSums[j] << endl;
    }

    // Вывод времени выполнения
    cout << "\nВремя выполнения:" << endl;
    cout << "Общее время: " << totalTime << " мс" << endl;
    cout << "Суммирование по строкам: " << rowsTime << " мс" << endl;
    cout << "Суммирование по столбцам: " << colsTime << " мс" << endl;

    // Освобождение ресурсов
    cudaFree(d_matrix);
    cudaFree(d_rowSums);
    cudaFree(d_colSums);

    cudaEventDestroy(startTotal);
    cudaEventDestroy(stopTotal);
    cudaEventDestroy(startRows);
    cudaEventDestroy(stopRows);
    cudaEventDestroy(startCols);
    cudaEventDestroy(stopCols);

    return 0;
}
