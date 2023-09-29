# Binary Radix Sort

An implementation of the **Binary Radix Sort** algorithm.

**Binary Radix Sort** is a simple sorting algorithm that leverages the binary representation of numbers to sort them efficiently. Unlike traditional sorting algorithms like QuickSort or MergeSort, Binary Radix Sort does not rely on comparisons between elements. Instead, it categorizes elements into two buckets (0 and 1) based on their binary digits, from the least significant bit (LSB) to the most significant bit (MSB).

- **Time Complexity**: O(k * n)
  - `n` is the number of elements in the array.
  - `k` is the number of bits required to represent the maximum element.
- **Space Complexity**: O(1)
  - The implementation of the algorithm works in place.
