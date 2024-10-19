#ifndef BLOCK
#define BLOCK

#include <string>

class Block
{
    private: 
        std::string filename;
        int block_id;

    public: 
        Block(std::string filename, int block_id);

        std::string_view get_filename() const;

        int get_id() const;

        friend bool operator==(const Block& b1, const Block& b2);

        std::string to_string() const; 

        std::size_t hashcode() const;
};

#endif