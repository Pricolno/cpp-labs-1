#ifndef LAB_11_MY_VECTOR_H
#define LAB_11_MY_VECTOR_H

#include <ostream>
#include <type_traits>

namespace cool {
    template<typename T>
    class my_vector {
    public:
        my_vector();
        my_vector(std::size_t n, const T &value);
        explicit my_vector(std::size_t n);

        my_vector(const my_vector &other);
        my_vector(my_vector &&other) noexcept;

        my_vector &operator=(const my_vector &other);
        my_vector &operator=(my_vector &&other) noexcept;

        std::size_t size() const;
        std::size_t capacity() const;
        bool empty() const;

        void resize(std::size_t n) noexcept(std::is_nothrow_constructible_v<T>);
        void reserve(std::size_t n) noexcept(std::is_nothrow_constructible_v<T>);

        T &operator[](std::size_t index);
        const T &operator[](std::size_t index) const;

        void push_back(const T &t);
        void push_back(T &&t) noexcept(std::is_nothrow_move_constructible_v<T>);

        template<typename ...Args>
        void emplace_back(Args &&...args);

        void pop_back();
        void clear();

        void swap(my_vector<T> &other) noexcept;

        ~my_vector() noexcept(std::is_nothrow_destructible_v<T>);

    private:
        T *data = nullptr;
        std::size_t sz = 0, cp = 0;
    };

    template<typename T>
    inline std::ostream &operator<<(std::ostream &out, const my_vector<T> &vec);
}

#include "my_vector_impl.h"

#endif //LAB_11_MY_VECTOR_H
