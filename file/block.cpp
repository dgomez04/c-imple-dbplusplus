#include <string>
#include "block.h"
#include <sstream>
#include <functional>

Block::Block(std::string filename, int block_id)
    : filename { filename }
    , block_id { block_id }
{
};

std::string_view Block::get_filename() const
{
    return filename;
};

int Block::get_id() const
{
    return block_id;
}

bool operator==(const Block& b1, const Block& b2)
{
    return b1.filename == b2.filename && b1.block_id == b2.block_id;
};

std::string Block::to_string() const
{
    std::ostringstream buffer;
     buffer << "[file " << filename << ", block " << block_id << "]";
     return buffer.str();
};

std::size_t Block::hashcode() const
{   
    return std::hash<std::string>{}(to_string());
};
