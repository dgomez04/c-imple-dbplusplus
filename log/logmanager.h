#ifndef LOG_MGR
#define LOG_MGR

#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logiterator.h"
#include <string>
#include <vector>
class LogManager
{
    private: 
        FileManager fm;
        std::string log_file;
        Page log_page;
        Block current_block;
        int latest_lsn;
        int last_saved_lsn; 

        Block append_new_block();
        void flush();

    public: 
        LogManager(FileManager& fm, std::string& log_file);

        void flush(int lsn);

        LogIterator iterator(); // NOTE: is this still needed? begin() and end() should be enough for range-based loops. 
        LogIterator begin();
        LogIterator end();


        int append(std::vector<uint8_t>& log_rec);
};

#endif