#include <string>
#include <vector>
#include <regex>
//#include <chai/filesystem/sandbox.hpp> 

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


class fs_sandbox { 
	vector<fs::path> sandbox_paths; 

public:
	fs_sandbox() { }
    fs_sandbox(string p) { 
        /*for ( auto p : paths ) { 
            sandbox_paths.push_back( fs::path(p) );
        }*/
        sandbox_paths.push_back( fs::path(p) );
    }
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
	bool isAllowed(const fs::path& p) const {
        auto cp = p.lexically_normal();
        for ( auto& sp: sandbox_paths ) { 
            //std::cout << "sboxpath: " << sp.string() << " input: " << cp.string() << std::endl;
        	if (isContainedBy(sp, cp))  { 
                //std::cout << "\tis Contained == true" << std::endl;
                return true;
            }
        }
        return false;
	}
    /*bool exists(const fs::path& p ) const  {
        if ( ! isAllowed(p) ) return false;
        error_code ec; 
        return fs::exists(p, ec); 
    }*/

protected:
	bool isContainedBy(const fs::path& dirParent, const fs::path& filePath) const {
		/* Care of
		 * https://stackoverflow.com/a/15549954
		 */
        fs::path dir(dirParent);
        if (dir.filename() == ".")
            dir.remove_filename();
        // We're also not interested in the file's name.
        //assert(file.has_filename());
        fs::path file(filePath);
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

class fs_file { 
    fs::path path_; 
public:
    fs_file(const string& path) : path_(path) {
    }
    bool is_valid() { 
        return false;
    }
    string readline() { 
        return "";
    }
    long writeline(const string& s) {
        return(0L);
    }
    bool eof() {
        return true;
    }
};

class fs_module {
    fs_sandbox  sandbox_;
    ChaiScript& chai_;
    chaiscript::ModulePtr module_;

public: 
    fs_sandbox& sandbox() { return sandbox_; };

    fs_module(ChaiScript& cs) : chai_(cs), module_(new chaiscript::Module()) { 
        chai_.register_namespace([&,this](Namespace& fsName) {
                auto fs = *this;
                fsName["exists"]       = var(fun([&,fs](const string& s) { return fs.exists(s); }));
                fsName["current_path"] = var(fun([&,fs]() { return fs.current_path(); }));
                fsName["is_directory"] = var(fun([&,fs](const string& s) { return fs.is_directory(s); }));
                fsName["create_directories"] = var(fun([fs](const string& s) { return fs.create_directories(s); }));
                fsName["open"]         = var(fun([fs](const string& s) { return fs.open(s); }));
                fsName["create"]         = var(fun([fs](const string& s) { return fs.open(s); }));
            },
            "chaifs");

        chaiscript::utility::add_class<fs_sandbox>(*module_, "fs_file", 
            {
                chaiscript::constructor<fs_file(const string& p)>()
                //chaiscript::constructor<fs_sandbox(const fs_sandbox&)>(), 
            },
            {
                { chaiscript::fun(&fs_file::is_valid), "is_valid" },
                { chaiscript::fun(&fs_file::eof), "eof" },
            });
        chai_.add( module_ );
    }

    static fs::path normalize(const string& p) { 
        string s = p;
        std::replace(s.begin(), s.end(), '\\', '/');
        return fs::path(s);
    }


    bool exists(const string& p ) const { 
        //std::cout << "Testing exists() " << p << std::endl;
        if ( ! sandbox_.isAllowed(p) ) return false;
        error_code ec; 
        return fs::exists(p, ec); 
        //return sandbox_.exists(fs::path(p));
    }
    bool is_directory(const string& p ) const { 
        if ( ! sandbox_.isAllowed(p) ) return false;
        error_code ec; 
        return fs::is_directory(p, ec); 
    }
    const string current_path() const {
        // TODO: Do we need to sandbox this?  If not an allowed path should we report sbox[0]?
        error_code ec;
        auto p = fs::current_path(ec);
        string s = p.string();
        //std::cout << "Current Path: " << s << std::endl;
        return s;
    }
    bool create_directories(const string& paths) const { 
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(paths) ) return false;
        error_code ec;
        fs::create_directories( p, ec );
        bool ret = fs::is_directory( p );
        /*std::cout << "creating directories: " << paths << " result: " << ret 
                  << " ec: " << ec.value() << std::endl;
        */
        return ret;
    }
    fs_file open(const string &p) const { 
        fs_file f(p);
        //std::cout << "fs_file : single arg func " << std::endl;

        return f;
    }
    fs_file create(const string &p) const { 
        fs_file f(p);
        //std::cout << "fs_file : create "  << std::endl;

        return f;
    }
};




    } //namespace fileystem
} // namespace chaiscript
