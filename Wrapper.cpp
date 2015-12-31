//a simple refs counting & auto release wrapper like shared_ptr
#include <cstddef>

template <typename T>
class Wrapper{
public:
    Wrapper(T* ptr) : data_(ptr), ref_(1) {}

    ~Wrapper() {
        if (release() == 0) {
            delete data_;
        }
    }

    Wrapper(const Wrapper<T>& other) : data_(other.data_), ref_(other.ref_) {
        add();    
    }

    Wrapper<T>& operator = (const Wrapper<T>& rhs) {
        if (this != rhs) {
            if (release == 0) {
                delete data_;
            }

            data_ = rhs.data_;
            ref_ = rhs.ref_;
            add();
        }
        return *this;
    }

    T& operator * () {
        return *data_;
    }

    T* operator -> () {
        return data_;
    }
private:
    void add() {
        ref_++;
    }

    size_t release() {
        return --ref_;
    }

    T* data_;
    size_t ref_;
};

