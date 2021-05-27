#ifndef LAB_14_LINQ_HPP
#define LAB_14_LINQ_HPP

#include <iterator>
#include <vector>
#include <optional>

namespace linq {
    namespace impl {
        template<typename T, typename Self>
        class enumerator;

        template<typename T, typename InputIt>
        class from_enumerator : public enumerator<T, from_enumerator<T, InputIt>> {
        public:
            from_enumerator(InputIt first, InputIt last)
                    : current(first), last(last) {}

            // To save one increment at the end
            // I had to write this awful conditional shit
            std::optional<T> next() {
                if (current == last)
                    return std::nullopt;
                if (first_call) {
                    first_call = false;
                    return *current;
                }
                ++current;
                if (current != last)
                    return *current;
                return std::nullopt;
            }

        private:
            bool first_call = true;
            InputIt current, last;
        };

        template<typename T, typename Enum>
        class take_enumerator : public enumerator<T, take_enumerator<T, Enum>> {
        public:
            take_enumerator(std::size_t num, Enum &prev)
                    : num(num), prev(prev) {}

            std::optional<T> next() {
                if (num) {
                    --num;
                    return prev.next();
                }
                return std::nullopt;
            }

        private:
            std::size_t num;
            Enum &prev;
        };

        template<typename T, typename F, typename Enum>
        class select_enumerator : public enumerator<T, select_enumerator<T, F, Enum>> {
        public:
            select_enumerator(F transform, Enum &prev)
                    : transform(std::move(transform)), prev(prev) {}

            std::optional<T> next() {
                if (auto value = prev.next())
                    return transform(*value);
                return std::nullopt;
            }

        private:
            F transform;
            Enum &prev;
        };

        template<typename T, typename F, typename Enum>
        class where_enumerator : public enumerator<T, where_enumerator<T, F, Enum>> {
        public:
            where_enumerator(F predicate, Enum &prev)
                    : predicate(std::move(predicate)), prev(prev) {}

            std::optional<T> next() {
                std::optional<T> value;
                do value = prev.next();
                while (value && !predicate(*value));
                return value;
            }

        private:
            F predicate;
            Enum &prev;
        };

        template<typename T, typename F, typename Enum>
        class until_enumerator : public enumerator<T, until_enumerator<T, F, Enum>> {
        public:
            until_enumerator(F predicate, Enum &prev)
                    : predicate(std::move(predicate)), prev(prev) {}

            std::optional<T> next() {
                if (stopped)
                    return std::nullopt;
                std::optional<T> value = prev.next();
                if (value && predicate(*value)) {
                    stopped = true;
                    return std::nullopt;
                }
                return value;
            }

        private:
            bool stopped = false;
            F predicate;
            Enum &prev;
        };

        template<typename T, typename Enum>
        class drop_enumerator : public enumerator<T, drop_enumerator<T, Enum>> {
        public:
            drop_enumerator(std::size_t num, Enum &prev)
                    : num(num), prev(prev) {}

            std::optional<T> next() {
                while (num)
                    prev.next(), num--;
                return prev.next();
            }

        private:
            std::size_t num;
            Enum &prev;
        };

        template<typename T, typename OutputIt, typename Enum>
        void _copy_to(OutputIt it, Enum &enumerator) {
            while (std::optional<T> value = enumerator.next())
                *it++ = *value;
        }

        template<typename T, typename Enum>
        std::vector<T> _to_vector(Enum &enumerator) {
            std::vector<T> res;
            auto it = std::back_inserter(res);
            _copy_to<T, decltype(it), Enum>(it, enumerator);
            return res;
        }

        template<typename T, typename Self>
        class enumerator {
        public:
            auto take(std::size_t num) &&{
                return take_enumerator<T, Self>(num, (Self &) *this);
            }

            auto drop(std::size_t num) &&{
                return drop_enumerator<T, Self>(num, (Self &) *this);
            }

            template<typename F>
            auto select(F transform) &&{
                using R = decltype(transform(std::declval<T>()));
                return select_enumerator<R, F, Self>(std::move(transform), (Self &) *this);
            }

            template<typename R, typename F>
            auto select(F transform) &&{
                return select_enumerator<R, F, Self>(std::move(transform), (Self &) *this);
            }

            template<typename F>
            auto where(F predicate) &&{
                return where_enumerator<T, F, Self>(std::move(predicate), (Self &) *this);
            }

            auto where_neq(T value) &&{
                auto predicate = [value = std::move(value)](const T &x) {
                    return x != value;
                };
                return std::move(*this).template where<decltype(predicate)>(std::move(predicate));
            }

            template<typename F>
            auto until(F predicate) &&{
                return until_enumerator<T, F, Self>(std::move(predicate), (Self &) *this);
            }

            auto until_eq(T value) &&{
                auto predicate = [value = std::move(value)](const T &x) {
                    return x == value;
                };
                return std::move(*this).template until<decltype(predicate)>(std::move(predicate));
            }

            std::vector<T> to_vector() &&{
                return _to_vector<T, Self>((Self &) *this);
            }

            template<typename OutputIt>
            void copy_to(OutputIt it) &&{
                return _copy_to<T, OutputIt, Self>(it, (Self &) *this);
            }
        };
    }

    template<typename InputIt>
    auto from(InputIt first, InputIt last) {
        using T = typename std::iterator_traits<InputIt>::value_type;
        return impl::from_enumerator<T, InputIt>(first, last);
    }
}

#endif //LAB_14_LINQ_HPP
