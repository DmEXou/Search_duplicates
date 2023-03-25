#ifndef CORE_SEARCH_H
#define CORE_SEARCH_H

#include <filesystem>
#include <fstream>
#include <QVector>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <set>
#include <QDebug>
#include <chrono>
#include <execution>

struct file_param {
    size_t hash_str = 0;
    std::filesystem::path file_path;
    size_t file_size = 0;
    int counter = 0;
    bool operator<(file_param lhs) const {
        if(lhs.hash_str != this->hash_str){
            return (lhs.hash_str < this->hash_str);
        }
        else{
            return (lhs.file_size < this->file_size);
        }
        //return (lhs.hash_str < this->hash_str);
    }
    bool operator==(file_param lhs) const {
        return (lhs.file_size == this->file_size && lhs.hash_str == this->hash_str);
    }
    bool operator!=(file_param lhs) const {
        return !(lhs == *this);
    }
};

class Core{
public:
    Core(const QString&, const QVector<QString>&);
    size_t GetSize();
    std::vector<file_param> get_duplicate();

private:
    void serach_all_files(const std::filesystem::path&);
    QVector<file_param> search_duplicate();
    //file_param generated_param(const std::filesystem::recursive_directory_iterator&);
    QVector<std::filesystem::path> all_internal_directories(const std::filesystem::path&);

private:
    template<typename T>
    file_param generated_param(const T& it){
        std::string str;
        std::ifstream ifs (it->path(), std::ios::binary);
        size_t hash_str = 0;
        file_param tmp;
        std::getline(ifs, str);
        hash_str += std::hash<std::string>{}(str)/2;
        std::getline(ifs, str);
        hash_str += std::hash<std::string>{}(str)/2;
        tmp.hash_str = hash_str;
        tmp.file_size = it->file_size();
        tmp.file_path = it->path();
        return tmp;
    }

private:
    std::set<file_param> all_files_sorted;
//    QVector<file_param> files_root_dir;
//    QVector<file_param> all_files;
    std::vector<file_param> all_duplicate;
    QVector<QString> format;
    int counter;
    std::mutex m;
};

#endif // CORE_SEARCH_H
