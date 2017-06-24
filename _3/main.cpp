#include "big_integer.h"
#include "big_integer.cpp"
#include <vector>
using namespace std;
template <typename T>
void erase_unordered(std::vector<T>& v, typename std::vector<T>::iterator pos)
{
    //cout << " swap " << v.size() << endl;
    std::swap(v.back(), *pos);
    //cout << " swap " << v.size() << endl;
    v.pop_back();
}

template <typename T>
T extract_random_element(std::vector<T>& v)
{
    size_t index = rand() % v.size();
    T copy = v[index];
    erase_unordered(v, v.begin() + index);
    return copy;
}

template <typename T>
void merge_two(std::vector<T>& v)
{
    T a = extract_random_element <big_integer> (v);
    T b = extract_random_element <big_integer> (v);
    T ab = a * b;
    v.push_back(ab);
}

template <typename T>
T merge_all(vector<T> v)
{
    //cout << "merge" << endl;;
    while (v.size() >= 2)
        merge_two(v);
    cout << v[0] << " " << v.size() << endl;
    return v[0];
}
int myrand()
{
    int val = rand() - RAND_MAX / 2;
    if (val != 0)
        return val;
    else
        return 1;
}
int main()

{
    size_t number_of_iterations = 100;;;
    size_t number_of_multipliers = 8;;;
    for (unsigned itn = 0; itn != number_of_iterations; ++itn)
    {
        vector<big_integer> x;
        for (size_t i = 0; i != number_of_multipliers; ++i)
            x.push_back(myrand());

        big_integer a = merge_all(x);
        big_integer b = merge_all(x);

        //EXPECT_TRUE(a == b);
    }

    big_integer a("-100000000000000000000000000");
    big_integer c("100000000000000000000000000"
    "00000000000000000000000000");
    cout << (a * c) << endl;;
    a *= c;
    big_integer b(a);
    cout << a << " " << b << endl;
    a++;
    cout << a << " " << b << endl;
    cout << (a * c) << endl;;
    cout << a << endl;;
    
}
