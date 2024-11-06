#ifndef TSX
#define TSX

#include "recoverymanager.h"
#include "concurrencymanager.h"
#include "buffermanager.h"
#include "filemanager.h"
#include "bufferlist.h"

class Transaction
{    
    private:
        static int next_transaction_no;
        static constexpr int END_OF_FILE {-1};
        int transaction_no;
        RecoveryManager rm;
        ConcurrencyManager cm;
        BufferManager& bm;
        FileManager& fm;
        BufferList my_buffers;

        static int next_transaction_number();
    public:
        Transaction(FileManager& fm, LogManager& lm, BufferManager& bm);

        void commit();

        void rollback();

        void recover();

        void pin(Block& block);

        void unpin(Block& block);

        int get_int(Block& block, int offset);

        void set_int(Block& block, int offset, int value, bool log);

        std::string get_string(Block& block, int offset);

        void set_string(Block& block, int offset, std::string value, bool log);

        int size(std::string filename);

        Block append(std::string filename);

        int block_size();

        int available_buffers();



};

#endif