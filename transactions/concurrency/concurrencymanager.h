#ifndef CONCURRENCY_MGR
#define CONCURRENCY_MGR


#include <unordered_map>
#include "block.h"
#include "locktable.h"

class ConcurrencyManager
{
    private: 
        static LockTable locktable;
        std::unordered_map<Block, std::string> locks;
        bool has_xlock(Block block);
    public: 
        void slock(Block block);

        void xlock(Block block);

        void release();
};

#endif