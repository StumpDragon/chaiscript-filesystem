#include <string>
#include <regex>
#include <chaiscript/filesystem.hpp> 

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


using std::string;
namespace cfs = chaiscript::filesystem;
namespace fs  = boost::filesystem;

#include <iostream>
#include <fstream>

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
        std::cout << "Script: " << script << std::endl;
        bool b = chai.eval<bool>( script );
        REQUIRE( false == b );
    }

    SECTION("Test creating an invalid set of directories") { 
        fs::path p("/tmpASDFASDFASDFASDF");
        std::string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "Script: " << script << std::endl;
        bool b = chai.eval<bool>(script);
        REQUIRE( false == b );
    }
    SECTION("Test creating a valid set of directories") { 
        fs::path tmp = fs::current_path();
        tmp = tmp / "tmp";
        fs.sandbox().add_path(tmp);
        fs::path p = tmp / "sandbox1/a/b/c/d"; 
        std::string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "Script: " << script << std::endl;
        bool b = chai.eval<bool>(script);
        REQUIRE( true == b );
        REQUIRE( true == fs::is_directory(p) );
    }

}

TEST_CASE("fs_file operations", "[chaiscript-filesystem]") { 
    chaiscript::ChaiScript chai; 
    cfs::fs_module fs(chai);

    fs::path tmpdir = fs::current_path();
    tmpdir = tmpdir / "/tmp";
    fs.sandbox().add_path(tmpdir);

    SECTION("Test open file - not present") {
        fs::path p = tmpdir / "test-open.txt";
        std::string script = R"(import("chaifs"); var f = chaifs.open("$PATH"); f.is_valid();)"; 
            script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE(false == b);
    }
    SECTION("Test open file - create" ) { 
        fs::path p = tmpdir / "test-create.txt";
        std::string script = R"(
                    import("chaifs"); 
                    var f = chaifs.create("$PATH"); 
                    f.writeline("This is a line to send to a file");
                    f.is_valid();
                )"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE(true == b);
    }

    /* Debugging for strange fstream   ios::in|ios::out behavior
    SECTION("Test open file - fstream") {
        fs::path p = tmpdir / "test-fstream.txt";
        std::string s = normalize(p.string());
        s = std::regex_replace(s, std::regex("[A-Z]\\:"), "");
        std::cout << "fstream path: " << s << std::endl;
        std::fstream f; 
        f.open(s, std::ios::in|std::ios::out|std::ios::binary);
        f << "Test" << std::endl;
        std::ofstream f1;
        f1.open("c:\\temp\\asdf.txt", std::ios::binary);
        f1 << "Test text" << std::endl;
        std::fstream f2("c:/temp/asdfasdf2.txt", std::ios::out|std::ios::binary);
        //f2.open("c:\\temp\\asdf2.txt", std::fstream::in|std::fstream::out|std::fstream::binary);
        f2 << "Test text" << std::endl;
    }
    */
}
