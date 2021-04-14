#include <cassert>

#include "my_array.h"

using lab_13::my_array;

int main() {
    my_array<bool, 5> arr;
    arr[3] = true;
    assert(arr[3]);
    assert(arr.size() == 5);
    assert(!arr.empty());

    return 0;
}
