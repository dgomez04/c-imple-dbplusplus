#ifndef BUFFER_LIST
#define BUFFER_LIST

#include <unordered_map>
#include <vector>
#include "block.h"
#include "buffer.h"
#include "buffermanager.h"

class BufferList
{    
    private:
        std::unordered_map<Block, std::unique_ptr<Buffer>> buffers;
        std::vector<Block> pins;
        BufferManager& bm;
    public:
        BufferList(BufferManager& bm);
        Buffer* get_buffer(const Block& block);
        void pin(const Block& block);
        void unpin(const Block& block);
        void unpin_all();
};

#endif