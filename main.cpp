#include <iostream>
#include "file/filemanager.h"
#include "file/block.h"     
#include "file/page.h"       

int main() {
    FileManager fm("db", 400);

    Block blk("testfile", 2);
    Page p1(fm.get_block_size());
    Page p2(fm.get_block_size());


    int pos1 = 88;
    std::string test_string { "abcdefghijklm" };
    int size = p1.max_length(test_string.length());  
    int pos2 = pos1 + size; 

    p1.set_string(pos1, test_string);
    p1.set_int(pos2, 345);

    fm.write(blk, p1);

    fm.read(blk, p2);

    std::cout << "offset " << pos1 << " contains " << p2.get_string(pos1) << std::endl;
    std::cout << "offset " << pos2 << " contains " << p2.get_int(pos2) << std::endl;

    return 0;
}