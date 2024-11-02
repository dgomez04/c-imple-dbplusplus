#include "block.h"
#include "page.h"
#include "buffer.h"
#include "filemanager.h"
#include "logmanager.h"
#include "buffermanager.h"
#include <condition_variable>
#include <vector>
#include <mutex> 

BufferManager::BufferManager(FileManager& fm, LogManager& lm, int num_buffers)
    : num_available(num_buffers)
{
    for(int i {0}; i < num_buffers; i++) 
        buffer_pool.emplace_back(fm, lm);
}


int BufferManager::available() const
{
    return num_available;
};

void BufferManager::flush_all(int transaction_no)
{
    for(Buffer& buff : buffer_pool)
        if(buff.modifying_transaction() == transaction_no)
            buff.flush();
};

void BufferManager::unpin(Buffer& buffer)
{
    std::lock_guard<std::mutex> lock(mtx);
    buffer.unpin();
    if(!buffer.is_pinned()) {
        num_available++;
        cv.notify_all();
    }
};


Buffer& BufferManager::pin(Block block)
{
    std::unique_lock<std::mutex> lock(mtx);
    auto start_time {std::chrono::steady_clock::now()};

    Buffer* buffer = try_to_pin(block);

    while(buffer == nullptr) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

        if(elapsed_time >= MAX_TIME) {
            throw std::runtime_error("Buffer pinning timed out.");
        }

        cv.wait_for(lock, std::chrono::milliseconds(MAX_TIME - elapsed_time), [&]() {
            return num_available > 0;
        });

        buffer = try_to_pin(block);
    }

    return *buffer;

};

Buffer* BufferManager::try_to_pin(Block block)
{
    Buffer* buffer = find_existing_buffer(block);
    if(buffer == nullptr)
    {
        buffer = choose_unpinned_buffer();
        if(buffer == nullptr) 
        {
            return nullptr;
        }
        buffer->assign_to_block(block);
    }

    if(!buffer->is_pinned())
    {
        num_available--;
    }

    buffer->pin();
    return buffer;
};

Buffer* BufferManager::find_existing_buffer(Block& block)
{
    for(Buffer& buffer : buffer_pool)
    {
        const Block& buffer_block {buffer.get_block()};
        if(buffer_block.get_id() != -1 && buffer_block == block)
            return &buffer;
    }
    return nullptr;
};

Buffer*  BufferManager::choose_unpinned_buffer() 
{
    for(Buffer& buffer : buffer_pool)
    {
        if(!buffer.is_pinned())
            return &buffer;
    }
    return nullptr;
};