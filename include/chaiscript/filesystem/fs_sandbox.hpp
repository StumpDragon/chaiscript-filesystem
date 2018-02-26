#pragma once

class fs_sandbox { 
	vector<fs::path> sandbox_paths; 
    fs::path         sandbox_tmpdir;

public:
	fs_sandbox() { }
    fs_sandbox(string p) { 
        /*for ( auto p : paths ) { 
            sandbox_paths.push_back( fs::path(p) );
        }*/
        sandbox_paths.push_back( fs::path(p) );
    }
    bool setTempDir(const fs::path& p) { 
        error_code ec;
        auto cp = fs::canonical(p, ec); 
        if ( ENOENT != ec.value()) { 
            sandbox_tmpdir = p;
            return true;
        }
        return false;
    }
    fs::path getTempDir() const { return sandbox_tmpdir; }
	error_code add_path(const fs::path& p) {
        error_code ec;
        /* use canonical here vs lexically_normal, we want to store the canonical path,
           and because it's a sandbox, it *SHOULD* probably already exist.
         */
        auto cp = fs::canonical(p, ec); 
        if ( ENOENT != ec.value()) { 
        	std::cout << "Adding sandbox path: " << cp << std::endl;
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
