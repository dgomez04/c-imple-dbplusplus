#include "recoverymanager.h"
#include "concurrencymanager.h"
#include "buffermanager.h"
#include "filemanager.h"
#include "bufferlist.h"
#include "transaction.h"
#include <iostream>

int Transaction::next_transaction_no = 0;



Transaction::Transaction(FileManager& fm, LogManager& lm, BufferManager& bm) 
    : fm(fm)
    , bm(bm)
    , transaction_no(next_transaction_number())
    , rm(*this, transaction_no, lm ,bm)
    , cm()
    , my_buffers(bm)
{
}

int Transaction::next_transaction_number() {
    return ++next_transaction_no;
}

void Transaction::commit() {
    rm.commit();
    std::cout << "transaction " << transaction_no  << " commited" << '\n';
    cm.release();
    my_buffers.unpin_all();
}

void Transaction::rollback() {
    rm.rollback();
    std::cout << "transaction " << transaction_no << " rolled back" << '\n';
    cm.release();
    my_buffers.unpin_all();
}

void Transaction::recover() {
    bm.flush_all(transaction_no);
    rm.recover();
}

void Transaction::pin(Block& block) {
    my_buffers.pin(block);
}

void Transaction::unpin(Block& block) {
    my_buffers.unpin(block);
}

int Transaction::get_int(Block& block, int offset) {
    cm.slock(block);
    Buffer* buffer {my_buffers.get_buffer(block)};
    if(buffer)
        return buffer->get_contents().get_int(offset);
    return -1;
}

void Transaction::set_int(Block& block, int offset, int value, bool log) {
    cm.xlock(block);
    Buffer* buffer {my_buffers.get_buffer(block)};
    int lsn {-1};
    if(log)
        lsn = rm.set_int(*buffer, offset, value);
    Page page {buffer->get_contents()};
    page.set_int(offset, value);
    buffer->set_modified(transaction_no, lsn);
}

std::string Transaction::get_string(Block& block, int offset) {
    cm.slock(block);
    Buffer* buffer {my_buffers.get_buffer(block)};
    if(buffer)
        return buffer->get_contents().get_string(offset);
    return "";
}

void Transaction::set_string(Block& block, int offset, std::string value, bool log) {
    cm.xlock(block);
    Buffer* buffer {my_buffers.get_buffer(block)};
    int lsn {-1};
    if(log)
        lsn = rm.set_string(*buffer, offset, value);
    Page page {buffer->get_contents()};
    page.set_string(offset, value);
    buffer->set_modified(transaction_no, lsn);
}

int Transaction::size(std::string filename) {
    Block dummy(filename, END_OF_FILE);
    cm.slock(dummy);
    return fm.length(filename);
}

Block Transaction::append(std::string filename) {
    Block dummy(filename, END_OF_FILE);
    cm.xlock(dummy);
    return fm.append(filename);
}

int Transaction::block_size() {
    return fm.get_block_size();
}

int Transaction::available_buffers() {
    return bm.available();
}
