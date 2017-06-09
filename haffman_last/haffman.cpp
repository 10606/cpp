#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include "status.h"
extern status encode (status);
extern status decode (status);
extern tree_e build_tree (std::vector <size_t>);
extern void build_tree (char *, size_t);
const size_t size_block = 10000;
char input[(size_block + 2) * 256];
char output[(size_block + 2) * 256];
int decode_file (int argc, char ** argv)
{
    std::ifstream in(argv[2], std::ios::binary);
    if (!in.is_open())
    {
        std::cout << " i cant open input file " << std::endl;
        return 1;
    }
    std::ofstream out(argv[3], std::ios::binary);
    if (!out.is_open())
    {
        std::cout << " i cant open output file " << std::endl;
        return 1;
    }
    bool flag = 1;
    char size [5]; 
    in.read(input, 128 + 256);
    size_t cread = in.gcount();
    status a;
    a.ans = input;
    a.size = cread;
    a.pos = 0;
    build_tree(input, cread);
    
    while (flag)
    {
        in.read(size, 5);
        cread = in.gcount();
        if (cread != 5)
            break;
        cread = 0;
        for (size_t i = 0; i != 4; ++i)
            cread |= (0xff & static_cast <size_t> (size[i])) << (i << 3);
        size_t pos_rem = 0xff & static_cast <size_t> (size[4]);
        char rem = 0;
        if (pos_rem != 0)
        {
            in.read(input, cread + 1);
            cread = in.gcount() - 1;
        }
        else
        {
            in.read(input, cread);
            cread = in.gcount();
        }
        a.ans = input;
        a.size = cread;
        a.rem_d = rem;
        a.pos_rem_d = pos_rem;
        a.out = output;
        a = decode(a);
        out.write(a.ans, a.size);
    }
    return 0;
}

int encode_file (int argc, char ** argv)
{
    std::ofstream out(argv[3], std::ios::binary);
    if (!out.is_open())
    {
        std::cout << " i cant open output file " << std::endl;
        return 1;
    }
    bool flag = 1;
    std::ifstream in_freq(argv[2], std::ios::binary);
    if (!in_freq.is_open())
    {
        std::cout << " i cant open input file " << std::endl;
        return 1;
    }
    std::vector <size_t> freq(256, 0);
    while (flag)
    {
        in_freq.read(input, size_block);
        size_t cread = in_freq.gcount();
        if (cread == 0)
            break;
        for (size_t i = 0; i != cread; ++i)
            freq[0xff & static_cast <size_t> (input[i])]++;
    }
    status a;
    try
    {
        tree_e tree(build_tree(freq));
        out.write(tree.tree, tree.size);
        //operator delete (a.ans);
    }
    catch(...)
    {
        return 1;
    }
    std::ifstream in(argv[2], std::ios::binary);
    if (!in.is_open())
    {
        std::cout << " i cant open input file " << std::endl;
        return 1;
    }
    a.pos = 0;
    while (flag)
    {
        in.read(input, size_block);
        size_t cread = in.gcount();
        if (cread == 0)
            break;
        a.ans = input;
        a.size = cread;
        a.out = output;
        a = encode(a);
        for (size_t i = 0; i != 4; ++i)
            out << static_cast < char>(a.size >> (i << 3));
        in.peek();
        if (in.good())
        {
            out << static_cast <char> (0);
            out.write(a.ans, a.size);
        }
        else
        {
            out << static_cast <char> (a.get_pos_rem_e() & 0xff);
            out.write(a.ans, a.size);
            if (a.get_pos_rem_e() != 0)
                out << a.get_rem_e();
        }
    }
    return 0;
}

int main (int argc, char ** argv)
{
    const char * encode0 = "encode";
    const char * encode1 = "e";
    const char * decode0 = "decode";
    const char * decode1 = "d";
    if (argc < 4)
    {
        printf("use: <encode/e/decode/d> <input_file_name> <output_file_name>");
        return 1;
    }
    if (strcmp(argv[1], encode0) == 0 || strcmp(argv[1], encode1) == 0)
        /*return*/ encode_file(argc, argv);
    else if (strcmp(argv[1], decode0) == 0 || strcmp(argv[1], decode1) == 0)
        /*return*/ decode_file(argc, argv);
    else 
    {
        printf("use: <encode/e/decode/d> <input_file_name> <output_file_name>");
        return 1;
    }
    return 0;
}
