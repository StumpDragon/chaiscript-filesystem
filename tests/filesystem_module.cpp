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
        string s = chai.eval<string>(R"(import("chaifs"); chaifs.current_path(); )");
        REQUIRE( cwd.length() == s.length() );
        REQUIRE( cwd == s );
    }

    SECTION("is a directory" ) { 
//        cfs::fs_sandbox sbox;
        fs::path cwd = fs::current_path();
        fs::path p = cwd / "/tmp/sandbox1";
        std::cout << "Path: " << p.string() << std::endl;
        //std::cout << "normalized: " << normalize(p.string()) << std::endl;
        string script = R"(import("chaifs"); chaifs.is_directory("$PATH");)";
        //script = std::regex_replace(script, std::regex(R"(\$PATH)"), p.string());
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );
        std::cout << "Script: " << script << std::endl;
        bool b = chai.eval<bool>( script );
        REQUIRE( false == b );
    }

    SECTION("Test creating an invalid set of directories") { 
        fs::path p("/tmpASDFASDFASDFASDF");
        string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
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
        string script = R"(import("chaifs"); chaifs.create_directories("$PATH");)"; 
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
        string script = R"(import("chaifs"); var f = chaifs.open("$PATH"); f.is_valid();)"; 
            script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE(false == b);
    }
    SECTION("Test open file - create" ) { 
        fs::path p = tmpdir / "test-create.txt";
        string script = R"(
                    import("chaifs"); 
                    var f = chaifs.create("$PATH"); 
                    f.writeline("This is a line to send to a file");
                    f.is_valid();
                )"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        bool b = chai.eval<bool>(script);
        REQUIRE(true == b);
    }

}

TEST_CASE("Testing file size", "[chaiscript-filesystem]") {
    chaiscript::ChaiScript chai; 
    cfs::fs_module fs(chai);

    fs::path tmpdir = fs::current_path();
    tmpdir = tmpdir / "/tmp";
    fs.sandbox().add_path(tmpdir);
    fs::path p = tmpdir / "test-filesize.txt";

    SECTION("file size") { 
        string script = R"(
                import("chaifs"); 
                var f = chaifs.create("$PATH"); 
                f.writeline("hello world");
                f.close();
                var l = fs.file_size("$PATH");
                return l;
           )";
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "SCript: " << script << std::endl;
        long val = chai.eval<long>(script);
        REQUIRE(val == 12);
    }

}

TEST_CASE("Testing writeline/eachline", "[chaiscript-filesystem]") {
    chaiscript::ChaiScript chai; 
    cfs::fs_module fs(chai);

    fs::path tmpdir = fs::current_path();
    tmpdir = tmpdir / "/tmp";
    fs.sandbox().add_path(tmpdir);
    fs::path p = tmpdir / "test-integers.txt";

    SECTION("writeline - create" ) { 
        string script = R"(
                    import("chaifs"); 
                    var f = chaifs.create("$PATH"); 
                    auto count = 0;
                    count += f.writeline("100");
                    count += f.writeline("200");
                    count += f.writeline("400");
                    count += f.writeline("800");
                    return count;
                )"; 
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "SCript: " << script << std::endl;
        int val = chai.eval<int>(script);
        REQUIRE(val >= 12);
    }

    SECTION("eachline - compute values") { 
        string script = R"(
             import("chaifs");
             var fs = chaifs.open("$PATH");
             var count = 0;
             auto computeLine = fun[count](s) { 
                 var val = s.to_int(); 
                 count += val; 
             }
             fs.eachline( computeLine );
             return count;
            )";
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "SCript: " << script << std::endl;
        auto val = chai.eval<int>(script);
        REQUIRE(val == 1500);
    }
    SECTION("readline - compute values") { 
        string script = R"(
             import("chaifs");
             var fs = chaifs.open("$PATH");
             var count = 0;
             while ( !fs.eof() ) { 
                auto s = fs.readline(); 
                if ( s.size() > 0 ) { 
                    var val = s.to_int();
                    count += val;
                }
             }
             return count;
            )";
        script = std::regex_replace(script, std::regex(R"(\$PATH)"), normalize(p.string()) );  
        std::cout << "SCript: " << script << std::endl;
        auto val = chai.eval<int>(script);
        REQUIRE(val == 1500);
    }

}