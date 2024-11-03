#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>
#include "page.h"
#include "startrecord.h"
#include <sstream>

StartRecord::StartRecord(Page p)
    : tsx_no { p.get_int(sizeof(int))}
{
}

LogRecord::Operation StartRecord::op() {
    return LogRecord::Operation::START;
}

int StartRecord::transaction_no() {
    return tsx_no;
}

void StartRecord::undo(Transaction transaction) {}

std::string StartRecord::to_string() {
    std::ostringstream oss;
    oss << "<START " << tsx_no << ">";
    return oss.str();
}

int StartRecord::write_to_log(LogManager& lm, int transaction_no) {
    std::vector<uint8_t> record (2 * sizeof(int));
    Page page {record};
    page.set_int(0, int(LogRecord::Operation::START));
    page.set_int(sizeof(int), transaction_no);
    return lm.append(record);
}