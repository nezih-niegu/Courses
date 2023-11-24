#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <unistd.h>
#define MAX_SIZE 100 // This will be used only as a fallback if dimensions are not provided

// Function to allocate memory for a matrix dynamically
double** allocate_matrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

// Function to free the allocated memory for a matrix
void free_matrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// CUDA kernel for matrix multiplication
__global__ void matrix_multiplication_kernel(double* matrixA, double* matrixB, double* result, int rowsA, int colsA, int colsB) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rowsA && col < colsB) {
        double value = 0;
        for (int k = 0; k < colsA; k++) {
            value += matrixA[row * colsA + k] * matrixB[k * colsB + col];
	    //usleep(1000);
        }
        result[row * colsB + col] = value;
    }
}

// Function to perform matrix multiplication on the GPU
void matrix_multiplication_cuda(double** matrixA, double** matrixB, int rowsA, int colsA, int colsB, double** result) {
    // Allocate device memory
    double *d_matrixA, *d_matrixB, *d_result;
    cudaError_t cudaStatus;

    cudaStatus = cudaMalloc((void**)&d_matrixA, rowsA * colsA * sizeof(double));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed for matrixA: %s\n", cudaGetErrorString(cudaStatus));
        return;
    }
    cudaMalloc((void**)&d_matrixB, colsA * colsB * sizeof(double));
    cudaMalloc((void**)&d_result, rowsA * colsB * sizeof(double));

    // Copy input matrices to device
    cudaMemcpy(d_matrixA, matrixA[0], rowsA * colsA * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_matrixB, matrixB[0], colsA * colsB * sizeof(double), cudaMemcpyHostToDevice);

    // Define block and grid sizes
    dim3 blockSize(16, 16);
    dim3 gridSize((colsB + blockSize.x - 1) / blockSize.x, (rowsA + blockSize.y - 1) / blockSize.y);

    // Launch the CUDA kernel
    matrix_multiplication_kernel<<<gridSize, blockSize>>>(d_matrixA, d_matrixB, d_result, rowsA, colsA, colsB);
    //usleep(1);
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "matrix_multiplication_kernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        return;
    }
    // Synchronize CUDA Device
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize failed: %s\n", cudaGetErrorString(cudaStatus));
        return;
    }

    // Copy the result back to the host
    cudaStatus = cudaMemcpy(result[0], d_result, rowsA * colsB * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed for result: %s\n", cudaGetErrorString(cudaStatus));
        return;
    }

    // Free device memory
    cudaFree(d_matrixA);
    cudaFree(d_matrixB);
    cudaFree(d_result);
}

// Function to save matrix to a file
void save_matrix_to_file(double** matrix, int rows, int cols, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%.10lf ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to read matrix from a file
void read_matrix_from_file(const char* filename, double** matrix, int rows, int cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%lf", &matrix[i][j]) != 1) {
                printf("Error reading matrix from file.\n");
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}
char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

int main(int argc, char *argv[]) {
    int rowsA, colsA, rowsB, colsB;
    char *filename1, *filename2;
    if (argc == 7) {
        rowsA = atoi(argv[1]);
        colsA = atoi(argv[2]);
        rowsB = atoi(argv[3]);
        colsB = atoi(argv[4]);
        filename1 = argv[5];
        filename2 = argv[6];
    } else {
        printf("Usage: %s <rowsA> <colsA> <rowsB> <colsB> <filename1> <filename2>\n", argv[0]);
        return 1;
    }

    if (colsA != rowsB) {
        printf("Matrix multiplication is not possible due to incompatible dimensions.\n");
        return 1;
    }
    // Dynamic allocation of matrices
    double **matrixA = allocate_matrix(rowsA, colsA);
    double **matrixB = allocate_matrix(rowsB, colsB);
    double **result = allocate_matrix(rowsA, colsB);

    // Read matrices from files (assuming files exist and have correct format)
    read_matrix_from_file(filename1, matrixA, rowsA, colsA);
    read_matrix_from_file(filename2, matrixB, rowsB, colsB);

    // Perform multiplication on the GPU
    clock_t start = clock();
    matrix_multiplication_cuda(matrixA, matrixB, rowsA, colsA, colsB, result);
    clock_t end = clock();

    int cpu_time_used = ((1000)*(end - start)) / CLOCKS_PER_SEC;
    printf("%d\n", cpu_time_used);

    // Save the result to a file
    save_matrix_to_file(result, rowsA, colsB, "matrixC_CUDA.txt");

    // Freeing dynamically allocated memory
    //free_matrix(matrixA, rowsA);
    //free_matrix(matrixB, rowsB);
    //free_matrix(result, rowsA);

    return cpu_time_used;
}
