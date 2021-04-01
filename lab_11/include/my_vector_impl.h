#include <memory>
#include <utility>
#include <algorithm>

namespace cool {
    template<typename T>
    my_vector<T>::my_vector() = default;

    template<typename T>
    std::size_t my_vector<T>::size() const {
        return sz;
    }

    template<typename T>
    std::size_t my_vector<T>::capacity() const {
        return cp;
    }

    template<typename T>
    bool my_vector<T>::empty() const {
        return sz == 0;
    }

    template<typename T>
    my_vector<T>::my_vector(std::size_t n, const T &value) {
        reserve(n);
        std::size_t i;
        try {
            for (i = 0; i < n; ++i)
                new(data + i) T(value);
        } catch (...) {
            std::destroy(data, data + i);
            throw;
        }
        sz = n;
    }

    template<typename T>
    my_vector<T>::my_vector(std::size_t n) {
        resize(n);
    }

    template<typename T>
    my_vector<T>::my_vector(const my_vector &other) {
        *this = other;
    }

    template<typename T>
    my_vector<T> &my_vector<T>::operator=(const my_vector &other) {
        if (this == &other)
            return *this;

        reserve(other.size());
        if (sz < other.sz) {
            std::copy_n(other.data, sz, data);
            std::uninitialized_copy_n(other.data + sz, other.sz - sz, data + sz);
        } else {
            std::copy_n(other.data, other.sz, data);
            std::destroy_n(other.data + other.sz, sz - other.sz);
        }
        sz = other.sz;
        return *this;
    }

    template<typename T>
    my_vector<T>::my_vector(my_vector &&other) noexcept {
        *this = std::move(other);
    }

    template<typename T>
    my_vector<T> &my_vector<T>::operator=(my_vector &&other) noexcept {
        swap(other);
        return *this;
    }

    template<typename T>
    T &my_vector<T>::operator[](std::size_t index) {
        return data[index];
    }

    template<typename T>
    const T &my_vector<T>::operator[](std::size_t index) const {
        return data[index];
    }

    template<typename T>
    void my_vector<T>::push_back(const T &t) {
        if (sz == cp) reserve(sz + 1); // automatically fits in power of 2
        new(data + sz) T(t);
        ++sz;
    }

    template<typename T>
    void my_vector<T>::push_back(T &&t) noexcept(std::is_nothrow_move_constructible_v<T>) {
        if (sz == cp) reserve(sz + 1);
        new(data + sz) T(std::move(t));
        ++sz;
    }

    template<typename T>
    template<typename ...Args>
    void my_vector<T>::emplace_back(Args &&...args) {
        if (sz == cp) reserve(sz + 1);
        new(data + sz) T(std::forward<Args>(args)...);
        ++sz;
    }

    template<typename T>
    void my_vector<T>::pop_back() {
        std::destroy_at(data + sz - 1);
        --sz;
    }

    template<typename T>
    void my_vector<T>::clear() {
        std::destroy_n(data, sz);
        sz = 0;
    }

    template<typename T>
    void my_vector<T>::resize(std::size_t n) noexcept(std::is_nothrow_constructible_v<T>) {
        reserve(n);
        if (n > sz)
            std::uninitialized_default_construct_n(data, n - sz);
        else if (n < sz)
            std::destroy(data + n, data + sz);
        sz = n;
    }

    template<typename T>
    void my_vector<T>::reserve(std::size_t n) noexcept(std::is_nothrow_constructible_v<T>) {
        if (n < cp)
            return;

        std::size_t n_ = 1;
        while (n_ < n)
            n_ *= 2;
        T *new_data = static_cast<T *>(operator new(n_ * sizeof(T)));
        try {
            if constexpr (!std::is_nothrow_move_constructible_v<T>
                          && std::is_copy_constructible_v<T>)
                std::uninitialized_copy_n(data, sz, new_data);
            else
                std::uninitialized_move_n(data, sz, new_data);

            std::destroy_n(data, sz);
            if (data) operator delete(data);

            data = new_data;
            cp = n_;
        } catch (...) {
            operator delete(new_data);
            std::rethrow_exception(std::current_exception());
        }
    }

    template<typename T>
    void my_vector<T>::swap(my_vector &other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cp, other.cp);
    }

    template<typename T>
    my_vector<T>::~my_vector() noexcept(std::is_nothrow_destructible_v<T>) {
        std::destroy_n(data, sz);
        operator delete(data);
        sz = 0, cp = 0;
        data = nullptr;
    }

    template<typename T>
    inline std::ostream &operator<<(std::ostream &out, const my_vector <T> &vec) {
        for (std::size_t i = 0; i < vec.size(); ++i) {
            out << vec[i];
            if (i + 1 < vec.size())
                out << " ";
        }
        return out;
    }

}
