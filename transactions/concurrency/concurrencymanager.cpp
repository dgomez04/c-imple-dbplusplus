#include "block.h"
#include "locktable.h"
#include "concurrencymanager.h"

bool ConcurrencyManager::has_xlock(Block block) {
    auto locktype (locks.find(block));
    return (locktype != locks.end() && locktype->second == "X");
};

void ConcurrencyManager::slock(Block block) {
    if(locks.find(block) == locks.end()) {
        locktable.slock(block);
        locks[block] = "S";
    }
};

void ConcurrencyManager::xlock(Block block) {
    if(!has_xlock(block)) {
        slock(block);
        locktable.xlock(block);
        locks[block] = "X";
    }
};

void ConcurrencyManager::release() {
    for(auto& [block, _] : locks) 
        locktable.unlock(block);
    locks.clear();
};