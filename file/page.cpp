#include <vector>
#include "page.h"

Page::Page(int block_size)
    : buffer(block_size)
{
};

int Page::get_int(int offset) const
{    
    if(offset < 0 || offset + 3 >= buffer.size())
    {
        return -1;
    }

    uint8_t pos_1 = buffer[offset];
    uint8_t pos_2 = buffer[offset + 1];
    uint8_t pos_3 = buffer[offset + 2];
    uint8_t pos_4 = buffer[offset + 3];

    uint32_t result = ((static_cast<uint32_t>(pos_1) << 24) | (static_cast<uint32_t>(pos_2) << 16) 
            | (static_cast<uint32_t>(pos_3) << 8) | static_cast<uint32_t>(pos_4));

    return static_cast<int32_t>(result);
};

void Page::set_int(int offset, int val)
{

};

