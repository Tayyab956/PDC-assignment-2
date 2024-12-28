#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to multiply matrices sequentially
void multiplyMatricesSequentially(int* matrixA, int* matrixB, int* resultMatrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            resultMatrix[i * size + j] = 0;
            for (int k = 0; k < size; k++) {
                resultMatrix[i * size + j] += matrixA[i * size + k] * matrixB[k * size + j];
            }
        }
    }
}

// Parallelized matrix multiplication using OpenMP
void multiplyMatricesParallel(int* matrixA, int* matrixB, int* resultMatrix, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            resultMatrix[i * size + j] = 0;
            for (int k = 0; k < size; k++) {
                resultMatrix[i * size + j] += matrixA[i * size + k] * matrixB[k * size + j];
            }
        }
    }
}

// Function to initialize a matrix with random values
void initializeRandomMatrix(int* matrix, int size) {
    for (int i = 0; i < size * size; i++) {
        matrix[i] = rand() % 100; // Random integers from 0 to 99
    }
}

// Function to display the matrix (optional for verification)
void displayMatrix(int* matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i * size + j] << " ";
        }
        cout << endl;
    }
}

int main() {
    const int size = 512; // Size of the matrix (you can change this value for testing)

    // Allocate memory for the matrices
    int* matrixA = new int[size * size];
    int* matrixB = new int[size * size];
    int* resultSequential = new int[size * size];
    int* resultParallel = new int[size * size];

    // Initialize matrices with random values
    srand(time(0));
    initializeRandomMatrix(matrixA, size);
    initializeRandomMatrix(matrixB, size);

    // Measure execution time for Sequential Multiplication
    clock_t startSeq = clock();
    multiplyMatricesSequentially(matrixA, matrixB, resultSequential, size);
    clock_t endSeq = clock();
    double seqTime = double(endSeq - startSeq) / CLOCKS_PER_SEC;

    // Measure execution time for Parallel Multiplication
    clock_t startPar = clock();
    multiplyMatricesParallel(matrixA, matrixB, resultParallel, size);
    clock_t endPar = clock();
    double parTime = double(endPar - startPar) / CLOCKS_PER_SEC;

    // Display the results
    cout << "Time taken for Sequential Multiplication: " << seqTime << " seconds" << endl;
    cout << "Time taken for Parallel Multiplication: " << parTime << " seconds" << endl;

    // Free allocated memory
    delete[] matrixA;
    delete[] matrixB;
    delete[] resultSequential;
    delete[] resultParallel;

    return 0;
}
