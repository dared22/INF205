#include <iostream>

int main() {
    // Part 1
    std::cout << "Size of different data types:" << std::endl;
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "Size of char: " << sizeof(char) << " bytes" << std::endl;
    std::cout << "Size of float: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "Size of long long: " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "Size of long double: " << sizeof(long double) << " bytes" << std::endl;
    std::cout << "Size of short: " << sizeof(short) << " bytes" << std::endl;

    // Part 2
    std::cout << "\n \n Verification using int:" << std::endl;

    int arr[2];
    int* p1 = &arr[0];
    int* p2 = p1 + 1; // Move the pointer to the next int

    char* c1 = reinterpret_cast<char*>(p1);
    char* c2 = reinterpret_cast<char*>(p2);

    std::ptrdiff_t byte_difference = c2 - c1; // Calculate the byte difference

    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "Byte difference between p1 and p2: " << byte_difference << " bytes" << std::endl;
    if (byte_difference == sizeof(int))
        std::cout << "Result: sizeof(int) matches pointer increment distance.\n";
}