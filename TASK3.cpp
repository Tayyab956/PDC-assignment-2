#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Merge function
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* leftArr = new int[n1];
    int* rightArr = new int[n2];

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    while (j < n2) {
        arr[k++] = rightArr[j++];
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Sequential Merge Sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(int arr[], int left, int right, int depth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth > 0) {
            #pragma omp parallel sections default(none) shared(arr, left, right, mid, depth)
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid, depth - 1);

                #pragma omp section
                parallelMergeSort(arr, mid + 1, right, depth - 1);
            }
        } else {
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
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

    // Copy the original array for sequential and parallel sorting
    copyArray(originalArray, arrSequential, n);
    copyArray(originalArray, arrParallel, n);

    // Measure time for Sequential Merge Sort
    clock_t startSeq = clock();
    mergeSort(arrSequential, 0, n - 1);
    clock_t endSeq = clock();
    double timeSeq = double(endSeq - startSeq) / CLOCKS_PER_SEC;

    // Measure time for Parallel Merge Sort
    clock_t startPar = clock();
    parallelMergeSort(arrParallel, 0, n - 1, 4);
    clock_t endPar = clock();
    double timePar = double(endPar - startPar) / CLOCKS_PER_SEC;

    // Display the results
    cout << "Time taken by Sequential Merge Sort: " << timeSeq << " seconds" << endl;
    cout << "Time taken by Parallel Merge Sort: " << timePar << " seconds" << endl;

    return 0;
}
