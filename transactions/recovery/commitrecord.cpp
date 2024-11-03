#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>
#include "page.h"
#include "commitrecord.h"
#include <sstream>

CommitRecord::CommitRecord(Page p)
    : tsx_no { p.get_int(sizeof(int))}
{
}

LogRecord::Operation CommitRecord::op() {
    return LogRecord::Operation::COMMIT;
}

int CommitRecord::transaction_no() {
    return tsx_no;
}

void CommitRecord::undo(Transaction transaction) {}

std::string CommitRecord::to_string() {
    std::ostringstream oss;
    oss << "<COMMIT " << tsx_no << ">";
    return oss.str();
}

int CommitRecord::write_to_log(LogManager& lm, int transaction_no) {
    std::vector<uint8_t> record (2 * sizeof(int));
    Page page {record};
    page.set_int(0, int(LogRecord::Operation::COMMIT));
    page.set_int(sizeof(int), transaction_no);
    return lm.append(record);
}