#include <stdexcept>

namespace lab_13 {
    template<typename T, std::size_t N>
    std::size_t my_array<T, N>::size() const {
        return N;
    }

    template<typename T, std::size_t N>
    bool my_array<T, N>::empty() const {
        return N == 0u;
    }

    template<typename T, std::size_t N>
    T &my_array<T, N>::at(std::size_t index) {
        if (index >= N)
            throw std::out_of_range("Array index out of range.");
        return data[index];
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::at(std::size_t index) const {
        if (index >= N)
            throw std::out_of_range("Array index out of range.");
        return data[index];
    }

    template<typename T, std::size_t N>
    T &my_array<T, N>::operator[](std::size_t index) {
        return data[index];
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::operator[](std::size_t index) const {
        return data[index];
    }

    template<typename T, std::size_t N>
    void my_array<T, N>::fill(const T &val) {
        for (std::size_t i = 0u; i < N; ++i)
            data[i] = val;
    }
}