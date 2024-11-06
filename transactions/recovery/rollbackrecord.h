#ifndef ROLLBACK_RECORD
#define ROLLBACK_RECORD

#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>

class RollbackRecord : public LogRecord
{    
    private:
        int tsx_no;
    public: 
        RollbackRecord(Page p);

        Operation op() override;

        int transaction_no() override;

        void undo(Transaction transaction) override; 
        
        std::string to_string();

        static int write_to_log(LogManager& lm, int transaction_no);
};

#endif