#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

struct Queue {
    std::stack<int> head;
    std::stack<int> tail;
    std::stack<int> max_head;
    std::stack<int> max_tail;
    void EnQueue(int elem) {
        tail.push(elem);
        if (max_tail.empty()) {
            max_tail.push(elem);
        } else {
            max_tail.push(std::max(elem, max_tail.top()));
        }
    }
    void DeQueue() {
        if (head.empty()) {
            while (!tail.empty()) {
                if (head.empty()) {
                    max_head.push(tail.top());
                } else {
                    max_head.push(std::max(tail.top(), max_head.top()));
                }
                head.push(tail.top());
                tail.pop();
                max_tail.pop();
            }
        }
        head.pop();
        max_head.pop();
    }
    int Max() {
        if (max_head.empty()) {
            return max_tail.top();
        } else if (max_tail.empty()) {
            return max_head.top();
        } else {
            return std::max(max_head.top(), max_tail.top());
        }
    }
};

int array[100000];

std::vector<int> FindMax(const std::string& input) {
    Queue loc_max;
    std::vector<int> lmax;
    size_t left = 0, right = 0;
    loc_max.EnQueue(array[0]);
    for (char let : input) {
        if (let == 'R') {
            right++;
            loc_max.EnQueue(array[right]);
        } else {
            left++;
            loc_max.DeQueue();
        }
        lmax.push_back(loc_max.Max());
    }
    return lmax;
}

int main() {
    size_t num_n;
    std::cin >> num_n;
    for (size_t ind = 0; ind < num_n; ++ind) {
        std::cin >> array[ind];
    }
    size_t num_m;
    std::cin >> num_m;
    std::string input;
    for (size_t ind = 0; ind < num_m; ++ind) {
        char let;
        std::cin >> let;
        if (let != ' ') {
            input += let;
        }
    }
    std::vector<int> res = FindMax(input);
    for (int el : res) {
        std::cout << el << ' ';
    }
    return 0;
}
