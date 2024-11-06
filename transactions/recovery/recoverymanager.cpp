#include "transaction.h"
#include <vector>
#include "logmanager.h"
#include "buffermanager.h"
#include "transaction.h"
#include "buffer.h"
#include "startrecord.h"
#include "commitrecord.h"
#include "rollbackrecord.h"
#include "checkpointrecord.h"
#include "setstringrecord.h"
#include "setintrecord.h"
#include "logiterator.h"
#include <unordered_set> 
#include <string>

RecoveryManager::RecoveryManager(Transaction& transaction, int transaction_no, LogManager& lm, BufferManager& bm) 
    : lm(lm)
    , bm(bm)
    , transaction(transaction)
    , transaction_no {transaction_no}
{
    StartRecord::write_to_log(lm, transaction_no);
}

void RecoveryManager::commit() {
    bm.flush_all(transaction_no);
    int lsn {CommitRecord::write_to_log(lm, transaction_no)};
    lm.flush(lsn);
}

void RecoveryManager::rollback() {
    do_rollback();
    bm.flush_all(transaction_no);
    int lsn {RollbackRecord::write_to_log(lm, transaction_no)};
    lm.flush(lsn);
}

void RecoveryManager::recover() {
    do_recover();
    bm.flush_all(transaction_no);
    int lsn {CheckpointRecord::write_to_log(lm)};
    lm.flush(lsn);
}

int RecoveryManager::set_int(Buffer& buffer, int offset, int newval) {
    int oldval {buffer.get_contents().get_int(offset)};
    Block block {buffer.get_block()};
    return SetIntRecord::write_to_log(lm, transaction_no, block, offset, oldval);
}

int RecoveryManager::set_string(Buffer& buffer, int offset, std::string newval) {
    std::string oldval {buffer.get_contents().get_string(offset)};
    Block block {buffer.get_block()};
    return SetStringRecord::write_to_log(lm, transaction_no, block, offset, oldval);
}

void RecoveryManager::do_rollback() {
    for(auto it {lm.begin()}; it != lm.end(); ++it) { 
        auto bytes {*it};
        auto record {LogRecord::create_logrecord(bytes)};
        if(record && record->transaction_no() == transaction_no) {
            if(record->op() == LogRecord::Operation::START) {
                return;
            }
            record->undo(transaction);
        }
    }
}

void RecoveryManager::do_recover() {
    std::unordered_set<int> finished_transactions;
    for(auto it {lm.begin()}; it != lm.end(); ++it) {
        auto bytes {*it};
        auto record {LogRecord::create_logrecord(bytes)};

        if(record && record->op() == LogRecord::Operation::CHECKPOINT) {
            return;
        }

        if(record && (record->op() == LogRecord::Operation::COMMIT || record->op() == LogRecord::Operation::ROLLBACK)) {
            finished_transactions.insert(record->transaction_no());
        } else if (finished_transactions.find(record->transaction_no()) == finished_transactions.end()) {
            record->undo(transaction);
        }
    }
}