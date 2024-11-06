#include "logrecord.h"
#include "startrecord.h"
#include "commitrecord.h"
#include "rollbackrecord.h"
#include "checkpointrecord.h"
#include "setstringrecord.h"
#include "setintrecord.h"

std::unique_ptr<LogRecord> LogRecord::create_logrecord(std::vector<uint8_t>& bytes) {
    Page page(bytes);
    int record_type {page.get_int(0)};
    
    switch (record_type) {
        case int(Operation::CHECKPOINT): 
            return std::make_unique<CheckpointRecord>();
        case int(Operation::START): 
            return std::make_unique<StartRecord>(page);
        case int(Operation::COMMIT): 
            return std::make_unique<CommitRecord>(page);
        case int(Operation::ROLLBACK): 
            return std::make_unique<RollbackRecord>(page);
        case int(Operation::SETINT): 
            return std::make_unique<SetIntRecord>(page);
        case int(Operation::SETSTRING): 
            return std::make_unique<SetStringRecord>(page);
        default:
            return nullptr;
    }
}
