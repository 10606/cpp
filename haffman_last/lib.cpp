#include <cstdio>
#include <memory>
#include <cstdlib>
#include <stdint.h>
#include <set>
#include <vector>
#include <cstring>
#include "status.h"
#include "lib.h"

//+char -> size_t static_cast<size_t> (static_cast <unsigden char> (c))
//+get status by &
//+-global variable -> status (work in thred)
//+tree_e -> vector<char> for write use .data
//+build_tree -> class with 2 constructor and methods
//  get data
//  encode
//  decode
//+in build_tree (char *) check size

bool haffman::get_bit(char * const & from, size_t & pos, size_t & ind_bit)
{
    bool ans = (from[pos] & (1 << ind_bit)) != 0;
    pos += ((ind_bit + 1) / size_char);
    ind_bit = (ind_bit + 1) % size_char; 
    return ans;
}

void haffman::set_bit(bool const & bit, char * const & to, size_t & pos, size_t & ind_bit)
{
    to[pos] &= ((max_char - 1) ^ (1 << ind_bit));
    to[pos] |= (bit << ind_bit);
    pos += ((ind_bit + 1) / size_char);
    ind_bit = (ind_bit + 1) % size_char;
}

void haffman::write(size_t const & c, int32_t const & child, char * const & to, size_t & pos, size_t & ind_bit)
{
    if (tree[c].parent != -1)
        write(tree[c].parent, c, to, pos, ind_bit);
    set_bit((tree[c].right == child), to, pos, ind_bit);
}

void haffman::calc_freq(std::vector <size_t> & freq, char * input, size_t len)
{
    for (size_t i = 0; i != len; ++i)
        freq[static_cast <size_t> (static_cast <unsigned char> (input[i]))]++;
}

void haffman::get_lengths(size_t pos, size_t lenghts)
{
    if (tree[pos].left == -1)
    {
        length[pos] = lenghts;
        return;
    }
    get_lengths(tree[pos].left, lenghts + 1);
    get_lengths(tree[pos].right, lenghts + 1);
}

void haffman::print_tree (size_t v, char * to, size_t & pos_char, size_t & pos, size_t & ind_pos)
{
    if (tree[v].left == -1)
    {
        to[pos_char++] = static_cast <char>(v);
        set_bit(0, to, pos, ind_pos);
        return;
    }
    set_bit(1, to, pos, ind_pos);
    print_tree(tree[v].left, to, pos_char, pos, ind_pos);
    set_bit(1, to, pos, ind_pos);
    print_tree(tree[v].right, to, pos_char, pos, ind_pos);
    set_bit(0, to, pos, ind_pos);
}

haffman::haffman (char * in, size_t size)
{
    length.resize(max_char);
    correct_index.resize(max_char);
    if (size < 128 + 256)
        throw;
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
            {
                size_t tmp = static_cast <int32_t> (static_cast <unsigned char> (in[c_pos++]));
                tree[v].right = tmp;
                correct_index[tmp] = v;
            }
            if (tree[v].parent == -1)
                throw;
            v = tree[v].parent;
        }
    }
    v_start = 0;
}

haffman::haffman (std::vector <size_t> freq_)
{
    length.resize(max_char);
    correct_index.resize(max_char);
    if (freq_.size() != max_char)
        throw;
    freq = freq_;
    tree.clear();
    tree.resize(max_char);
    std::set <std::pair <size_t, size_t> > char_collect; //freq index
    for (size_t i = 0; i != max_char; ++i)
    {
        correct_index[i] = i;
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
    v_start = tree.size() - 1;
}

std::vector <char> haffman::get_tree()
{
    size_t pos, ind_pos;
    get_lengths(v_start, 0);
    size_t out_size = max_char + 128;
    std::vector <char> out(out_size);
    pos = max_char;
    size_t pos_c = 0;
    ind_pos = 0;
    print_tree(v_start, out.data(), pos_c, pos, ind_pos);
    return out;
}

void haffman::encode(status & a)
{
    size_t pos = 0, ind_pos = a.pos_rem_e;
    char * out = a.out;
    size_t size = a.size;
    char * in = a.ans;
    out[0] = a.rem_e;
    for (size_t i = 0; i != size; ++i)
    {
        size_t tmp = correct_index[(static_cast <size_t>(static_cast <unsigned char> (in[i])))];
        write(tree[tmp].parent, tmp, out, pos, ind_pos);
    }
    a.ans = out;
    a.size = pos;
    a.pos = 0;
    a.pos_rem_e = ind_pos;
    a.rem_e = out[pos];
}

void haffman::decode (status & a)
{
    size_t size = a.size;
    char * in = a.ans;
    size_t ind_bit = 0;
    size_t v = v_start;
    size_t pos = 0;
    v = a.pos;
    ind_bit = 0;
    size_t j = 0;
    char * out = a.out;
    size_t tail = a.pos_rem_d;
    size_t lenn = (size << 3) + tail;
    for (size_t i = 0; i != lenn; ++i)
    {
        if (get_bit(in, pos, ind_bit))
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
    a.ans = out;
    a.size = j;
    a.pos = v;
    a.pos_rem_d = 0;
    a.rem_d = 0;
}
