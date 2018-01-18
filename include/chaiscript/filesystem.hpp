//#include <chai/filesystem/sandbox.hpp> 

#include <chaiscript/chaiscript.hpp>
//#include <chaiscript/chaiscript_basic.hpp>
//#include <chaiscript/utility/utility.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using boost::system::error_code;

#include <vector>
#include <iostream>

// Create the module here. 
//
namespace chaiscript { 
    namespace filesystem { 


class canonicalized_path : public fs::path {
    fs::path canonicalize(const fs::path& p) {
        error_code ec;
        auto retP = fs::canonical(p, ec); 

        std::cout << "error_code: " << ec.value() << std::endl;
        return retP;
    }

public:
	canonicalized_path(const fs::path& p)    : fs::path(canonicalize(p)) { }
    canonicalized_path(const std::string& p) : fs::path(canonicalize(fs::path(p))) { }
    canonicalized_path(const char* p)        : fs::path(canonicalize(fs::path(p))) { }
};



class fs_sandbox { 
	std::vector<canonicalized_path> sandbox_paths; 

public:
	fs_sandbox() { }
	error_code add_path(const canonicalized_path& p) {
        sandbox_paths.push_back(p);
	} 
	bool isAllowed(const fs::path& p) {
        error_code ec;
        auto cp = fs::canonical(p, ec);
        for ( auto& sp: sandbox_paths ) { 
        	if (isContainedBy(sp, cp))  return true;
        }
        return false;
	}
protected:
	bool isContainedBy(fs::path& dir, fs::path& file) {
		/* Care of
		 * https://stackoverflow.com/a/15549954
		 */
        if (dir.filename() == ".")
            dir.remove_filename();
        // We're also not interested in the file's name.
        //assert(file.has_filename());
        file.remove_filename();

        // If dir has more components than file, then file can't possibly
        // reside in dir.
        auto dir_len = std::distance(dir.begin(), dir.end());
        auto file_len = std::distance(file.begin(), file.end());
        if (dir_len > file_len)
            return false;

        // This stops checking when it reaches dir.end(), so it's OK if file
        // has more directory components afterward. They won't be checked.
        return std::equal(dir.begin(), dir.end(), file.begin());
    }

};


class fs_module : public chaiscript::Module {
	// Need a place to hold the sandbox paths? 

public: 
    fs_module() : chaiscript::Module() { 
    }
};




    } //namespace fileystem
} // namespace chaiscript
