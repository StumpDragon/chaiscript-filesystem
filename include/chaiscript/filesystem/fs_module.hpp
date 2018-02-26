#pragma once

class fs_module {
    fs_sandbox  sandbox_;
    ChaiScript& chai_;
    chaiscript::ModulePtr module_;

public: 
    fs_sandbox& sandbox() { return sandbox_; };

    fs_module(ChaiScript& cs) : chai_(cs), module_(new chaiscript::Module()) { 
        chai_.register_namespace([&,this](Namespace& fsName) {
                auto fs = *this;
                fsName["create_directories"] = var(fun([&,fs](const string& s) { return fs.create_directories(s); }));
                fsName["rmdir"]             = var(fun([&,fs](const string& s) { return fs.rmdir(s); }));
                fsName["open_directory"]     = var(fun([&,fs](const string& s) { return fs.open_directory(s); }));
                fsName["exists"]       = var(fun([&,fs](const string& s) { return fs.exists(s); }));
                fsName["current_path"] = var(fun([&,fs]() { return fs.current_path(); }));
                fsName["is_directory"] = var(fun([&,fs](const string& s) { return fs.is_directory(s); }));
                fsName["is_file"]      = var(fun([&,fs](const string& s) { return fs.is_file(s); }));
                fsName["open"]         = var(fun([fs](const string& s) { return fs.open(s); }));
                fsName["create"]       = var(fun([fs](const string& s) { return fs.create(s); }));
                fsName["file_size"]    = var(fun([fs](const string& s) { return fs.file_size(s); }));
                fsName["remove"]    = var(fun([fs](const string& s) { return fs.remove(s); }));
                fsName["remove_all"]    = var(fun([fs](const string& s) { return fs.remove_all(s); }));
                // This results in a compile error (Cygwin/babun, GCC 6.4.0) 
                //fsName["get_temp_file"] = var(fun([fs]() mutable  { return fs.get_temp_file(); } ));
                fsName["get_temp_file"] = var(fun([fs]() { return fs.get_temp_file(); } ));
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
                { chaiscript::fun(&fs_file::close), "close" },
                { chaiscript::fun(&fs_file::readline), "readline" }, 
                { chaiscript::fun(&fs_file::writeline), "writeline" },
                { chaiscript::fun(&fs_file::eachline), "eachline" },

            });
        chai_.add( module_ );
    }

    static fs::path normalize(const string& p) { 
        string s = p;
        std::replace(s.begin(), s.end(), '\\', '/');
        return fs::path(s);
    }

    fs_file get_temp_file() const { 
        fs::path randPath = fs::unique_path();
        fs::path tmpdir = sandbox_.getTempDir();

        fs::path complete_path = tmpdir / randPath.filename();
        std::cout  << "Temp file path : " << complete_path.string() << std::endl;

        fs_file f(complete_path.string());
        return(f);
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
    bool is_file(const string& p ) const { 
        if ( ! sandbox_.isAllowed(p) ) return false;
        error_code ec; 
        return fs::is_regular_file(p, ec); 
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
        std::cout << "creating directories: " << paths << " result: " << ret 
                  << " ec: " << ec.value() << std::endl;
        return ret;
    }
    bool rmdir(const string& paths) const { 
        return false;
    }
    fs_file open(const string &paths) const { 
        fs_file f(paths);
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(p) ) return f;
        std::cout << "fs_file : single arg func " << std::endl;
        f.open(false);

        return f;
    }
    fs_file create(const string &paths) const { 
        fs_file f(paths);
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(p) ) return f;
        std::cout << "fs_file : create "  << std::endl;
        f.open(true);

        return f;
    }
    long file_size(const string& paths) const { 
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(paths) ) return 0L;
        long l = 0L;
        error_code ec;
        l = fs::file_size(p, ec);
        return l;
    }
    bool remove(const string& paths) const {
        bool rc = false; 
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(paths) ) return false;
        error_code ec;
        rc = fs::remove(p, ec);
        return rc;
    }
    bool remove_all(const string& paths) const { 
        bool rc = false; 
        fs::path p(paths);
        if ( ! sandbox_.isAllowed(paths) ) return false;
        error_code ec;
        rc = fs::remove_all(p, ec);
        return rc;
    }
    fs_directory open_directory(const string& path) const { 
        fs_directory dir; 
        if ( ! sandbox_.isAllowed(path) ) return dir;
        dir.open( fs::path(path) );
        return dir;    
    }
};
