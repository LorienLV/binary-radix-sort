#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <span>
#include <vector>

/**
 * A sorting algorithm based on the bit representation of the data.
 *
 */

/**
 * Binary Radix Sort recursive implementation.
 * 
 * @param v Vector to sort.
 */
template <typename T>
void bin_radix_sort_rec(std::vector<T> &v) {
    std::function<void(std::span<T>, int)> bin_radix_sort_rec_inm =
        [&](std::span<T> vview, const int bit) {
            if (vview.size() <= 1 || bit < 0) {
                return;
            }

            int left = 0;
            int right = vview.size() - 1;

            while (left <= right) {
                // Get the bith bit.
                const unsigned char b = (vview[left] >> bit) & 1;

                // If the bit is 0, then the element is in the right partition.
                if (b == 0) {
                    ++left;
                }
                // Otherwise, move the element to the other partition.
                else {
                    std::swap(vview[left], vview[right]);
                    --right;
                }
            }

            bin_radix_sort_rec_inm(vview.subspan(0, left), bit - 1);
            bin_radix_sort_rec_inm(vview.subspan(left), bit - 1);
        };

    bin_radix_sort_rec_inm(std::span{v}, sizeof(T) * 8 - 1);
}

/**
 * Binary Radix Sort iterative implementation.
 * 
 * @param v Vector to sort.
 */
template <typename T>
void bin_radix_sort_it(std::vector<T> &v) {
    // Save some instructions by using an array instead of a vector + push_back,
    // pop_back (stack).
    std::vector<std::tuple<int, int, int>> stack(v.size());
    int top = -1;
    stack[++top] = {0, v.size(), sizeof(T) * 8 - 1};

    while (top >= 0) {
        const auto [start, size, bit] = stack[top--];

        if (size <= 1 || bit < 0) {
            continue;
        }

        int left = start;
        int right = start + size - 1;

        while (left <= right) {
            // Get the bith bit.
            const unsigned char b = (v[left] >> bit) & 1;

            // If the bit is 0, then the element is in the right partition.
            if (b == 0) {
                ++left;
            }
            // Otherwise, move the element to the other partition.
            else {
                // Swap
                T tmp = v[left];
                v[left] = v[right];
                v[right] = tmp;

                // std::swap(v[left], v[right]);

                --right;
            }
        }

        stack[++top] = {start, left - start, bit - 1};
        stack[++top] = {left, size - (left - start), bit - 1};
    }
}

/**
 * Check if v1 and v2 are equal.
 * 
 * @param v1 One vector.
 * @param v2 Another vector.
 * @param msg Message to print in case the vectors differ.
 * @return bool True if the vectors are equal, false otherwise.
 */
template <typename T>
static bool equal_vectors(const std::vector<T> &v1,
                          const std::vector<T> &v2,
                          const std::string &msg) {
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            std::cout << msg << " Error: v1[" << i << "] = " << v1[i]
                      << " != " << v2[i] << "\n";
            return false;
        }
    }
    return true;
}

/**
 * Benchmark the Binary Radix Sort algorithm for a given data type.
 * 
 * @param reps Number of repetitions.
 * @param size Size of the vector to sort.
 * @param type_name Human readable name of the data type.
 */
template <typename T>
void benchmark(const int reps, const int size, const std::string &type_name) {
    std::cout << "Benchmarking with type \"" << type_name << "\""
              << "\n";

    auto std_sort_time = std::chrono::nanoseconds::zero();
    auto bin_radix_sort_rec_time = std::chrono::nanoseconds::zero();
    auto bin_radix_sort_it_time = std::chrono::nanoseconds::zero();

    for (int r = 0; r < reps; ++r) {
        std::vector<T> v1(size);
        std::generate(v1.begin(), v1.end(), []() {
            return rand();
        });
        auto v2 = v1;
        auto v3 = v1;

        // std::sort
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(v1.begin(), v1.end());
        auto end = std::chrono::high_resolution_clock::now();
        std_sort_time += end - start;

        // Binary Radix Sort.
        start = std::chrono::high_resolution_clock::now();
        bin_radix_sort_rec(v2);
        end = std::chrono::high_resolution_clock::now();
        bin_radix_sort_rec_time += end - start;

        // Binary Radix Sort.
        start = std::chrono::high_resolution_clock::now();
        bin_radix_sort_it(v3);
        end = std::chrono::high_resolution_clock::now();
        bin_radix_sort_it_time += end - start;

        equal_vectors(v1, v2, std::string("(std_sort_rec)"));
        equal_vectors(v1, v3, std::string("(std_sort_rec)"));
    }

    std::cout << "    std_sort_time (s): " << std_sort_time.count() / 1e9 << "\n";
    std::cout << "    bin_radix_sort_rec_time (s): " << bin_radix_sort_rec_time.count() / 1e9
              << "\n";
    std::cout << "    bin_radix_sort_it_time (s): " << bin_radix_sort_it_time.count() / 1e9
              << "\n";
}

int main(int argc, char const *argv[]) {
    const int reps = argc > 1 ? std::atoi(argv[1]) : 1;
    const int size = argc > 2 ? std::atoi(argv[2]) : 10;

    std::cout << "Reps: " << reps << "\n";
    std::cout << "Vector size: " << size << "\n";

    srand(time(NULL));

    benchmark<uint8_t>(reps, size, std::string("uint8_t"));
    benchmark<uint16_t>(reps, size, std::string("uint16_t"));
    benchmark<uint32_t>(reps, size, std::string("uint32_t"));
    benchmark<uint64_t>(reps, size, std::string("uint64_t"));

    return 0;
}
