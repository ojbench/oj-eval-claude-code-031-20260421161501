#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <vector>
#include <memory>
#include <set>

class pylist {
    struct Data {
        bool is_int;
        int val;
        std::vector<pylist> list;
        Data() : is_int(false), val(0) {}
        Data(int v) : is_int(true), val(v) {}
    };

    std::shared_ptr<Data> ptr;

public:
    pylist() : ptr(std::make_shared<Data>()) {}
    pylist(int v) : ptr(std::make_shared<Data>(v)) {}

    pylist(const pylist& other) : ptr(other.ptr) {}
    pylist& operator=(const pylist& other) {
        ptr = other.ptr;
        return *this;
    }

    pylist& operator=(int v) {
        ptr = std::make_shared<Data>(v);
        return *this;
    }

    void append(const pylist &x) {
        if (ptr && !ptr->is_int) {
            ptr->list.push_back(x);
        }
    }

    void append(int x) {
        if (ptr && !ptr->is_int) {
            ptr->list.push_back(pylist(x));
        }
    }

    pylist pop() {
        if (ptr && !ptr->is_int && !ptr->list.empty()) {
            pylist last = ptr->list.back();
            ptr->list.pop_back();
            return last;
        }
        return pylist();
    }

    pylist &operator[](size_t i) {
        if (ptr->is_int) return *this;
        return ptr->list[i];
    }

    const pylist &operator[](size_t i) const {
        if (ptr->is_int) return *this;
        return ptr->list[i];
    }

    // Support for bitwise and arithmetic operators
    friend int operator+(int a, const pylist& b) { return a + (int)b; }
    friend int operator+(const pylist& a, int b) { return (int)a + b; }
    friend int operator+(const pylist& a, const pylist& b) { return (int)a + (int)b; }

    friend int operator-(int a, const pylist& b) { return a - (int)b; }
    friend int operator-(const pylist& a, int b) { return (int)a - b; }
    friend int operator-(const pylist& a, const pylist& b) { return (int)a - (int)b; }

    friend int operator*(int a, const pylist& b) { return a * (int)b; }
    friend int operator*(const pylist& a, int b) { return (int)a * b; }
    friend int operator*(const pylist& a, const pylist& b) { return (int)a * (int)b; }

    friend int operator/(int a, const pylist& b) { return a / (int)b; }
    friend int operator/(const pylist& a, int b) { return (int)a / b; }
    friend int operator/(const pylist& a, const pylist& b) { return (int)a / (int)b; }

    friend int operator%(int a, const pylist& b) { return a % (int)b; }
    friend int operator%(const pylist& a, int b) { return (int)a % b; }
    friend int operator%(const pylist& a, const pylist& b) { return (int)a % (int)b; }

    friend int operator&(int a, const pylist& b) { return a & (int)b; }
    friend int operator&(const pylist& a, int b) { return (int)a & b; }
    friend int operator&(const pylist& a, const pylist& b) { return (int)a & (int)b; }

    friend int operator|(int a, const pylist& b) { return a | (int)b; }
    friend int operator|(const pylist& a, int b) { return (int)a | b; }
    friend int operator|(const pylist& a, const pylist& b) { return (int)a | (int)b; }

    friend int operator^(int a, const pylist& b) { return a ^ (int)b; }
    friend int operator^(const pylist& a, int b) { return (int)a ^ b; }
    friend int operator^(const pylist& a, const pylist& b) { return (int)a ^ (int)b; }

    friend int operator<<(const pylist& a, int b) { return (int)a << b; }
    friend int operator>>(const pylist& a, int b) { return (int)a >> b; }

    int operator+() const { return +(int)*this; }
    int operator-() const { return -(int)*this; }

    friend bool operator<(const pylist& a, const pylist& b) { return (int)a < (int)b; }
    friend bool operator>(const pylist& a, const pylist& b) { return (int)a > (int)b; }
    friend bool operator<=(const pylist& a, const pylist& b) { return (int)a <= (int)b; }
    friend bool operator>=(const pylist& a, const pylist& b) { return (int)a >= (int)b; }
    friend bool operator==(const pylist& a, const pylist& b) { return (int)a == (int)b; }
    friend bool operator!=(const pylist& a, const pylist& b) { return (int)a != (int)b; }

    friend bool operator<(int a, const pylist& b) { return a < (int)b; }
    friend bool operator>(int a, const pylist& b) { return a > (int)b; }
    friend bool operator<=(int a, const pylist& b) { return a <= (int)b; }
    friend bool operator>=(int a, const pylist& b) { return a >= (int)b; }
    friend bool operator==(int a, const pylist& b) { return a == (int)b; }
    friend bool operator!=(int a, const pylist& b) { return a != (int)b; }

    friend bool operator<(const pylist& a, int b) { return (int)a < b; }
    friend bool operator>(const pylist& a, int b) { return (int)a > b; }
    friend bool operator<=(const pylist& a, int b) { return (int)a <= b; }
    friend bool operator>=(const pylist& a, int b) { return (int)a >= b; }
    friend bool operator==(const pylist& a, int b) { return (int)a == b; }
    friend bool operator!=(const pylist& a, int b) { return (int)a != b; }

    operator int() const {
        return (ptr && ptr->is_int) ? ptr->val : 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const pylist &ls) {
        static std::set<const Data*> visited;
        if (!ls.ptr) return os;
        if (ls.ptr->is_int) {
            os << ls.ptr->val;
        } else {
            if (visited.count(ls.ptr.get())) {
                os << "[...]";
            } else {
                visited.insert(ls.ptr.get());
                os << "[";
                for (size_t i = 0; i < ls.ptr->list.size(); ++i) {
                    os << ls.ptr->list[i];
                    if (i < ls.ptr->list.size() - 1) os << ", ";
                }
                os << "]";
                visited.erase(ls.ptr.get());
            }
        }
        return os;
    }

    ~pylist() {
        if (ptr && ptr.use_count() == 1 && !ptr->is_int) {
            std::vector<pylist> current_list;
            current_list.swap(ptr->list);
            while (!current_list.empty()) {
                pylist item = std::move(current_list.back());
                current_list.pop_back();
                if (item.ptr && item.ptr.use_count() == 1 && !item.ptr->is_int) {
                    std::vector<pylist> child_list;
                    child_list.swap(item.ptr->list);
                    for(auto& child : child_list) {
                        current_list.push_back(std::move(child));
                    }
                }
            }
        }
    }
};

#endif //PYLIST_H
