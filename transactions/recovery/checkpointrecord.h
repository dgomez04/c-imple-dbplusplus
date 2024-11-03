#ifndef CHECKPOINT_RECORD
#define CHECKPOINT_RECORD

#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>

class CheckpointRecord : LogRecord
{    
    public: 
        CheckpointRecord() = default;

        Operation op() override;

        int transaction_no() override;

        void undo(Transaction transaction) override; 
        
        std::string to_string();

        static int write_to_log(LogManager& lm);
};

#endif