#ifndef SETSTRING_RECORD
#define SETSTRING_RECORD

#include "transaction.h"
#include "logrecord.h"
#include "block.h"
#include "logmanager.h"
#include <vector>
#include <string>

class SetStringRecord : public LogRecord
{    
    private: 
        int tsx_no;
        int offset;
        std::string value;
        Block block;
    public: 
        SetStringRecord(Page p);

        Operation op() override;

        int transaction_no() override;

        void undo(Transaction transaction) override; 
        
        std::string to_string();

        static int write_to_log(LogManager& lm, int transaction_no, Block block, int offset, std::string val);
};

#endif