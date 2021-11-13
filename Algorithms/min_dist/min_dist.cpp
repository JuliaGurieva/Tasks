#include <iostream>

uint32_t par_a = 0;
uint32_t par_b = 0;
uint32_t cur = 0;

uint32_t NextRand() {
    cur = cur * par_a + par_b;
    uint32_t first = cur >> 8;
    cur = cur * par_a + par_b;
    uint32_t second = cur >> 8;
    return (first << 8) ^ second;
}

uint32_t UAbs(uint32_t first, uint32_t second) {
    if (first >= second) {
        return first - second;
    } else {
        return second - first;
    }
}

uint32_t KOrderStat(uint32_t* array, int left, int right, int ind) {
    while (left < right) {
        uint64_t x = array[ind];
        int first = left;
        int second = right;
        while (first <= second) {
            while (array[first] < x) {
                first++;
            }
            while (array[second] > x) {
                second--;
            }
            if (first <= second) {
                uint64_t temp = array[first];
                array[first] = array[second];
                array[second] = temp;
                first++;
                second--;
            }
        }
        if (second < ind) {
            left = first;
        }
        if (ind < first) {
            right = second;
        }
    }
    return array[ind];
}

uint32_t x[10000000];

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t num;
    std::cin >> num >> par_a >> par_b;
    for (size_t ind = 0; ind < num; ++ind) {
        x[ind] = NextRand();
    }
    uint32_t y;
    if (num % 2) {
        y = KOrderStat(x, 0, num - 1, num / 2);
    } else {
        y = KOrderStat(x, 0, num - 1, num / 2 - 1);
    }
    uint64_t min_sum = 0ll;
    for (size_t ind = 0; ind < num; ++ind) {
        min_sum += UAbs(x[ind], y);
    }
    std::cout << min_sum;
    return 0;
}
