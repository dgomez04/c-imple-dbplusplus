#ifndef LOG_ITERATOR
#define LOG_ITERATOR

#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logiterator.h"
#include <iterator>
#include <vector>

class LogIterator
{
    private: 
        FileManager& fm;
        Block block; 
        Page page;
        int current_pos;
        int boundary;

        void move_to_block(const Block& block);

    public: 
        // type definitions for stdlib compatibility
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::vector<uint8_t>;
        using pointer = value_type*;
        using reference = value_type&;


        LogIterator(FileManager& fm, const Block& block); 
        
        LogIterator& operator++(); //Prefix increment, maps to next().
        value_type operator*() const; // Dereference operator. 
        bool operator!=(const LogIterator& other) const; // Comparison for end-check. 
};

#endif