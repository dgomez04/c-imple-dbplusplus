#include <vector>
#include "page.h"

Page::Page(int block_size)
    : buffer(block_size)
{
};

int Page::get_int(int offset) const
{    
    if(offset < 0 || offset + 3 > buffer.size()) return -1;


    uint8_t pos_1 = buffer[offset];
    uint8_t pos_2 = buffer[offset + 1];
    uint8_t pos_3 = buffer[offset + 2];
    uint8_t pos_4 = buffer[offset + 3];

    uint32_t result { ((static_cast<uint32_t>(pos_1) << 24) | (static_cast<uint32_t>(pos_2) << 16) 
            | (static_cast<uint32_t>(pos_3) << 8) | static_cast<uint32_t>(pos_4)) };

    return static_cast<int32_t>(result);
};

void Page::set_int(int offset, int val)
{
    if(offset < 0 || offset + 3 > buffer.size()) return;

    buffer[offset] = (val >> 24) & 0xFF;
    buffer[offset + 1] = (val >> 16) & 0xFF;
    buffer[offset + 2] = (val >> 8) & 0xFF;
    buffer[offset + 3] = (val & 0xFF);
};


std::vector<uint8_t> Page::get_bytes(int offset) const
{  
    int length { get_int(offset) };

    if(offset < 0 || offset + 4 + length > buffer.size()) return {};    

    int data_offset { offset + 4 };
    std::vector<uint8_t> rs {};

    auto begin = buffer.begin() + data_offset;
    auto end = buffer.begin() + data_offset + length;

    std::copy(begin, end, std::back_inserter(rs));

    return rs;
};

void Page::set_bytes(int offset, const std::vector<uint8_t>& b)
{
    int length { static_cast<int>(b.size()) };

    if(offset < 0 || offset + 4 + length > buffer.size()) return;

    set_int(offset, length);

    int b_offset { offset + 4 };

    for(int i { 0 }; i < length; i++)
    {
        buffer[b_offset + i] = b[i];
    }
};

std::string Page::get_string(int offset) const
{
    std::vector<uint8_t> rs = get_bytes(offset);
    return std::string(rs.begin(), rs.end());
};

void Page::set_string(int offset, std::string_view s)
{
    set_bytes(offset, std::vector<uint8_t>(s.begin(), s.end()));
};

int Page::max_length(int strlen) 
{
    const int bytes_per_char = 1; // assuming 1 byte for ASCII;
    return sizeof(int) + (strlen * bytes_per_char);
}

uint8_t* Page::contents() {
    return buffer.data();
}
