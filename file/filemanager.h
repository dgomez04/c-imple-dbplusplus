#ifndef FILEMANAGER
#define FILEMANAGER

#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "block.h"
#include "page.h"
#include <mutex>

class FileManager
{
    private:
        std::mutex mtx;
        std::filesystem::path directory;
        const int block_size;
        bool is_new;
        std::unordered_map<std::string, std::fstream> open_files;

        std::fstream& get_file(const std::string& filename);
    public: 
        FileManager(const std::filesystem::path& directory, int block_size);
        
        void read(const Block& block, Page& page);
        void write(const Block& block, Page& page);
        Block append(std::string filename);

        int length(std::string filename);

        bool get_is_new() const;

        int get_block_size() const;
};

#endif