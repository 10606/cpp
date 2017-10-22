#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "big_integer.h"
#include "big_integer.cpp"

using namespace std;
int main()
{
    /*
        big_integer 1545494452991
        big_integer 1545494452991
        big_integer -747212393
        big_integer -745471297
    */

    string s = "1545494452991";
    string ss = "-745471297";
    //string s(91, '0');
    //s = "1" + s;
    //string ss(38, '0');
    //ss = "1" + ss;
    //2147483648 * 1071432244
    big_integer c = big_integer(s) * big_integer(ss);
    big_integer a = big_integer(s) * big_integer(ss) / big_integer(s);
    cout << c << std::endl << big_integer(s) << std::endl << a << std::endl;
    big_integer q = -7;
    big_integer w = q >> 1;
    std::cout << w << std::endl;;
}
