#include <iostream>
#include <string>
#include "bindf.h"

void f(std::string a, std::string b, std::string c, std::string d, std::string && e) {
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

void no_changer(int & a, int & b, int && c)
{
    std::cout << a << b << c << "\n";
}

void r()
{}

struct TT
{
    int a;
    TT () = delete;
    TT (TT && _a)
    {
        a = _a.a;
        _a.a = -1;
        std::cout << a << " move\n";
    }
    TT (TT const & _a)
    {
        a = _a.a;
        std::cout << a << " copy\n";
    }
    TT(int _a)
    {
        a = _a;
    }
};

void tt(TT & a, TT & b, TT & c, TT && d)
{
    std::cout << a.a << b.a << c.a << d.a << std::endl;
    ++a.a;
    ++b.a;
    ++c.a;
    ++d.a;
}

void tt0(TT & a, TT & b, TT & c, TT & d, TT & e)
{
    std::cout << a.a << b.a << c.a << d.a << e.a << std::endl;
    ++a.a;
    ++b.a;
    ++c.a;
    ++d.a;
    ++e.a;
}

void tt1(TT const & a, TT & b, TT c, TT && d)
{
    std::cout << a.a << b.a << c.a << d.a << std::endl;
}

int main()
{
    std::string s = "s";
    std::string a = "a";
    std::string b = "b";

    auto ww = call_once_bind(&f, "1", "2", placeholder<1>(), "4", s);
    ww("3");
    auto vg = bind(&g, _1);
    auto lg = bind(&g, s);
    
    vg(s); //s
    vg(s); //S

    lg();  //s
    lg();  //S
    
    //auto w = bind(&f, "1", placeholder<2>(), bind(&concat, std::string("3 "), _1), _3, s);
    auto w = call_once_bind(&f, "1", placeholder<2>(), bind(&concat, std::string("3 "), _1), _3, s);
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

    TT t0 = TT(1);
    TT t1 = TT(2);
    auto ttt = bind(&tt, t0,  _1, TT(3), _2);
    ttt(t1, TT(4));
    ttt(t1, TT(4));

    auto ttt0 = bind(&tt0, t0,  _1, TT(3), _2, _2);
    ttt0(t1, TT(4));
    ttt0(t1, TT(4));

    auto ttt1 = bind(&tt1, t0,  _1, TT(3), _2);
    ttt1(t1, TT(4));
    ttt1(t1, TT(4));

    auto concat0123 = bind(&concat, "0", "123");
    auto concat10123 = bind(&concat, "1", std::move(concat0123));
    std::cout << concat10123() << std::endl;

    auto concatss = bind(&concat, s, _1);
    auto concat1ss = bind(&concat, "1", concatss);
    std::cout << concat1ss(s) << std::endl;

    return 0;
}
