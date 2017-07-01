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
    q = a.rbegin();
    w = a.begin();
    for (size_t i = 0; i != a.size(); ++i)
        cout << (*q++) << " " << (*w++) << "\n";
    deque<int> r;
    r.insert(r.begin(), 1);
    cout << r[0];
}
