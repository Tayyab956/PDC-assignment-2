#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function for Sequential Bubble Sort
void sequentialBubbleSort(int array[], int size) {
    for (int outer = 0; outer < size - 1; outer++) {
        for (int inner = 0; inner < size - outer - 1; inner++) {
            if (array[inner] > array[inner + 1]) {
                int temp = array[inner];
                array[inner] = array[inner + 1];
                array[inner + 1] = temp;
            }
        }
    }
}

// Function for Parallel Bubble Sort using OpenMP
void parallelBubbleSort(int array[], int size) {
    for (int step = 0; step < size - 1; step++) {
        #pragma omp parallel default(none) shared(array, size, step)
        {
            #pragma omp for schedule(static)
            for (int i = (step % 2 == 0 ? 0 : 1); i < size - step - 1; i += 2) {
                if (array[i] > array[i + 1]) {
                    int temp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = temp;
                }
            }
        }
    }
}

// Helper function to copy arrays
void copyArray(const int source[], int target[], int size) {
    #pragma omp parallel for schedule(static)
    for (int index = 0; index < size; index++) {
        target[index] = source[index];
    }
}

int main() {
    const int arraySize = 10000;
    int originalArray[arraySize], sequentialArray[arraySize], parallelArray[arraySize];

    // Fill the original array with random numbers
    srand(static_cast<unsigned>(time(0)));
    #pragma omp parallel for schedule(dynamic, 100)
    for (int i = 0; i < arraySize; i++) {
        originalArray[i] = rand() % 1000;
    }

    // Create copies for sorting
    copyArray(originalArray, sequentialArray, arraySize);
    copyArray(originalArray, parallelArray, arraySize);

    // Time the Sequential Bubble Sort
    clock_t seqStart = clock();
    sequentialBubbleSort(sequentialArray, arraySize);
    clock_t seqEnd = clock();
    double sequentialTime = static_cast<double>(seqEnd - seqStart) / CLOCKS_PER_SEC;

    // Time the Parallel Bubble Sort
    clock_t parStart = clock();
    parallelBubbleSort(parallelArray, arraySize);
    clock_t parEnd = clock();
    double parallelTime = static_cast<double>(parEnd - parStart) / CLOCKS_PER_SEC;

    // Display the execution times
    cout << "Sequential Bubble Sort Time: " << sequentialTime << " seconds" << endl;
    cout << "Parallel Bubble Sort Time: " << parallelTime << " seconds" << endl;

    return 0;
}
