#include "block.h"
#include "page.h"
#include "filemanager.h"
#include "logmanager.h"
#include "buffer.h"
#include <vector> 

Buffer::Buffer(FileManager& fm, LogManager& lm)
    : fm(fm)
    , lm(lm)
    , contents(std::vector<uint8_t>(fm.get_block_size()))
    , block("", -1)
    , pins(0)
    , transaction_no(-1)
    , lsn(-1)
{

};

Page& Buffer::get_contents() 
{
    return contents;
};

Block& Buffer::get_block()
{
    return block;
};

void Buffer::set_modified(int transaction_no, int lsn)
{
    this->transaction_no = transaction_no;
    if(lsn >= 0) this->lsn = lsn;
};

bool Buffer::is_pinned() const
{
    return pins > 0;
};

int Buffer::modifying_transaction() const 
{
    return transaction_no;
};

void Buffer::assign_to_block(const Block& block)
{
    flush();
    this->block = block;
    fm.read(this->block, contents);
    pins = 0;
};

void Buffer::flush() 
{
    if(transaction_no >= 0) {
        lm.flush(lsn);
        fm.write(block, contents);
        transaction_no = -1;
    }
};

void Buffer::pin()
{
    pins++;
};

void Buffer::unpin()
{
    pins--;
}

bool Buffer::operator!() const {
    return block.get_id() != -1;
}