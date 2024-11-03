#include <unordered_map>
#include <vector>
#include "block.h"
#include "buffer.h"
#include "buffermanager.h"
#include "bufferlist.h"

BufferList::BufferList(BufferManager& bm) 
    : bm {bm}
    , pins()
    , buffers()
{
}

Buffer* BufferList::get_buffer(const Block& block) {
    auto iterator = buffers.find(block);
    return (iterator != buffers.end()) ? iterator->second.get() : nullptr;
}

void BufferList::pin(const Block& block) {
    buffers[block] = std::make_unique<Buffer>(bm.pin(block));
    pins.push_back(block);
}

void BufferList::unpin(const Block& block) {
    auto iterator {buffers.find(block)};
    if(iterator != buffers.end()) {
        auto buffer {iterator->second.get()};
        bm.unpin(*buffer);
        pins.erase(std::remove(pins.begin(), pins.end(), block), pins.end());
        if(std::find(pins.begin(), pins.end(), block) == pins.end()) {
            buffers.erase(block);
        }
    }
}

void BufferList::unpin_all() {
    for(Block block : pins) {
        auto iterator {buffers.find(block)};
        auto buffer {iterator->second.get()};
        bm.unpin(*buffer);
    }
    buffers.clear();
    pins.clear();
}