#ifndef PAGE
#define PAGE

#include <vector> 

class Page
{
    private: 
        std::vector<uint8_t> buffer;
    public: 
        Page(int block_size);

        int get_int(int offset) const;

        void set_int(int offset, int val);

        std::vector<uint8_t> get_bytes(int offset) const;

        void set_bytes(int offset, const std::vector<uint8_t>& bytes);

        std::string get_string(int offset) const;

        void set_string(int offset, std::string_view s);

        int max_length(int strlen);

        uint8_t* contents();
};

#endif