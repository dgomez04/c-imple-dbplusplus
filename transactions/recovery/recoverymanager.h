#ifndef RECOVERY_MGR
#define RECOVERY_MGR

#include "transaction.h"
#include <vector>
#include "logmanager.h"
#include "buffermanager.h"
#include "transaction.h"
#include "buffer.h"
#include <string>

class RecoveryManager
{    
    private: 
        LogManager& lm;
        BufferManager& bm;
        Transaction& transaction; 
        int transaction_no;    
    public: 
        RecoveryManager(Transaction& transaction, int transaction_no, LogManager& lm, BufferManager& bm);
        
        void commit();

        void rollback();

        void recover();

        int set_int(Buffer& buffer, int offset, int newval);

        int set_string(Buffer& buffer, int offset, std::string newval);

        void do_rollback();

        void do_recover();
};

#endif