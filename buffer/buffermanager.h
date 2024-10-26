#ifndef BUFFER_MGR
#define BUFFER_MGR

#include "block.h"
#include "page.h"
#include "buffer.h"
#include "filemanager.h"
#include "logmanager.h"
#include <vector>
#include <mutex> 
#include <condition_variable>

class BufferManager
{
    private: 
        std::vector<Buffer> buffer_pool;
        int num_available;
        long MAX_TIME = 10000; // 10 seconds
        std::mutex mtx;
        std::condition_variable cv;

         Buffer* try_to_pin(Block block);
         Buffer* find_existing_buffer(Block& block);
         Buffer* choose_unpinned_buffer();
    public: 
        BufferManager(FileManager& fm, LogManager& lm, int num_buffers);

        int available() const; 

        void flush_all(int transaction_no);

        void unpin(Buffer& buff);

        Buffer& pin(Block block);

};

#endif