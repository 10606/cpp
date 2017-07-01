#include <iostream>
#include "deque.h"
using namespace std;


int main()
{
    deque <int> a;
    a.push_back(1);
    cout << a << endl;
    a.push_back(2);
    cout << a << endl;
    a.push_back(3);
    cout << a << endl;
    a.push_back(4);
    cout << a << endl;
    a.insert((a.end() ), 5);
    cout << a << endl;
    a.insert((a.begin() + 3), 6);
    cout << a << " erase " << endl;
    a.erase(a.begin() + 0);
    cout << a << endl;
    a.pop_front();
    cout << a << endl;
    a.pop_front();
    cout << a << endl;
    a.pop_front();
    cout << a << endl;
    a.pop_front();
    cout << a << endl;
    cout << a[0] << endl;
    typename deque<int>::reverse_iterator q = a.rbegin();
    typename deque<int>::iterator w = a.begin();
    cout << *q << "\n";
    a.clear();
    for (size_t i = 0; i != 100; ++i)
        a.push_front(i);
    for (size_t i = 0; i != 100; ++i)
    {
        if (a[99 - i] != i)
            cout << "error " << a[99 - i] << " " << i << endl;
    }
    deque<int> ss(a);
    ss.pop_front();
    a = ss;
    for (size_t i = 0; i != 99; ++i)
    {
        if (a[98 - i] != i)
            cout << "error " << a[98 - i] << " " << i << endl;
    }
    typename deque<int>::iterator tmp = a.insert(a.begin() + 10, 10392394823907423);
    a.erase(tmp);
    //a.erase(a.begin() + 20);
    cout << tmp - a.begin() << endl;
    cout << (tmp == (a.begin() + 19)) << endl;
    q = a.rbegin();
    w = a.begin();
    for (size_t i = 0; i != a.size(); ++i)
        cout << (*q++) << " " << (*w++) << "\n";
    deque<int> r;
    r.insert(r.begin(), 1);
    cout << r[0] << endl;

    deque <int> o ;
    o.push_back(1);
    o.push_back(2);
    o.push_back(3);
    o.push_back(4);
    o.push_back(5);
    o.push_back(6);
    o.push_back(7);
    o.push_back(8);
    o.push_back(9);
    o.push_back(0);
    o.pop_back();
    o.pop_back();
    o.pop_front();
    o.pop_back();
    o.pop_front();
    o.pop_back();
    o.pop_front();
    o.pop_front();
    o.pop_back();
    o.pop_front();
    o.push_front(1);
    for (int i = 0; i != 10; ++i)
    {
        o.insert(o.begin() + 1, i);
    }
    cout << o;
}
