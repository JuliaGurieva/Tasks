#include <string>

class WeakPtr;

struct Block {
    int strong;
    int weak;
};

class SharedPtr {
public:
    SharedPtr() {
        obj_ = nullptr;
        block_ = nullptr;
    }
    SharedPtr(const std::string& s) {
        obj_ = new std::string(s);
        block_ = new Block{1, 0};
    }
    SharedPtr(std::string* s) {
        obj_ = s;
        block_ = new Block{1, 0};
    }

    SharedPtr(const SharedPtr& other) {
        obj_ = other.obj_;
        block_ = other.block_;
        if (block_) {
            ++(block_->strong);
        }
    }

    SharedPtr(SharedPtr&& other) {
        obj_ = other.obj_;
        block_ = other.block_;
        other.obj_ = nullptr;
        other.block_ = nullptr;
    }

    std::string& operator*() {
        return *obj_;
    }

    const std::string& operator*() const {
        return *obj_;
    }

    std::string* Get() {
        return obj_;
    }

    const std::string* Get() const {
        return obj_;
    }

    SharedPtr& operator=(const std::string& s) {
        *obj_ = s;
        return *this;
    }

    void Reset(std::string* s) {
        if (block_) {
            --(block_->strong);
            if (block_->strong == 0) {
                delete obj_;
            }
            obj_ = s;
            if (block_->weak + block_->strong == 0) {
                delete block_;
            }
            if (s) {
                block_ = new Block{1, 0};
            } else {
                block_ = nullptr;
            }
        }
    }

    void Swap(SharedPtr& other) {
        std::swap(obj_, other.obj_);
        std::swap(block_, other.block_);
    }

    SharedPtr& operator=(SharedPtr other) {
        Swap(other);
        return *this;
    }

    SharedPtr(const WeakPtr& rhs);

    ~SharedPtr() {
        if (block_ && obj_) {
            --(block_->strong);
            if (block_->strong == 0) {
                delete obj_;
            }
            if (block_->weak + block_->strong == 0) {
                delete block_;
            }
        } else if (!block_) {
            delete block_;
        }
    }
    friend class WeakPtr;

private:
    std::string* obj_;
    Block* block_;
};

class WeakPtr {
public:
    friend class SharedPtr;

    WeakPtr() {
        obj_ = nullptr;
        block_ = nullptr;
    }

    WeakPtr(std::string*) = delete;

    WeakPtr(const SharedPtr& other) {
        obj_ = other.obj_;
        block_ = other.block_;
        if (block_) {
            ++(block_->weak);
        }
    }

    WeakPtr(const WeakPtr& other) {
        obj_ = other.obj_;
        block_ = other.block_;
        if (block_) {
            ++(block_->weak);
        }
    }

    WeakPtr(WeakPtr&& other) {
        obj_ = other.obj_;
        block_ = other.block_;
        other.obj_ = nullptr;
        other.block_ = nullptr;
    }

    void Swap(WeakPtr& other) {
        std::swap(obj_, other.obj_);
        std::swap(block_, other.block_);
    }
    WeakPtr& operator=(WeakPtr other) {
        Swap(other);
        return *this;
    }

    bool IsExpired() const {
        if (block_) {
            return block_->strong == 0;
        } else {
            return 1;
        }
    }

    SharedPtr Lock() {
        return SharedPtr(*this);
    }

    ~WeakPtr() {
        if (block_) {
            --(block_->weak);
            if (block_ && block_->weak + block_->strong == 0) {
                delete block_;
            }
        } else {
            delete block_;
        }
    }

    std::string* Get() {
        return obj_;
    }

private:
    std::string* obj_;
    Block* block_;
};

SharedPtr::SharedPtr(const WeakPtr& rhs) {
    obj_ = rhs.obj_;
    block_ = rhs.block_;
    if (block_ && block_->strong > 0) {
        ++(rhs.block_->strong);
        block_ = rhs.block_;
    }
    if (block_ && block_->strong == 0) {
        obj_ = nullptr;
        *block_ = {1, 0};
    }
}
