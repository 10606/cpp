#include <cstdio>
#include "list.h"
using namespace std;

ostream & operator << (ostream & s, list_base <int> const & tmp)
{
    for (list_base <int>::const_iterator i = tmp.begin(); i != tmp.end(); ++i)
    {
        cout << *i << endl;
    }
    return s;
}
int main()
{
    cout << " ____ " << endl;
    list <int> kek;
    kek.insert(kek.begin(), 7);
    kek.insert(kek.begin(), 8);
    kek.insert(kek.begin(), 9);
    cout << kek;
    for (list_base <int>::iterator i = kek.begin(); i != kek.end(); ++i)
    {
        cout << (*i) << endl;
    }
    cout << " ____erase " << endl;
    kek.erase(++kek.begin());
    cout << " ____kek " << kek.back() << endl;
    for (list_base <int>::iterator i = kek.begin(); i != kek.end(); ++i)
    {
        cout << (*i) << endl;
    }
    cout << " ____kek- " << kek.front() << endl;
    for (list_base <int>::iterator i = --kek.end(); i != kek.end(); --i)
    {
        cout << (*i) << endl;
    }
    list_base <int> lol;
    cout << " ____splice " << endl;
    lol.splice(lol.begin(), kek, kek.begin(), kek.end());
    lol.insert(lol.end(), 7);
    lol.insert(lol.end(), 8);
    lol.insert(lol.end(), 9);
    cout << " ____splice " << endl;
    lol.splice(lol.begin(), kek, kek.begin(), kek.end());
    cout << " ____kek " << endl;
    for (list_base <int>::iterator i = kek.begin(); i != kek.end(); ++i)
    {
        cout << (*i) << endl;
    }
    cout << " ____kek- " << endl;
    for (list_base <int>::iterator i = --kek.end(); i != kek.end(); --i)
    {
        cout << (*i) << endl;
    }
    cout << " ____lol " << endl;
    for (list_base <int>::iterator i = lol.begin(); i != lol.end(); ++i)
    {
        cout << (*i) << endl;
    }
    list_base <int>::iterator i = lol.begin();
    ++i;
    list_base <int>::iterator j = lol.end();
    cout << " ____erase " << endl;
    lol.erase(i, j);
    cout << " ____lol " << endl;
    for (list_base <int>::iterator i = lol.begin(); i != lol.end(); ++i)
    {
        cout << *i << endl;
    }
    cout << " ____make_tmp_(lol) " << endl;
    list <int> tmp(lol);
    cout << " ____tmp " << endl;
    for (list_base <int>::iterator i = tmp.begin(); i != tmp.end(); ++i)
    {
        cout << *i << endl;
    }
    cout << " ____tmp- " << endl;
    for (list_base <int>::iterator i = --tmp.end(); i != tmp.end(); --i)
    {
        cout << *i << endl;
    }
    cout << " ____tmp=kek " << endl;
    tmp = kek;
    cout << " ____tmp " << endl;
    for (list_base <int>::iterator i = tmp.begin(); i != tmp.end(); ++i)
    {
        cout << *i << " rr " << endl;
    }
    list <int> :: iterator kk = kek.begin();
    list <int> :: iterator qq = kek.end();
    std::cout << (kk == qq) << endl;
    cout << lol.empty() << " " << kek.empty() << endl;
    swap(lol, kek);
    cout << lol.empty() << " " << tmp.empty() << endl;
    swap(lol, tmp);
    cout << tmp.empty() << " " << kek.empty() << endl;
    swap(tmp, kek);
    cout << tmp.empty() << " " << kek.empty() << endl;
}
