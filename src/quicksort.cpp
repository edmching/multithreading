#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
int partition(std::vector<int>& data, int low, int high) {
  // your code here
	int pivot = data[high];
	int i = low - 1;
	int temp;
	for (int j = low; j < high ; j++)
	{
		if (data[j] < pivot)
		{
			i++;
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}
	if (data[high] < data[i + 1])
	{
		temp = data[i + 1];
		data[i + 1] = data[high];
		data[high] = temp;
	}

	return i + 1;
}

// sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high) {
  // your code here
	int pivot;
	if (low < high)
	{
		pivot = partition(data, low, high);
		quicksort(data, low, pivot - 1);
		quicksort(data, pivot + 1, high);
	}
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
  // your code here
	int pivot;
	if (low < high)
	{
		pivot = partition(data, low, high);
		std::thread t1(&quicksort, data, low, pivot - 1);
		std::thread t2(&quicksort, data, pivot + 1, high);
	}

}

int main() {

  // create two copies of random data
  const int VECTOR_SIZE = 1000000;
  std::vector<int> v1(VECTOR_SIZE, 0);
  // fill with random integers
  for (int i=0; i<VECTOR_SIZE; ++i) {
    v1[i] = rand();
  }
  std::vector<int> v2 = v1;  // copy all contents

  // sort v1 using sequential algorithm
 // quicksort(v1, 0, v1.size()-1);

 // sort v2 using parallel algorithm
  parallel_quicksort(v2, 0, v2.size() - 1);


  for (int j = 0; j < VECTOR_SIZE-1; ++j)
  {
	  std::cout << v2[j] << std::endl;
	  if (v2[j] > v2[j + 1])
	  {
		  std::cout << "the vector is not sorted" << std::endl;
		  break;
	  }

  }

  return 0;
}
