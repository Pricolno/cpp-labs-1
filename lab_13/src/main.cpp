#include <cassert>

#include "my_array.h"

using lab_13::my_array;

int main() {
    my_array<bool, 11> arr;
    arr[10] = true;
    assert(arr[10]);
    assert(arr.size() == 11);
    assert(!arr.empty());

    return 0;
}
