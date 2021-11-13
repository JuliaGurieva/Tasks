#include <initializer_list>
#include <algorithm>
#include <iostream>
const int kBlockSize = 128;

class Deque {
public:
    Deque() : ring_buff_(nullptr), size_(0), capacity_(0), buffer_ind_(0), block_ind_(0){};

    Deque(const Deque& rhs)
        : size_(rhs.size_),
          capacity_(rhs.capacity_),
          buffer_ind_(rhs.buffer_ind_),
          block_ind_(rhs.block_ind_) {
        if (capacity_) {
            ring_buff_ = new int*[capacity_];
            for (size_t i = 0; i < capacity_; ++i) {
                ring_buff_[i] = new int[kBlockSize];
                if (i == buffer_ind_) {
                    size_t j = block_ind_;
                    while (j < kBlockSize && kBlockSize * i + j < size_) {
                        ring_buff_[i][j] = rhs.ring_buff_[i][j];
                        j++;
                    }
                }
                if (i > buffer_ind_) {
                    size_t j = 0;
                    while (j < kBlockSize && kBlockSize * i + j < size_) {
                        ring_buff_[i][j] = rhs.ring_buff_[i][j];
                        j++;
                    }
                }
            }
        } else {
            ring_buff_ = nullptr;
        }
    }

    Deque(Deque&& rhs)
        : ring_buff_(rhs.ring_buff_),
          size_(rhs.size_),
          capacity_(rhs.capacity_),
          buffer_ind_(rhs.buffer_ind_),
          block_ind_(rhs.block_ind_) {
        rhs.ring_buff_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
        rhs.buffer_ind_ = 0;
        rhs.block_ind_ = 0;
    }

    explicit Deque(size_t size) : ring_buff_(new int*[size]), size_(size), capacity_(size) {
        buffer_ind_ = 0;
        block_ind_ = 0;
        for (size_t i = 0; i < capacity_; ++i) {
            ring_buff_[i] = new int[kBlockSize];
            for (size_t j = 0; j < kBlockSize; ++j) {
                if (kBlockSize * i + j == size) {
                    break;
                }
                ring_buff_[i][j] = 0;
            }
        }
    }

    Deque(std::initializer_list<int> list) : size_(list.size()), capacity_(list.size()) {
        if (list.size()) {
            ring_buff_ = new int*[list.size()];
            buffer_ind_ = 0;
            block_ind_ = 0;
            size_t ind = 0;
            for (size_t i = 0; i < capacity_; ++i) {
                ring_buff_[i] = new int[kBlockSize];
                for (size_t j = 0; j < kBlockSize; ++j) {
                    if (ind == size_) {
                        break;
                    }
                    ring_buff_[i][j] = *(list.begin() + ind);
                    ++ind;
                }
            }
        } else {
            ring_buff_ = nullptr;
            buffer_ind_ = 0;
            block_ind_ = 0;
        }
    }
    size_t Size() {
        return size_;
    }

    size_t Size() const {
        return size_;
    }

    ~Deque() {
        for (size_t i = 0; i < capacity_; ++i) {
            delete[] ring_buff_[i];
        }
        delete[] ring_buff_;
    }

    int& operator[](size_t i) {
        size_t buffer_i = ((block_ind_ + i) / kBlockSize + buffer_ind_) % capacity_;
        size_t block_i = (block_ind_ + i) % kBlockSize;
        return ring_buff_[buffer_i][block_i];
    }
    int& operator[](size_t i) const {
        size_t buffer_i = ((block_ind_ + i) / kBlockSize + buffer_ind_) % capacity_;
        size_t block_i = (block_ind_ + i) % kBlockSize;
        return ring_buff_[buffer_i][block_i];
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(buffer_ind_, rhs.buffer_ind_);
        std::swap(block_ind_, rhs.block_ind_);
        std::swap(ring_buff_, rhs.ring_buff_);
    }

    void PushBack(int value) {
        if (capacity_ * kBlockSize > size_) {
            this->operator[](size_) = value;
        } else if (size_ == capacity_ * kBlockSize) {  // realloc
            int** tmp = new int*[2 * capacity_ + 1];
            for (size_t i = 0; i < capacity_; ++i) {
                tmp[i] = ring_buff_[(buffer_ind_ + i) % capacity_];
            }
            for (size_t i = capacity_; i < 2 * capacity_ + 1; ++i) {
                tmp[i] = new int[kBlockSize];
            }
            tmp[capacity_][0] = value;
            delete[] ring_buff_;
            ring_buff_ = tmp;
            capacity_ *= 2;
            ++capacity_;
        }
        ++size_;
    }

    void PopBack() {
        --size_;
    }

    void PushFront(int value) {
        if (size_ < kBlockSize * capacity_) {
            if (block_ind_ > 0) {
                ring_buff_[buffer_ind_][block_ind_ - 1] = value;
                --block_ind_;
            } else if (buffer_ind_ != 0) {
                ring_buff_[buffer_ind_ - 1][kBlockSize - 1] = value;
                --buffer_ind_;
                block_ind_ = kBlockSize - 1;
            } else {
                ring_buff_[capacity_ - 1][kBlockSize - 1] = value;
                buffer_ind_ = capacity_ - 1;
                block_ind_ = kBlockSize - 1;
            }
        } else if (!capacity_) {
            ring_buff_ = new int*[1];
            ring_buff_[0] = new int[kBlockSize];
            capacity_ = 1;
            ring_buff_[0][kBlockSize - 1] = value;
            block_ind_ = kBlockSize - 1;
            size_ = 0;
        } else if (size_ == kBlockSize * capacity_) {  // realloc
            int** tmp = new int*[2 * capacity_];
            for (size_t i = 0; i < capacity_; ++i) {
                tmp[capacity_ + i] = ring_buff_[(buffer_ind_ + i) % capacity_];
            }
            for (size_t i = 0; i < capacity_; ++i) {
                tmp[i] = new int[kBlockSize];
            }
            tmp[capacity_ - 1][kBlockSize - 1] = value;
            buffer_ind_ = capacity_ - 1;
            block_ind_ = kBlockSize - 1;
            delete[] ring_buff_;
            ring_buff_ = tmp;
            capacity_ *= 2;
        }
        ++size_;
    }

    void PopFront() {
        if (block_ind_ == kBlockSize - 1) {
            ++buffer_ind_;
            block_ind_ = 0;
        } else {
            ++block_ind_;
        }
        --size_;
    }

    void Clear() {
        buffer_ind_ = 0;
        block_ind_ = 0;
        size_ = 0;
    }

private:
    int** ring_buff_;
    size_t size_;
    size_t capacity_;
    size_t buffer_ind_ = 0;
    size_t block_ind_ = 0;
};
