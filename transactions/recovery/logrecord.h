#ifndef LOG_RECORD
#define LOG_RECORD

#include "transaction.h"
#include <vector>

class LogRecord
{    
    public: 
        enum class Operation {
            CHECKPOINT,
            START,
            COMMIT, 
            ROLLBACK, 
            SETINT,
            SETSTRING
        };

        virtual ~LogRecord() = default;

        virtual Operation op() = 0;

        virtual int transaction_no() = 0;

        virtual void undo(Transaction transaction)= 0;

        static std::unique_ptr<LogRecord> create_logrecord(std::vector<uint8_t>& bytes);
    
};

#endif