#include <string>
#include <iostream>
#include <memory>
#include "persistent_set.h"
#include "linked_ptr.h"
#include "shared_ptr.h"
#include <cstdlib>
#include <set>
#include <vector>
#include <assert.h>

struct not_copy
{
    int a, status;
    not_copy() = delete;
    not_copy(not_copy const &) = delete;
    not_copy(not_copy && b):
        a(b.a),
        status(1)
    {
        b.status = 0;
    }
    not_copy(int _a):
        a(_a),
        status(1)
    {}

    bool operator == (not_copy const & b)
    {
        return (status && b.status) && (a == b.a);
    }
    bool operator != (not_copy const & b)
    {
        return (status && b.status) && (a != b.a);
    }
    bool operator < (not_copy const & b)
    {
        return (status && b.status) && (a < b.a);
    }
    bool operator > (not_copy const & b)
    {
        return (status && b.status) && (a > b.a);
    }
};

template < typename Set >
std::vector < std::vector < int > > test(std::string const& msg) {
    int tt = clock();
    srand(1);
    std::vector < Set > ss(256);
    for (int i = 0; i < 1000000; ++i) {
        int r = rand() & 7;
        if (r == 0) {
            ss[rand() & 255] = ss[rand() & 255];
        } else if (r <= 3) {
            ss[rand() & 255].insert(rand() & 16383);
        } else {
            int j = rand() & 255;
            auto it = ss[j].find(rand() & 16383);
            if (it != ss[j].end())
                ss[j].erase(it);
        }
    }
    std::vector < std::vector < int > > a(256);
    for (int i = 0; i < 256; ++i)
        for (int x : ss[i])
            a[i].push_back(x);
    std::cout << msg << " " << 1.*(clock() - tt) / CLOCKS_PER_SEC << "\n";
    return a;
}

std::ostream & operator << (std::ostream & s, not_copy const & a)
{
    s << "(" << a.status << "  " << a.a << ") "; 
    return s;
}

template <typename T>
std::ostream & operator << (std::ostream & s, persistent_set <T, shared_ptr> const & a)
{
    for (typename persistent_set <T, shared_ptr> :: iterator i = a.begin(); i != a.end(); ++i)
    {
        s << (*i) << "  "; 
    }
    return s;
}
template <typename T>
std::ostream & operator << (std::ostream & s, persistent_set <T, linked_ptr> const & a)
{
    for (typename persistent_set <T, linked_ptr> :: iterator i = a.begin(); i != a.end(); ++i)
    {
        s << (*i) << "  "; 
    }
    return s;
}
int main() {
    
    persistent_set <not_copy> tmp;
    tmp.insert(std::move(not_copy(1)));
    persistent_set <not_copy> tmp0;
    tmp0 = tmp;
    tmp0.insert(std::move(not_copy(2)));
    tmp0.insert(std::move(not_copy(3)));
    tmp0.insert(std::move(not_copy(-1)));
    tmp0.insert(std::move(not_copy(2)));
    std::cout << tmp << std::endl;
    std::cout << tmp0 << std::endl;
    tmp0.erase(++++tmp0.begin());
    std::cout << tmp << std::endl;
    std::cout << tmp0 << std::endl;
    

    auto r2 = test<persistent_set<int, linked_ptr>>("linked_ptr");
    auto r1 = test<persistent_set<int, shared_ptr>>("shared_ptr");
    auto r3 = test<std::set<int>>("std::set  ");
    assert(r1 == r2);
    assert(r2 == r3);
    
    
    return 0;
}
