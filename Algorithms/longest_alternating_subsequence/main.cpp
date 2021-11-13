#include <iostream>
#include <vector>

bool IntervalType(const std::vector<int>& seq, int ind) {
    if (ind == static_cast<int>(seq.size()) - 1) {
        return true;
    }
    return (seq[ind + 1] - seq[ind] > 0);
}

int main() {
    int len;
    std::vector<int> seq;
    std::cin >> len;
    std::vector<int> interval_indexes;
    std::vector<int> curr_indexes;
    std::vector<int> res{};
    int num;
    std::cin >> num;
    seq.push_back(num);
    int16_t sign = 0;
    for (int i = 1; i < len; ++i) {
        std::cin >> num;
        if (num != seq[seq.size() - 1]) {
            seq.push_back(num);
        }
    }
    curr_indexes = {0};
    interval_indexes.push_back(0);
    if (seq.size() > 1) {
        sign = seq[1] > seq[0] ? 1 : -1;
        curr_indexes.push_back(1);
    }
    std::vector<bool> sub_sequence(seq.size(), false);
    sub_sequence[0] = true;
    for (size_t ind = 2; ind < seq.size(); ++ind) {
        if (curr_indexes.size() == 1) {
            sign = seq[ind] > seq[ind - 1] ? 1 : -1;
        }
        if ((seq[ind] - seq[ind - 1]) * sign > 0) {
            curr_indexes.push_back(ind);
        } else {
            sub_sequence[ind - 1] = true;
            interval_indexes.push_back(ind);
            curr_indexes = {};
            curr_indexes.push_back(ind);
        }
    }
    if (interval_indexes.size() == 1) {
        res.push_back(0);
        if (seq.size() > 1) {
            sub_sequence[1] = true;
            res.push_back(1);
        }
    } else if (interval_indexes.size() > 1) {
        sub_sequence[seq.size() - 1] = true;
        for (size_t ind = 1; ind < interval_indexes.size(); ++ind) {
            if (IntervalType(seq, interval_indexes[ind]) ==
                IntervalType(seq, interval_indexes[ind - 1])) {
                sub_sequence[interval_indexes[ind]] = 1;
            }
        }
        for (size_t ind = 0; ind < sub_sequence.size(); ++ind) {
            if (sub_sequence[ind]) {
                res.push_back(ind);
            }
        }
        int interval_id = 0;
        for (size_t ind = 1; ind < res.size(); ++ind) {
            if (res[ind] != interval_indexes[interval_id]) {
                int right = res[ind];
                sign = (seq[res[ind]] - seq[res[ind - 1]] > 0) ? 1 : -1;
                if (ind < res.size() - 1) {
                    while (sign * (seq[right] - seq[res[ind - 1]]) > 0 &&
                           sign * (seq[right] - seq[res[ind + 1]]) > 0 &&
                           right >= interval_indexes[interval_id]) {
                        right--;
                    }
                } else {
                    while (sign * (seq[right] - seq[res[ind - 1]]) > 0 &&
                           right >= interval_indexes[interval_id]) {
                        right--;
                    }
                }
                res[ind] = right + 1;
                interval_id++;
            }
        }
    }
    for (int el : res) {
        std::cout << seq[el] << " ";
    }
    return 0;
}
