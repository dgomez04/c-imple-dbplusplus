#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>
#include "page.h"
#include "checkpointrecord.h"

LogRecord::Operation CheckpointRecord::op() {
    return LogRecord::Operation::CHECKPOINT;
}

int CheckpointRecord::transaction_no() {
    return -1;
}

void CheckpointRecord::undo(Transaction transaction) {}

std::string CheckpointRecord::to_string() {
    return "<CHECKPOINT>";
}

int CheckpointRecord::write_to_log(LogManager& lm) {
    std::vector<uint8_t> record (sizeof(int));
    Page page {record};
    page.set_int(0, int(LogRecord::Operation::CHECKPOINT));
    return lm.append(record);
}