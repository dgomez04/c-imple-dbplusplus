#include "block.h"
#include "locktable.h"
#include <mutex> 

void LockTable::xlock(Block block) {
    std::unique_lock<std::mutex> lock(mtx);
    try {
        auto timestamp {std::chrono::steady_clock::now()};
        while(has_slocks(block)) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - timestamp);
            auto remaining_time = MAX_TIME - elapsed_time; 

            if(remaining_time <= std::chrono::milliseconds::zero()) throw std::runtime_error("Lock operation timed out");

            cv.wait_for(lock, remaining_time);

            if(has_slocks(block) && waiting_too_long(timestamp)) throw std::runtime_error("Lock operation timed out");
        }
        locks[block] = -1;
    } catch(...) {
        throw;
    }
}

void LockTable::slock(Block block) {
    std::unique_lock<std::mutex> lock(mtx);

    try {
        auto timestamp {std::chrono::steady_clock::now()};
        while(has_xlock(block)) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - timestamp);
            auto remaining_time = MAX_TIME - elapsed_time; 

            if(remaining_time <= std::chrono::milliseconds::zero()) throw std::runtime_error("Lock operation timed out");

            cv.wait_for(lock, remaining_time);

            if(has_xlock(block) && waiting_too_long(timestamp)) throw std::runtime_error("Lock operation timed out");
        }
        locks[block]++;
    } catch(...) { 
        throw;
    }
}

void LockTable::unlock(Block block) {
    std::lock_guard<std::mutex> lock(mtx);
    int lockval {get_lockval(block)};
    if(lockval > 1) { 
        locks[block]--;
    } else {
        locks.erase(block);
         cv.notify_all();
    }
}

bool LockTable::has_xlock(Block block) {
    return get_lockval(block) < 0;
}

bool LockTable::has_slocks(Block block) {
    return get_lockval(block) > 1;
}

int LockTable::get_lockval(Block block) {
    auto lockval (locks.find(block));
    if(lockval == locks.end()) return 0;
    return lockval->second;
}

bool LockTable::waiting_too_long(const std::chrono::steady_clock::time_point& starttime) {
    auto elapsed {std::chrono::steady_clock::now() - starttime};
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) > MAX_TIME;
}