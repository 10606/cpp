#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <set>
#include <vector>
#include <cstring>
#include "status.h"

const size_t size_char = 8;
const size_t max_char = 256;

struct bin_tree
{
    int32_t parent, left, right;
    bin_tree(int32_t _parent = -1, int32_t _left = -1, int32_t _right = -1):
        parent(_parent),
        left(_left),
        right(_right)
    {}
};

std::vector <bin_tree> tree; // -1 has not parent
std::vector <size_t> length(max_char); 
std::vector <size_t> freq;

bool get_bit(char  * from, size_t & pos, size_t & ind_bit)
{
    bool ans = (from[pos] & (1 << ind_bit)) != 0;
    pos += ((ind_bit + 1) / size_char);
    ind_bit = (ind_bit + 1) % size_char; 
    return ans;
}

void set_bit(bool bit,  char  * to, size_t & pos, size_t & ind_bit)
{
    to[pos] &= ((max_char - 1) ^ (1 << ind_bit));
    to[pos] |= (bit << ind_bit);
    pos += ((ind_bit + 1) / size_char);
    ind_bit = (ind_bit + 1) % size_char;
}

void write(size_t c, int32_t child,  char  * to, size_t & pos, size_t & ind_bit)
{
    if (tree[c].parent != -1)
        write(tree[c].parent, c, to, pos, ind_bit);
    set_bit((tree[c].right == child), to, pos, ind_bit);
}

void get_lengths(size_t pos, size_t lenghts)
{
    if (tree[pos].left == -1)
    {
        length[pos] = lenghts;
        return;
    }
    get_lengths(tree[pos].left, lenghts + 1);
    get_lengths(tree[pos].right, lenghts + 1);
}

void print_tree (size_t v,  char  * to, size_t & pos_char, size_t & pos, size_t & ind_pos)
{
    if (tree[v].left == -1)
    {
        to[pos_char++] = static_cast < char >(v);
        set_bit(0, to, pos, ind_pos);
        return;
    }
    set_bit(1, to, pos, ind_pos);
    print_tree(tree[v].left, to, pos_char, pos, ind_pos);
    set_bit(1, to, pos, ind_pos);
    print_tree(tree[v].right, to, pos_char, pos, ind_pos);
    set_bit(0, to, pos, ind_pos);
}

status build_tree(char * in)
{
    tree.clear();
    tree.push_back(bin_tree(-1, -1, -1));
    size_t pos = max_char;
    size_t ind_bit  = 0;
    size_t v = 0;
    size_t c_pos = 0;
    for (size_t i = 0; i != 1020; ++i)
    {
        bool to = get_bit(in, pos, ind_bit);
        if (to)
        {
            if (tree[v].left == -1)
                tree[v].left = tree.size();
            else
                tree[v].right = tree.size();
            tree.push_back(bin_tree(v));
            v = tree.size() - 1;
        }
        else
        {
            if (tree[v].left == -1)
                tree[v].right = 0xff & static_cast <int32_t> (in[c_pos++]);
            if (tree[v].parent == -1)
            {
                throw;
            }
            v = tree[v].parent;
        }
    }
    status ans;
    ans.ans = 0;
    ans.size = 0;
    ans.pos = 0;
    return ans;
}

status build_tree(std::vector <size_t> freq_)
{
    freq = freq_;
    tree.clear();
    tree.resize(max_char);
    std::set <std::pair <size_t, size_t> > char_collect; //freq index
    for (size_t i = 0; i != max_char; ++i)
    {
        char_collect.insert(std::make_pair(freq[i], i));
    }
    for (size_t i = 0; i != max_char - 1; ++i)
    {
        std::pair <size_t, size_t> c1 = *char_collect.begin();
        char_collect.erase(char_collect.begin());
        std::pair <size_t, size_t> c2 = *char_collect.begin();
        char_collect.erase(char_collect.begin());
        tree[c1.second].parent = tree.size();
        tree[c2.second].parent = tree.size();
        char_collect.insert(std::make_pair(c1.first + c2.first, tree.size()));
        tree.push_back(bin_tree(-1, c1.second, c2.second));
    }
    size_t pos, ind_pos;
    get_lengths(tree.size() - 1, 0);
    size_t out_size = max_char + 128;
    char * out = static_cast <char *> (operator new (out_size));
    memset(out, 0, out_size);
    pos = max_char;
    size_t pos_c = 0;
    ind_pos = 0;
    print_tree(tree.size() - 1, out, pos_c, pos, ind_pos);
    status ans;
    ans.pos = 0;
    ans.ans = out;
    ans.size = out_size;
    return ans;
}

status encode(status a)
{
    size_t pos = 0, ind_pos = a.pos_rem_e;
    char * out = a.out;
    size_t size = a.size;
    char * in = a.ans;

    out[0] = a.rem_e;
    for (size_t i = 0; i != size; ++i)
    {
        write(tree[0xff & (static_cast <size_t>(in[i]))].parent, 
                    0xff & (static_cast <size_t>(in[i])), 
                    out, pos, ind_pos);
    }
    status ans;
    ans.ans = out;
    ans.size = pos;
    ans.pos = 0;
    ans.pos_rem_e = ind_pos;
    ans.rem_e = out[pos];
    return ans;
}

status decode (status a)
{
    size_t size = a.size;
    char * in = a.ans;
    size_t ind_bit = 0;
    size_t v = 0;
    size_t pos = 0;
    v = a.pos;
    ind_bit = 0;
    size_t j = 0;
    char * out = a.out;
    size_t tail = a.pos_rem_d;
    size_t lenn = (size << 3) + tail;
    for (size_t i = 0; i != lenn; ++i)
    {
        bool to = get_bit(in, pos, ind_bit);
        if (to)
            v = tree[v].right;
        else
            v = tree[v].left;
        if (tree[v].left == -1)
        {
            out[j] = (static_cast <char> (tree[v].right));
            v = 0;
            j++;
        }
    }
    status ans;
    ans.ans = out;
    ans.size = j;
    ans.pos = v;
    ans.pos_rem_d = 0;
    ans.rem_d = 0;
    return ans;
}
