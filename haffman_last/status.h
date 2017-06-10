#ifndef STATUS_H
#define STATUS_H
#include <memory>
struct haffman;
struct status
{
    size_t pos;
    size_t size;
    char * ans;
    char * out;
    char rem_d;
    size_t pos_rem_d;
private:
    char rem_e;
    size_t pos_rem_e;
    
public:
    char get_rem_e()
    {
        return rem_e;
    }
    size_t get_pos_rem_e()
    {
        return pos_rem_e;
    }
    status ():
        pos(0),
        size(0),
        ans(0),
        out(0),
        pos_rem_d(0),
        pos_rem_e(0)
    {}

    friend haffman;
};
#endif
