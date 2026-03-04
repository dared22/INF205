#include <iostream>
using namespace std;

int main() {
    cout << sizeof(short) << endl; //2
    cout << sizeof(long) << endl; //8
    cout << sizeof(int) << endl; //4
    cout << sizeof(long long) << endl; //8

    short value = 0;
    short* p = &value;
    short* p_incremented = p + 1;
    ptrdiff_t distance = reinterpret_cast<char*>(p_incremented) - reinterpret_cast<char*>(p);

    cout << "Distance in memory for short: " << distance << endl;
    return 0;
}
