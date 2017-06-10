#ifndef LIB_H
#define LIB_H
struct haffman 
{
private:
    struct bin_tree
    {
        int32_t parent, left, right;
        bin_tree(int32_t _parent = -1, int32_t _left = -1, int32_t _right = -1):
            parent(_parent),
            left(_left),
            right(_right)
        {}
    };
    const size_t size_char = 8;
    const size_t max_char = 256;
    std::vector <bin_tree> tree; // -1 has not parent
    std::vector <size_t> length; 
    std::vector <size_t> freq;
    std::vector <size_t> correct_index;
    size_t v_start;
    inline bool get_bit(char * const & from, size_t & pos, size_t & ind_bit);
    inline void set_bit(bool const & bit, char * const & to, size_t & pos, size_t & ind_bit);
    void write(size_t const & c, int32_t const & child, char * const & to, size_t & pos, size_t & ind_bit);
    void get_lengths(size_t pos, size_t lenghts);
    void print_tree (size_t v, char * to, size_t & pos_char, size_t & pos, size_t & ind_pos);
public:
    haffman (char * in, size_t size);
    haffman (std::vector <size_t> freq_);
    std::vector <char> get_tree();
    void encode(status & a);
    void decode (status & a);
    static void calc_freq(std::vector <size_t> & freq, char *, size_t len);
};
#endif
