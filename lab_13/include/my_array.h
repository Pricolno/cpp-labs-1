#ifndef LAB_13_MY_ARRAY_H
#define LAB_13_MY_ARRAY_H

#include <cstdlib>
#include <array>

namespace lab_13 {

    template<typename T, std::size_t N>
    class my_array {
    public:
        my_array() = default;

        T &at(std::size_t index);
        const T &at(std::size_t index) const;

        T &operator[](std::size_t index);
        const T &operator[](std::size_t index) const;

        bool empty() const;
        std::size_t size() const;

        void fill(const T &val);

    private:
        T data[N];
    };

}

#include "my_array_impl.h"
#include "my_array_impl_bool.h"

#endif //LAB_13_MY_ARRAY_H
