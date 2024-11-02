#ifndef LOCK_TBL
#define LOCK_TBL


#include <unordered_map>
#include "block.h"
#include <mutex> 
#include <condition_variable>

class LockTable
{
    private: 
        static constexpr std::chrono::milliseconds MAX_TIME {10000};        
        std::unordered_map<Block, int> locks;
        std::mutex mtx; // to mimic synchronized behavior.
        std::condition_variable cv;

        bool has_xlock(Block block);
        bool has_slocks(Block block);
        int get_lockval(Block block);
        bool waiting_too_long(const std::chrono::steady_clock::time_point& starttime);
    public: 
        LockTable() = default;

        void slock(Block block);

        void xlock(Block block);

        void unlock(Block block);
        
};

#endif