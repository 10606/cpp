#include "deque.h"
#include <iostream>
int main()
{
    deque <int> a;
    a.push_back(1);
    deque <int> :: const_reverse_iterator b = a.rbegin();
    deque <int> :: iterator c = a.begin();
    deque <int> :: const_iterator d = a.begin();
    std::cout << (c == d);
    std::cout << (d == c);
    std::cout << (*b);

}
