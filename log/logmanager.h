#ifndef LOG_MGR
#define LOG_MGR

#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logiterator.h"
#include <string>
#include <vector>
#include <mutex>

class LogManager
{
    private: 
        FileManager& fm;
        std::string logfile;
        Page logpage;
        Block current_block;
        std::mutex mtx;

        int latest_lsn;
        int last_saved_lsn; 

        Block append_new_block();
        void flush();

    public: 
        LogManager(FileManager& fm, std::string& log_file);

        void flush(int lsn);

        LogIterator begin();
        LogIterator end();


        int append(std::vector<uint8_t>& record);
};

#endif