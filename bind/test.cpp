#include <iostream>
#include <string>
#include "bindf.h"

void f(std::string && a, std::string b, std::string c, std::string d, std::string const & e) {
    std::cout << a << " " << b << " " << c << " " << d << " " << e << "\n";
    d = "D";
    a = "A";
    b = "B";
    c = "C";
}

std::string concat(std::string x, std::string y) {
    return "(" + x + y + ")";
}

void g(std::string & s) {
    std::cout << s << "\n";
    s = "S";
}

void changer(int & a, int & b, int & c)
{
    std::cout << a << b << c << "\n";
    ++a;
    ++b;
    ++c;
}

void no_changer(int && a, int && b, int && c)
{
    std::cout << a << b << c << "\n";
}

void r()
{}

int main()
{
    std::string s = "s";
    std::string a = "a";
    std::string b = "b";
    auto ww = call_once_bind(f, "1", "2", placeholder<1>(), "4", s);
    ww("3");
    auto vg = bind(&g, _1);
    auto lg = bind(&g, s);
    
    vg(s); //s
    vg(s); //S

    lg();  //s
    lg();  //S
    
    auto w = call_once_bind(&f, "1", placeholder<2>(), bind(concat, std::string("3 "), _1), _3, s);
    w(std::move(a), std::move(b), s);
    std::cout << a << "!" << b << "!" << s <<  "\n";


    int a0 = 0, b0 = 1, c0 = 2;
    auto tmp = bind(&changer, _1, _1, _2);
    auto tmp_no = bind(&no_changer, _1, _1, _2);
    tmp(a0, b0);
    std::cout << a0 << b0 << c0 << "\n";
    tmp_no(2, 2);

    auto cr  = bind(&r);
    cr();

    return 0;
}
