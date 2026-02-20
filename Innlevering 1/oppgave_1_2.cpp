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

//to get -12 you need to take maximum value of a byte, which is 255. Add 1. and subtract 12. 
//So, 255 + 1 - 12 = 244
//Now since hex is base 16 system divide 244 by 16.
//We get 244 / 16 = 15 with 4 as remainder.
//Hence 15 = f, 4 = 4
//0x is just a prefix to mark that we are using hex.
//Since -12 only need one byte to be represented in hex we use 0xf4 and the rest are just fillers.
//Since the number is negative we fill it with 0xff, if it was positive we would fill the rest with 0x00.
//Also we can see that two hex characters represent one byte, and thus int is 4 bytes, short is 2 bytes, and long and long long is 8 bytes.
