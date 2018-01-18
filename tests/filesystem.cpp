#include <string>
#include <chaiscript/filesystem.hpp> 
#include <catch.hpp> 

using std::string;
namespace cfs = chaiscript::filesystem;
namespace fs  = boost::filesystem;

TEST_CASE("Create Filesystem Model", "[core-fs-tests]") {
    cfs::fs_sandbox fsbox; 

    auto cwd = fs::current_path(); 
    fs::create_directories( cwd / "tmp/sandbox1" );
    fs::create_directories( cwd / "tmp/sandbox2" );
    fs::create_directories( cwd / "tmp/user123/sandbox" );
    
    fsbox.add_path(cwd / "/tmp/sandbox1"); 
    fsbox.add_path(cwd / "/tmp/sandbox2");
    fsbox.add_path(cwd / "/tmp/user123/sandbox");


    SECTION("Testing childpath /etc/passwd") { 
    	REQUIRE(false == fsbox.isAllowed("/etc/passwd") );
    }
    SECTION("Testing valid pathname /tmp/sandbox1/asdf") {
    	REQUIRE(true == fsbox.isAllowed(cwd / "tmp/sandbox1/asdf"));
    }
    SECTION("Testing valid pathname /tmp/sandbox1/asdf") {
    	REQUIRE(true == fsbox.isAllowed(cwd / "tmp/sandbox1/../../tmp/sandbox1/asdf"));
    }
    SECTION("Testing invalid path /tmp/.../etc/passwd") {
    	REQUIRE(false == fsbox.isAllowed(cwd / "tmp/sandbox1/../../../../../asdf"));
    }

}