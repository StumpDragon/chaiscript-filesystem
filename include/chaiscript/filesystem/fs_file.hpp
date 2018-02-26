#pragma once

#include <fstream>
#include <regex>
#include <memory>
#include <vector>

using std::fstream;
using std::ios; 
using std::string;

class fs_file { 
    fs::path path_; 
    fstream file_; 

public:
    fs_file(const string& path) : path_(path) {
    }
    bool open(bool create) {
        auto p= path_.string();
        #ifdef __CYGWIN__ 
            auto m = create ? ios::out : ios::in|ios::out;
            file_.open(p, m|ios::binary);
        #else
            file_.open(path_.string(), ios::in|ios::out|ios::binary);
        #endif
    	std::cout << "opening file: " << path_.string() << " stream: " << file_.good() << " f: " << file_.fail() << " b: " << file_.bad() << std::endl;
    	return file_.is_open();
    }
    void close() {
        if ( is_valid() ) file_.close();
    }
    bool is_valid() { 
    	bool retVal = file_.is_open();
        std::cout << "fs_file::is_valid p:  " << path_.string() << " RC: " << retVal << std::endl;
        return retVal;
    }
    bool eof() {
        if ( !file_.is_open() ) return false;
        return file_.eof();
    }
    long size() { 
        if ( !file_.is_open() ) return -1L;
        return( fs::file_size( path_ ) );
    }
    void seek(uint64_t bytes, bool relative) { 
        file_.seekg(bytes); 
    }
    uint64_t tell() {
        return(file_.tellg());
    }
    int peek() { 
        return file_.peek();
    }
    void eachline(const std::function<void (const std::string &)> &t_func) { 
        while ( !file_.eof()) {
            string s; 
            std::getline(file_, s);
            std::cout << "Reading line: "  << s << std::endl;
            std::cout << "Reading line length: "  << s.length() << std::endl;
            if ( s.length() > 0 )  t_func(s);
        }
        return;
    }
    void each_byte(const std::function<void (const char x)> &t_func) { 
        while ( !file_.eof()) {
            // read byte from file 
            // call closure
        }        
    }
    string readline() { 
        string s; 
        std::getline(file_, s);
        if ( s.length() > 0 ) return s;
        return "";
    }
    string read(int bytes) { 
        std::vector<char> buffer; 
        buffer.resize(bytes);
        file_.read(&buffer[0], bytes);
        return string(  " " );
    }

    /* WRITING TO THE FILE */
    fs_file& append(const string& s) {
        return (*this);
    }
    fs_file& append(const char ch) { 
        return (*this);
    }
    fs_file& put(char ch) { 
        file_.put(ch);
        return (*this);
    }
    fs_file& write(const string& s) { 
        file_.write(s.c_str(), s.length());
        return (*this);  
    }
    fs_file& write(const char* s, long n) { 
        file_.write(s, n);
        return (*this);
    }
    int writeline(const string& s) {
        long v = s.length() + 1; 
        file_ << s << std::endl;
        return(v);
    }

};
