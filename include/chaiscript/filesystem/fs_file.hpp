#pragma once

#include <fstream>

using std::fstream;
using std::ios; 

class fs_file { 
    fs::path path_; 
    fstream file_; 

public:
    fs_file(const string& path) : path_(path) {
    }
    bool open() {
    	file_.open(path_.string(), ios::in|ios::out|ios::binary);
    	std::cout << "stream: " << file_.good() << " f: " << file_.fail() << " b: " << file_.bad() << std::endl;
    	return file_.is_open();
    }
    bool is_valid() { 
    	bool retVal = file_.is_open();
        std::cout << "fs_file::is_valid p:  " << path_.string() << " RC: " << retVal << std::endl;
        return retVal;
    }
    string readline() { 
        return "";
    }
    void eachline(const std::function<void (const std::string &)> &t_func) { 
        return;
    }
    long writeline(const string& s) {
        return(0L);
    }
    bool eof() {
        return true;
    }
    bool seek(uint64_t bytes, bool relative) { 
        return(false);
    }
    uint64_t tell() {
        return(-1L);
    }
};
