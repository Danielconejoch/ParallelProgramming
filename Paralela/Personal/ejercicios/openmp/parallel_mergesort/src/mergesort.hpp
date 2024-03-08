// Copyright 2023 Jeisson Hidalgo jeisson.hidalgo@ucr.ac.cr CC-BY-4
#include <algorithm>
#include <vector>

// TODO(you): parallelize merge sort
template <typename Type>
void mergesort(std::vector<Type>& values, const ptrdiff_t left,
    const ptrdiff_t right) {
  // Count of elements we have to sort
  const ptrdiff_t count = right - left;
  if (count > 0) {
    const size_t mid = (left + right) / 2;
    // Sort left half of the array
    #pragma omp taskgroup
    {
      #pragma omp task default(none) shared(values) firstprivate(left, mid)
      mergesort(values, left, mid);
      #pragma omp task default(none) shared(values) firstprivate(mid, right)
      // Sort right half of the array
      mergesort(values, mid + 1, right);
      #pragma omp taskwait
    }
    
    // Both halves are sorted, merge them into a temp vector
    std::vector<Type> temp;
    temp.reserve(count + 1);
    std::merge(values.begin() + left, values.begin() + mid + 1,
        values.begin() + mid + 1, values.begin() + right + 1,
        std::back_inserter(temp));
    // Copy the sorted temp vector back to the original vector
    std::copy(temp.begin(), temp.end(), values.begin() + left);
  }
}

template <typename Type>
void mergesort(std::vector<Type>& values) {
  #pragma omp parallel default(none) shared (values)
  {
    #pragma omp single
    mergesort(values, 0, values.size() - 1);
  }
}
  
