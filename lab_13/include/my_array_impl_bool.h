namespace lab_13 {
    template<std::size_t N>
    class my_array<bool, N> {
    private:
        class proxy;

    public:
        my_array() = default;

        proxy at(std::size_t index);
        bool at(std::size_t index) const;
        proxy operator[](std::size_t index);
        bool operator[](std::size_t index) const;

        bool empty() const {
            return N == 0;
        }

        std::size_t size() const {
            return N;
        }

        void fill(bool val) {
            for (std::size_t i = 0; i < N; ++i)
                (*this)[i] = val;
        }

    private:
        static constexpr std::size_t arr_size = (N + 7) / 8;
        uint8_t data[arr_size] {}; // value initialization
    };

    template<std::size_t N>
    class my_array<bool, N>::proxy {
    private:
        friend my_array<bool, N>;
        uint8_t &data;
        uint8_t ind;

        proxy(uint8_t &data, uint8_t ind)
                : data(data), ind(ind) {}
    public:
        proxy &operator=(const proxy &other) {
            *this = (bool) other;
            return *this;
        }

        proxy &operator=(bool val) {
            data &= ~(1u << ind);
            data |= ((uint8_t) val) << ind;
            return *this;
        }

        operator bool() const {
            return (data >> ind) & 1u;
        }
    };

    template<std::size_t N>
    typename my_array<bool, N>::proxy
    my_array<bool, N>::at(std::size_t index) {
        if (index >= N)
            throw std::out_of_range("Array index out of range.");
        return proxy(data[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::at(std::size_t index) const {
        if (index >= N)
            throw std::out_of_range("Array index out of range.");
        return (data[index / 8] >> (index % 8)) & 1u;
    }

    template<std::size_t N>
    typename my_array<bool, N>::proxy
    my_array<bool, N>::operator[](std::size_t index) {
        return proxy(data[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::operator[](std::size_t index) const {
        return (data[index / 8] >> (index % 8)) & 1u;
    }
}