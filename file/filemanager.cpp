#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include "block.h"
#include "page.h"
#include "filemanager.h"


FileManager::FileManager(const std::filesystem::path& directory, int block_size)
    : directory { directory }
    , block_size { block_size }
    , is_new { !std::filesystem::exists(directory) }
{
    if(is_new) std::filesystem::create_directory(directory);

    for(auto& entry : std::filesystem::directory_iterator(directory))
        if(entry.path().filename().string().substr(0, 4) == "temp") std::filesystem::remove(entry); // starts_with is not compiling.
};


std::fstream& FileManager::get_file(const std::string& filename)
{
    auto iterator { open_files.find(filename) };
    
    if(iterator == open_files.end())
    {
        std::filesystem::path path { directory / filename};
        auto& file = open_files[filename];

        file.open(path, std::ios::out);
        if(!file.is_open()) throw std::runtime_error("failed to create file: " + filename);
        file.close();

        file.open(path, std::ios::in | std::ios::out);
        if(!file.is_open()) throw std::runtime_error("failed to open file for reading/writing: " + filename);


        open_files[filename] = std::move(file);
    }

    return iterator->second;
};

void FileManager::read(const Block& block, Page& page)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::fstream& file { get_file(block.get_filename()) };
    file.seekg(block.get_id() * block_size);
    if(!file.read(reinterpret_cast<char*>(page.contents()), block_size)) { throw std::runtime_error("failed to read block: " + std::to_string(block.get_id())); };
};

void FileManager::write(const Block& block, Page& page)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::fstream& file { get_file(block.get_filename()) };
    file.seekp(block.get_id() * block_size);
    if (!file.write(reinterpret_cast<const char*>(page.contents()), block_size)) { throw std::runtime_error("failed to write to block: " + std::to_string(block.get_id())); } 
};

Block FileManager::append(std::string filename)
{   
    std::lock_guard<std::mutex> lock(mtx);
    int block_id = length(filename);
    if (block_id < 0) throw std::runtime_error("invalid block identifier.");

    Block block { filename, block_id };
    std::vector<uint8_t> b (block_size, 0);

    std::fstream& file { get_file(filename) };
    file.seekp(block_id * block_size);
    if (!file.write(reinterpret_cast<const char*>(b.data()), b.size())) { throw std::runtime_error("failed to write to file: " + filename); }
    
    return block;
}
    


int FileManager::length(std::string filename)
{
    auto iterator { open_files.find(filename) };

    if(iterator != open_files.end())
    {
        std::fstream& file { iterator->second };
        if(!file.is_open()) throw std::runtime_error("failed to open file: " + filename);
        
        file.seekg(0, std::ios::end);
        std::streampos size = file.tellg();
        if(size == static_cast<std::streampos>(-1)) throw std::runtime_error("failed to get file size for:" + filename);

        return static_cast<int>(size / block_size);
    }
    return -1;
};

bool FileManager::get_is_new() const { return is_new; };

int FileManager::get_block_size() const { return block_size; };

