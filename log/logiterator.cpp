#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logiterator.h"
#include <iterator>
#include <vector>

LogIterator::LogIterator(FileManager& fm, const Block& block)
    : fm(fm)
    , block(block)
    , page(std::vector<uint8_t>(fm.get_block_size()))
    , current_pos(0)
    , boundary(0)
{
    move_to_block(block);
};

/*
    NOTE. To achieve same behavior as next(), we have to increment and then dereference. 
*/       
LogIterator& LogIterator::operator++()
{
    if(current_pos == fm.get_block_size()) { 
        block = Block(block.get_filename(), block.get_id() - 1);
        move_to_block(block);
    }
    current_pos += sizeof(int) + page.get_bytes(current_pos).size();
    return *this;
};

LogIterator::value_type LogIterator::operator*() const
{
    return page.get_bytes(current_pos);
};
/*
    Conceptually the same as hasNext(), will allow us to set iterator != iterator.end();
*/
bool LogIterator::operator!=(const LogIterator& other) const
{
    return current_pos != other.current_pos || block != other.block;
};

/* 
    Moves to the specified log block and positions it at the first record in that block. 
*/
void LogIterator::move_to_block(const Block& block)
{
    fm.read(block, page);
    boundary = page.get_int(0);
    current_pos = boundary;
}
