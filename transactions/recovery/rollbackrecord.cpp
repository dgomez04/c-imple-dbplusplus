#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>
#include "page.h"
#include "rollbackrecord.h"
#include <sstream>

RollbackRecord::RollbackRecord(Page p)
    : tsx_no { p.get_int(sizeof(int))}
{
}

LogRecord::Operation RollbackRecord::op() {
    return LogRecord::Operation::ROLLBACK;
}

int RollbackRecord::transaction_no() {
    return tsx_no;
}

void RollbackRecord::undo(Transaction transaction) {}

std::string RollbackRecord::to_string() {
    std::ostringstream oss;
    oss << "<ROLLBACK " << tsx_no << ">";
    return oss.str();
}

int RollbackRecord::write_to_log(LogManager& lm, int transaction_no) {
    std::vector<uint8_t> record (2 * sizeof(int));
    Page page {record};
    page.set_int(0, int(LogRecord::Operation::ROLLBACK));
    page.set_int(sizeof(int), transaction_no);
    return lm.append(record);
}