
#include <iostream>
#include <vector>

void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum element in the unsorted part
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum element with the first element of the unsorted part
        std::swap(arr[i], arr[minIndex]);
    }
}

void sort(std::vector<int>& arr) {
    selectionSort(arr);  // Call the selection sort algorithm
}

int main() {
    std::vector<int> arr = {64, 25, 12, 22, 11};

    std::cout << "Unsorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Using the custom sort function (Selection Sort-based implementation)
    sort(arr);

    std::cout << "Sorted array using custom sort: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}


#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no elements were swapped, the array is sorted
        if (!swapped) break;
    }
}

int main() {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

    std::cout << "Unsorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    bubbleSort(arr);

    std::cout << "Sorted array using Bubble Sort: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
