#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

class LruCache {
public:
    LruCache(size_t max_size) : max_size_(max_size) {
    }

    void Set(const std::string& key, const std::string& value) {
        auto it = m_.find(key);
        if (it == m_.end() && m_.size() < max_size_) {
            list_.push_back(value);
            auto lend = list_.end();
            m_[key] = --lend;
        } else if (it != m_.end()) {
            list_.erase(m_[key]);
            list_.push_back(value);
            auto lend = list_.end();
            m_[key] = --lend;
        } else if (m_.size() == max_size_) {
            for (auto i = m_.begin(); i != m_.end(); i++) {
                if (i->second == list_.begin()) {
                    m_.erase(i);
                    break;
                }
            }
            list_.pop_front();
            list_.push_back(value);
            auto lend = list_.end();
            m_[key] = --lend;
        }
    }

    bool Get(const std::string& key, std::string* value) {
        auto it = m_.find(key);
        if (it != m_.end()) {
            *value = *(m_[key]);
            list_.erase(m_[key]);
            list_.push_back(*value);
            auto lend = list_.end();
            m_[key] = --lend;
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, std::list<std::string>::iterator> m_;
    size_t max_size_;
    std::list<std::string> list_;
};
