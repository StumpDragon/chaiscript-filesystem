#pragma once

#include <memory>
#include <vector>

class fs_directory_iterator { 
public: 
	fs_directory_iterator() { 
	}
};

class fs_directory { 
    std::unique_ptr<fs::directory_entry> dir_; 
public: 
	fs_directory() {}
    fs_directory(const fs::path &p) { 
    	dir_ = std::make_unique<fs::directory_entry>(p);
    }
    bool open(const fs::path &p) {
    	dir_ = std::make_unique<fs::directory_entry>(p);
        return false;	
    }
    bool is_valid() { 
    	bool ret = dir_->path().empty();
    	return ret;
    }
};