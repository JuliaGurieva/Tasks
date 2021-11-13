#include <iostream>

const int kNum = 1000000;
int array[kNum];
int storage[kNum];

void Merge(int* first, size_t num_first, int* second, size_t num_second, int* store) {
    size_t j_a = 0;
    size_t j_b = 0;
    for (size_t ind = 0; ind < num_first + num_second; ++ind) {
        if (j_a == num_first && j_b < num_second) {
            store[ind] = second[j_b];
            j_b++;
        } else if (j_b == num_second && j_a < num_first) {
            store[ind] = first[j_a];
            j_a++;
        } else {
            if (first[j_a] >= second[j_b]) {
                store[ind] = second[j_b];
                j_b++;
            } else {
                store[ind] = first[j_a];
                j_a++;
            }
        }
    }
}

void MergeParts(int* store, size_t num_n, size_t num_m) {
    size_t part_size = 1;
    size_t length = num_n * num_m;
    while (2 * part_size <= num_n) {
        size_t part = part_size * num_m;
        size_t merged = 0;
        while (merged < length) {
            if (length - merged < 2 * part && merged >= 2 * part) {
                for (size_t ind = 0; ind < 2 * part; ++ind) {
                    array[merged - 2 * part + ind] = store[merged - 2 * part + ind];
                }
                Merge(array + merged - 2 * part, 2 * part, array + merged, length - merged,
                      store + merged - 2 * part);
            } else {
                Merge(array + merged, part, array + merged + part, part, store + merged);
            }
            merged += 2 * part;
        }
        part_size *= 2;
        for (size_t ind = 0; ind < length; ++ind) {
            array[ind] = store[ind];
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t num_n, num_m;
    std::cin >> num_n >> num_m;
    size_t length = num_n * num_m;
    for (size_t ind = 0; ind < length; ++ind) {
        std::cin >> array[ind];
    }
    MergeParts(storage, num_n, num_m);
    for (size_t ind = 0; ind < length; ++ind) {
        std::cout << array[ind] << ' ';
    }
    return 0;
}
