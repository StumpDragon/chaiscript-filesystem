//#include <chai/filesystem/sandbox.hpp> 

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>
//#include <chaiscript/chaiscript_basic.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using boost::system::error_code;

#include <vector>
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



class fs_sandbox { 
	std::vector<fs::path> sandbox_paths; 

public:
	fs_sandbox() { }
	error_code add_path(const fs::path& p) {
        error_code ec;
        /* use canonical here vs lexically_normal, we want to store the canonical path,
           and because it's a sandbox, it *SHOULD* probably already exist.
         */
        auto cp = fs::canonical(p, ec); 
        if ( ENOENT != ec.value()) { 
            sandbox_paths.push_back(cp);
        }
        return ec;
	} 
	bool isAllowed(const fs::path& p) {
        auto cp = p.lexically_normal();
        for ( auto& sp: sandbox_paths ) { 
        	if (isContainedBy(sp, cp))  return true;
        }
        return false;
	}
    bool exists(const fs::path& p ) {
        error_code ec; 
        return fs::exists(p, ec); 
    }
    bool exists_str(const std::string& p ) { 
        return exists(fs::path(p));
    }
    const std::string current_path() const {
        error_code ec;
        auto p = fs::current_path(ec);
        std::string s = p.string();
        std::cout << "Current Path: " << s << std::endl;
        return s;
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
    fs_sandbox  sandbox;

public: 
    fs_module() : chaiscript::Module() { 
        chaiscript::utility::add_class<fs_sandbox>(*this, "chai_fs", 
            {
                chaiscript::constructor<fs_sandbox()>()/*,
                chaiscript::constructor<fs_sandbox(const fs_sandbox&)>()*/
            }, 
            {
                { chaiscript::fun(&fs_sandbox::current_path), "current_path" },
                { chaiscript::fun(&fs_sandbox::exists_str), "exists" },
            });
    }
};




    } //namespace fileystem
} // namespace chaiscript
