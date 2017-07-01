//
// main.cpp
// deque
//
// Created by Vadim on 01.07.17.
// Copyright :copyright: 2017 Vadim. All rights reserved.
//

#include <iostream>
#include "deque.h"
using namespace std;

void sashas_test()
{
deque<int> q;
for (int i = 0; i < 10; ++i)
q.push_back(i);
for (int i = 0; i < 10; ++i)
q.pop_back();
q.push_back(1);
for (int i = 10; i < 20; ++i) {
q.insert(q.begin() + 1, i);
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n' << '\n';
}
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n' << '\n';
}

int main(int argc, const char * argv[]) {
sashas_test();
deque<int> q;
for (int i = 0; i < 20; ++i)
q.push_back(i);
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n';
for (deque<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it)
cout << *it << ' ';
q.insert(q.begin() + 3, 100000);
q.insert(q.begin() + 17, 99999);
cout << '\n' << '\n';
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n';
for (deque<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it)
cout << *it << ' ';
q.erase(q.begin());
cout << '\n' << '\n';
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n';
for (deque<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it)
cout << *it << ' ';
//q.erase(q.begin() + 16);
//q.erase(q.begin() + 2);
q.erase(q.begin() + 10);
cout << '\n' << '\n';
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n';
for (deque<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it)
cout << *it << ' ';
cout << *(q.begin() + 15) << endl;
q.erase(q.begin() + 15);
cout << '\n' << '\n';
for (deque<int>::iterator it = q.begin(); it != q.end(); ++it)
cout << *it << ' ';
cout << '\n';
for (deque<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it)
cout << *it << ' ';

deque <int> l;
typename deque <int> :: iterator a = l.begin();
typename deque <int> :: iterator b = l.end();
cout << endl << (a == b);
return 0;
}
 

