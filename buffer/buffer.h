#ifndef BUFFER
#define BUFFER

#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logmanager.h"

class Buffer
{
    private: 
        FileManager& fm;
        LogManager& lm;
        Page contents;
        Block block;
        int pins;
        int transaction_no;
        int lsn; 
    public: 
        Buffer(FileManager& fm, LogManager& lm);

        bool operator!() const;

        Page& get_contents();

        Block& get_block();

        void set_modified(int transaction_no, int lsn);

        bool is_pinned() const;

        int modifying_transaction() const;

        void assign_to_block(const Block& block);

        void flush();

        void pin();

        void unpin();
};

#endif