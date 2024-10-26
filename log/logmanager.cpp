#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logiterator.h"
#include "logmanager.h"
#include <string>
#include <vector> 
#include <mutex>

LogManager::LogManager(FileManager& fm, std::string& logfile)
    : fm(fm)
    , logfile(logfile)
    , logpage(std::vector<uint8_t>(fm.get_block_size()))
    , current_block(logfile, 0)
    , latest_lsn(0)
    , last_saved_lsn(0) 
{
    int logsize = fm.length(logfile);

    if(logsize == 0) {
        current_block = append_new_block();
    } else {
        current_block = Block(logfile, logsize - 1);
        fm.read(current_block, logpage);
    }
};

/*
    Writes buffer to the log file.
*/
void LogManager::flush()
{
    fm.write(current_block, logpage);
    last_saved_lsn = latest_lsn;
};

/*
    Ensures that the log record corresponidng to the
    specified sequence number has been written to disk. 
    All earlier log records will also be written to disk. 
    @param lsn the log sequence number of a log record.
*/
void LogManager::flush(int lsn)
{
    if(lsn >= last_saved_lsn) flush();
};

int LogManager::append(std::vector<uint8_t>& record)
{
    std::lock_guard<std::mutex> lock(mtx);
    int boundary {logpage.get_int(0)};
    int bytes_needed {record.size() + sizeof(int)};

    if(boundary - bytes_needed < sizeof(int)) {
        flush();
        current_block = append_new_block();
        boundary = logpage.get_int(0);
    }

    int record_position {boundary - bytes_needed};

    logpage.set_bytes(record_position, record);
    logpage.set_int(0, record_position);
    ++latest_lsn;
    return latest_lsn; 
};

/*
    Creates a new block, sets logpage with the blocksize, writes it to the block, and returns it. 
*/
Block LogManager::append_new_block()
{
    Block block {fm.append(logfile)};
    logpage.set_int(0, fm.get_block_size());
    fm.write(block, logpage);
    return block;
};

LogIterator LogManager::begin()
{
    flush();
    return LogIterator(fm, current_block);
};

LogIterator LogManager::end()
{
    return LogIterator(fm, Block("", -1));
};

