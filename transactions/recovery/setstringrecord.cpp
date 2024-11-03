#include "transaction.h"
#include "logrecord.h"
#include "logmanager.h"
#include <vector>
#include "page.h"
#include <string>
#include "setstringrecord.h"
#include <sstream>

SetStringRecord::SetStringRecord(Page p)
    : tsx_no {}
    , offset {}
    , value {}
    , block("", -1)
{
    int tsx_pos {sizeof(int)};
    tsx_no = p.get_int(tsx_pos);

    int file_pos {tsx_pos + sizeof(int)};
    std::string filename {p.get_string(file_pos)};

    int block_pos {file_pos + p.max_length(filename.length())};
    int block_num {p.get_int(block_pos)};
    block = Block(filename, block_num);

    int offset_pos {block_pos + sizeof(int)};
    offset = p.get_int(offset_pos);

    int value_pos {offset_pos + sizeof(int)};
    value = p.get_string(value_pos);
}

LogRecord::Operation SetStringRecord::op() {
    return LogRecord::Operation::SETSTRING;
}

int SetStringRecord::transaction_no() {
    return tsx_no;
}

void SetStringRecord::undo(Transaction transaction) {
    transaction.pin(block);
    transaction.set_string(block, offset, value, false); 
    transaction.unpin(block);
}

std::string SetStringRecord::to_string() {
    std::ostringstream oss;
    oss << "<SETSTRING " << tsx_no << " " << block.to_string() << " " << offset << " " << value << ">";
    return oss.str();
}

int SetStringRecord::write_to_log(LogManager& lm, int transaction_no, Block block, int offset, std::string value) {
    int transaction_pos {sizeof(int)};
    int file_pos {transaction_pos + sizeof(int)};
    int block_pos {file_pos + Page::max_length(block.get_filename().length())};
    int offset_pos {block_pos + sizeof(int)};
    int value_pos {offset_pos + sizeof(int)};

    std::vector<uint8_t> record (value_pos + Page::max_length(value.length()));
    Page p (record);

    p.set_int(0, int(LogRecord::Operation::SETSTRING));
    p.set_int(transaction_pos, transaction_no);
    p.set_string(file_pos, block.get_filename());
    p.set_int(block_pos, block.get_id());
    p.set_int(offset_pos, offset);
    p.set_string(value_pos, value);
    return lm.append(record);
}