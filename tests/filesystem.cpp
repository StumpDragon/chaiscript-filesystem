#include <string>
#include <chaiscript/filesystem.hpp> 
#include <catch.hpp> 

using std::string;
namespace cfs = chaiscript::filesystem;

TEST_CASE("Create Filesystem Model", "[core-fs-tests]") {
    cfs::fs_sandbox fsbox; 

    fsbox.add_path("/tmp/sandbox1"); 
    fsbox.add_path("/tmp/sandbox2");
    fsbox.add_path("/tmp/user123/sandbox");


    SECTION("Testing childpath /etc/passwd") { 
    	REQUIRE(false == fsbox.isAllowed((string)"/etc/passwd") );
    }
    SECTION("Testing valid pathname /tmp/sandbox1/asdf") {
    	REQUIRE(true == fsbox.isAllowed((string)"/tmp/sandbox1/asdf"));
    }
    SECTION("Testing valid pathname /tmp/sandbox1/asdf") {
    	REQUIRE(true == fsbox.isAllowed((string)"/tmp/sandbox1/../../tmp/sandbox1/asdf"));
    }
    SECTION("Testing invalid path /tmp/.../etc/passwd") {
    	REQUIRE(false == fsbox.isAllowed((string)"/tmp/sandbox1/../../../asdf"));
    }

}