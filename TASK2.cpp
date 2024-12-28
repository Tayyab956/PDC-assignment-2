#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Partition function for quicksort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    #pragma omp parallel for shared(arr, pivot, low, high) default(none)
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            #pragma omp critical
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Sequential Quicksort
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

// Parallel Quicksort using OpenMP
void parallelQuicksort(int arr[], int low, int high, int depth) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        // Parallelize only if depth > 0
        if (depth > 0) {
            #pragma omp parallel sections default(none) shared(arr, low, high, pivotIndex, depth)
            {
                #pragma omp section
                parallelQuicksort(arr, low, pivotIndex - 1, depth - 1);

                #pragma omp section
                parallelQuicksort(arr, pivotIndex + 1, high, depth - 1);
            }
        } else {
            // Fallback to sequential quicksort
            quicksort(arr, low, pivotIndex - 1);
            quicksort(arr, pivotIndex + 1, high);
        }
    }
}

// Function to copy the array
void copyArray(int src[], int dest[], int n) {
    #pragma omp parallel for shared(src, dest, n) default(none)
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int main() {
    const int n = 100000; // Size of the array
    int originalArray[n], arrSequential[n], arrParallel[n];

    // Initialize the array with random values
    srand(time(0));
    #pragma omp parallel for shared(originalArray, n) default(none)
    for (int i = 0; i < n; i++) {
        originalArray[i] = rand() % 10000; // Random values between 0 and 9999
    }

    // Copy the original array for sequential and parallel part
    copyArray(originalArray, arrSequential, n);
    copyArray(originalArray, arrParallel, n);

    // Measure time for Sequential Quicksort
    clock_t startSeq = clock();
    quicksort(arrSequential, 0, n - 1);
    clock_t endSeq = clock();
    double timeSeq = double(endSeq - startSeq) / CLOCKS_PER_SEC;

    // Measure time for Parallel Quicksort
    double startPar = clock();
    parallelQuicksort(arrParallel, 0, n - 1, 4);
    double endPar = clock();
    double timePar = (endPar - startPar) / CLOCKS_PER_SEC;

    // Display the results
    cout << "Time taken by Sequential Quicksort: " << timeSeq << " seconds" << endl;
    cout << "Time taken by Parallel Quicksort: " << timePar << " seconds" << endl;

    return 0;
}
