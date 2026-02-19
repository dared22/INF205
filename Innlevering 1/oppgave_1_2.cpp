#include <iostream>
using namespace std;

int main() {
    int char_int = -12;
    short char_short = -12;
    long char_long = -12;
    long long char_long_long = -12;
    cout << "int (" << sizeof(char_int) << " bytes): ";
    for (int j = 0; j < sizeof(char_int); j++) {
        cout << "0x" << hex << (int)*((unsigned char*)&char_int + j) << " ";
    }
    cout << endl;
    cout << "short (" << sizeof(char_short) << " bytes): ";
    for (int j = 0; j < sizeof(char_short); j++) {
        cout << "0x" << hex << (int)*((unsigned char*)&char_short + j) << " ";
    }
    cout << endl;
    cout << "long (" << sizeof(char_long) << " bytes): ";
    for (int j = 0; j < sizeof(char_long); j++) {
        cout << "0x" << hex << (int)*((unsigned char*)&char_long + j) << " ";
    }
    cout << endl;
    cout << "long long (" << sizeof(char_long_long) << " bytes): ";
    for (int j = 0; j < sizeof(char_long_long); j++) {
        cout << "0x" << hex << (int)*((unsigned char*)&char_long_long + j) << " ";
    }
    cout << endl;
    return 0; 
}