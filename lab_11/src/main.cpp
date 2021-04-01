#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>

#include "my_vector.h"

template<typename T>
class has_equality_operator {
private:
    typedef char yes_type[1];
    typedef char no_type[2];

    template<typename C>
    static yes_type &test(decltype(&C::operator==));

    template<typename C>
    static no_type &test(...);

public:
    enum {
        value = sizeof(test<T>(0)) == sizeof(yes_type)
    };
};

template<typename T>
constexpr bool has_equality_operator_v = has_equality_operator<T>::value;

namespace test {
    template<typename T>
    void test_my_vector_default_constructor() {
        cool::my_vector<T> vec;
        assert(vec.size() == 0);
        assert(vec.capacity() == 0);
        assert(vec.empty());
    }

    template<typename T>
    void test_my_vector_push_back(const std::vector<T> &defaults, bool copy) {
        cool::my_vector<T> vec;

        std::size_t expected_sz = 0;
        std::size_t expected_cp = 0;
        for (std::size_t i = 0; i < defaults.size(); ++i) {
            if (expected_sz == expected_cp)
                expected_cp = std::max<std::size_t>(1, 2 * expected_cp);
            ++expected_sz;

            if (copy) {
                if constexpr (std::is_copy_constructible_v<T>)
                    vec.push_back(defaults[i]);
            } else {
                if constexpr (std::is_move_constructible_v<T>)
                    vec.push_back(std::move(defaults[i]));
            }

            if constexpr (has_equality_operator_v<T>)
                assert(vec[i] == defaults[i]);
            assert(vec.size() == expected_sz);
            assert(vec.capacity() == expected_cp);
        }
    }

    template<typename T>
    void test_my_vector_reserve_with_args(std::size_t cp) {
        cool::my_vector<T> vec;
        vec.reserve(cp);

        std::size_t expected_cp = 1;
        while (expected_cp < cp)
            expected_cp *= 2;

        assert(vec.capacity() == expected_cp);
        assert(vec.empty());
    }

    template<typename T>
    void test_my_vector_reserve() {
        test_my_vector_reserve_with_args<T>(7u);
        test_my_vector_reserve_with_args<T>(16u);
        test_my_vector_reserve_with_args<T>(0u);
    }

    template<typename T>
    void test_my_vector_resize() {
        cool::my_vector<T> vec;

        std::size_t sz = 10u, cp = 16u;
        vec.resize(sz);

        assert(vec.size() == sz);
        assert(vec.capacity() == cp);
    }

    template<typename T>
    void test_my_vector_push_back_copy(const std::vector<T> &defaults) {
        test_my_vector_push_back(defaults, true);
    }

    template<typename T>
    void test_my_vector_push_back_move(const std::vector<T> &defaults) {
        test_my_vector_push_back(defaults, false);
    }

    template<typename T>
    cool::my_vector<T> construct_my_vector(const std::vector<T> &defaults) {
        cool::my_vector<T> vec;
        for (const auto &d : defaults)
            vec.push_back(d);
        return vec;
    }

    template<typename T>
    void test_my_vector_pop_back(const std::vector<T> &defaults) {
        cool::my_vector<T> vec = construct_my_vector(defaults);
        std::size_t sz = vec.size();
        std::size_t cp = vec.capacity();

        vec.pop_back();
        assert(vec.size() == sz - 1);
        assert(vec.capacity() == cp);
    }

    template<typename T>
    void test_my_vector_clear(const std::vector<T> &defaults) {
        cool::my_vector<T> vec = construct_my_vector(defaults);
        std::size_t cp = vec.capacity();

        vec.clear();
        assert(vec.empty());
        assert(vec.capacity() == cp);
    }

    template<typename T>
    void test_my_vector_element_default_constructor() {
        std::size_t sz = 10u, cp = 16u;
        cool::my_vector<T> vec(sz);

        assert(vec.size() == sz);
        assert(vec.capacity() == cp);
    }

    template<typename T>
    void test_my_vector_indexing(const std::vector<T> &defaults) {
        cool::my_vector<T> vec = construct_my_vector(defaults);

        for (std::size_t i = 0; i < vec.size(); ++i)
            assert(vec[i] == defaults[i]);
    }

    template<typename T>
    void test_my_vector_copy_assignment(const std::vector<T> &defaults) {
        cool::my_vector<T> vec = construct_my_vector(defaults);
        cool::my_vector<T> copy = vec;

        for (std::size_t i = 0; i < vec.size(); ++i)
            assert(vec[i] == copy[i]);
    }

    template<typename T>
    void test_my_vector_move_assignment(const std::vector<T> &defaults) {
        cool::my_vector<T> vec = construct_my_vector(defaults);
        cool::my_vector<T> copy = vec;
        cool::my_vector<T> moved = std::move(copy);

        for (std::size_t i = 0; i < vec.size(); ++i)
            assert(moved[i] == defaults[i]);
    }

    template<typename T>
    void test_my_vector_copy_constructible(const std::vector<T> &defaults) {
        test_my_vector_push_back_copy<T>(defaults);
    }

    template<typename T>
    void test_my_vector_move_constructible(const std::vector<T> &defaults) {
        test_my_vector_push_back_move<T>(defaults);
    }

    template<typename T>
    void test_my_vector_default_constructible() {
        test_my_vector_resize<T>();
        test_my_vector_element_default_constructor<T>();
    }

    template<typename T>
    void test_my_vector_has_equality_operator(const std::vector<T> &defaults) {
        test_my_vector_indexing<T>(defaults);
        test_my_vector_copy_assignment<T>(defaults);
        test_my_vector_move_assignment<T>(defaults);
    }

    template<typename T, typename ...Args,
            typename = std::enable_if<std::is_copy_constructible_v<T>>>
    void test_my_vector(Args ...defaults) {
        std::vector<T> defaults_ = {defaults...};

        test_my_vector_default_constructor<T>();
        test_my_vector_reserve<T>();
        test_my_vector_pop_back<T>(defaults_);
        test_my_vector_clear<T>(defaults_);

        test_my_vector_copy_constructible<T>(defaults_);
        if constexpr (std::is_default_constructible_v<T>)
            test_my_vector_default_constructible<T>();
        if constexpr (std::is_move_constructible_v<T>)
            test_my_vector_move_constructible<T>(defaults_);
        if constexpr (has_equality_operator_v<T>)
            test_my_vector_has_equality_operator(defaults_);
    }
}

namespace product {

    class Product {
    public:
        Product(const char *name, int quantity, double price)
                : name_(strcpy(new char[strlen(name) + 1], name)),
                  quantity_(quantity), price_(price) {}

        Product(const Product &other)
                : Product(other.name_, other.quantity_, other.price_) {}

        Product(Product &&other) = delete;

        ~Product() {
            delete[] name_;
        }

        friend std::ostream &operator<<(std::ostream &out, const Product &product) {
            out << product.name_ << " " << product.quantity_ << " " << product.price_;
            return out;
        }

    private:
        char *name_;  // Указатель владеет C-style строкой, на которую указывает.
        int quantity_;
        double price_;
    };

    void print(const cool::my_vector<Product> &v) {
        std::cout << v << std::endl;
    }

}  // namespace product

int main() {
    test::test_my_vector<int>(5, 10, 1, 4, 5, 2, 0, 129, 4);
    test::test_my_vector<product::Product>(
            product::Product("asdf", 4, 12.0),
            product::Product("qwe", -1, 7.5)
    );

    return 0;
}
