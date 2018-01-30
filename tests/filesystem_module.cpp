#include <string>
#include <regex>
#include <chaiscript/filesystem.hpp> 
#include <catch.hpp> 

using std::string;
namespace cfs = chaiscript::filesystem;
namespace fs  = boost::filesystem;

#include <iostream>

string normalize(const string& p) { 
    string s = p; 
    std::replace(s.begin(), s.end(), '\\', '/');
    //std::cout << "path normalized: " << s << std::endl;
    return s;
}

TEST_CASE("Testing Chaiscript Filesystem Module", "[chaiscript-filesystem]") {
    chaiscript::ChaiScript chai; 
    cfs::fs_module fs(chai);

    //auto fs_module = chaiscript::ModulePtr(new cfs::fs_module());
    //chai.add(fs_module);

    SECTION("Access basic filesystem object") {
        bool a = chai.eval<bool>(R"(import("chaifs"); return chaifs.exists("/a-random-act-of-kindness"); )");
        REQUIRE(a == false);
    }

    SECTION("Current working path" ) { 
//        cfs::fs_sandbox sbox;
        string cwd = fs::current_path().string();
        std::string s = chai.eval<std::string>(R"(import("chaifs"); chaifs.current_path(); )");
        REQUIRE( cwd.length() == s.length() );
        REQUIRE( cwd == s );
    }

    SECTION("is a directory" ) { 
//        cfs::fs_sandbox sbox;
        fs::path cwd = fs::current_path();
        fs::path p = cwd / "/tmp/sandbox1";
        std::cout << "Path: " << p.string() << std::endl;
        //std::cout << "normalized: " << normalize(p.string()) << std::endl;
        std::string script = R"(import("chaifs"); chaifs.is_directory("$PATH");)";
        //script = std::regex_replace(script, std::regex(R"(\$PATH)"), p.string());
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );
        //std::cout << "Script: " << script << std::endl;
        bool b = chai.eval<bool>( script );
        REQUIRE( false == b );
    }

    SECTION("Test creating an invalid set of directories") { 
        fs::path p("/tmpASDFASDFASDFASDF");
        std::string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE( true == b );
    }
    SECTION("Test creating a valid set of directories") { 
        fs::path cwd = fs::current_path();
        fs::path p = cwd / "tmp/sandbox1/a/b/c/d"; 
        std::string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE( true == b );
    }

}

/*TEST_CASE("Get current path", "[chaiscript-filesystem]") { 
    chaiscript::ChaiScript chai; 
    auto fs_module = chaiscript::ModulePtr(new cfs::fs_module());
    chai.add(fs_module);

    std::string cwd = fs::current_path().string();
    std::string s = chai.eval<std::string>(R"(var fs = chai_fs(); return fs.current_path();)");
    REQUIRE( s.length() > 0 );
}
*/