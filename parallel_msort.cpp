#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

void mergeSort(vector<int>& arr, int low, int high) {
  if (low < high) {
    int mid = (low + high) / 2;

    // Recursively sort the left and right halves.
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);

    // Merge the sorted halves.
    #pragma omp parallel for
    for (int i = 0; i < high - low + 1; i++) {
      int j = low;
      int k = mid + 1;
      int t = low;

      while (j <= mid && k <= high) {
        if (arr[j] <= arr[k]) {
          arr[t++] = arr[j++];
        } else {
          arr[t++] = arr[k++];
        }
      }

      while (j <= mid) {
        arr[t++] = arr[j++];
      }

      while (k <= high) {
        arr[t++] = arr[k++];
      }
    }
  }
}

int main() {
  int n;
  cout << "Enter the number of elements: ";
  cin >> n;

  vector<int> arr(n);

  for (int i = 0; i < n; i++) {
    cout << "Enter element " << i + 1 << ": ";
    cin >> arr[i];
  }

  cout << "Unsorted array: ";
  for (int i = 0; i < n; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;

  // Merge sort.
  cout << "Sequential merge sort: ";
  auto start = std::chrono::high_resolution_clock::now();
  mergeSort(arr, 0, n - 1);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cout << elapsed.count() << " seconds" << endl;
  
  // Parallel merge sort.
  cout << "Parallel merge sort: ";
  start = std::chrono::high_resolution_clock::now();
  #pragma omp parallel
  mergeSort(arr, 0, n - 1);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << elapsed.count() << " seconds" << endl;

  return 0;
}
