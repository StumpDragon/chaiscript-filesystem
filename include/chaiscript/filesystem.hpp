#include <cstdint>
#include <string>
#include <vector>
#include <regex>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>
//#include <chaiscript/chaiscript_basic.hpp>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace c  = chaiscript;
using boost::system::error_code;
using std::string;
using std::vector;

// TODO: Temporary for debugging
#include <iostream>

// Create the module here. 
//
namespace chaiscript { 
    namespace filesystem { 


/*
class canonicalized_path : public fs::path {
    fs::path canonicalize(const fs::path& p) {
        error_code ec;
        auto retP = fs::canonical(p, ec); 

        std::cout << "error_code: " << ec.value() << " " << ec.category().name() << " " << ec.message() << std::endl;
        std::cout << "canonical: " << retP << std::endl;
        return retP;
    }

public:
	canonicalized_path(const fs::path& p)    : fs::path(canonicalize(p)) { }
    canonicalized_path(const std::string& p) : fs::path(canonicalize(fs::path(p))) { }
    canonicalized_path(const char* p)        : fs::path(canonicalize(fs::path(p))) { }
};
*/

/* Do not change order of includes without due care */
#include <chaiscript/filesystem/fs_directory.hpp> 
#include <chaiscript/filesystem/fs_file.hpp> 
#include <chaiscript/filesystem/fs_sandbox.hpp> 
#include <chaiscript/filesystem/fs_module.hpp> 


    } //namespace fileystem
} // namespace chaiscript
