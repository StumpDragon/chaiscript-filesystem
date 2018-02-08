#include <string>
#include <chaiscript/filesystem.hpp> 

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


using std::string;
namespace cfs = chaiscript::filesystem;
namespace fs  = boost::filesystem;

cfs::fs_sandbox makeSandbox() { 
    cfs::fs_sandbox fsbox; 

    auto cwd = fs::current_path(); 
    fs::create_directories( cwd / "tmp/sandbox1" );
    fs::create_directories( cwd / "tmp/sandbox2" );
    fs::create_directories( cwd / "tmp/user123/sandbox" );
    
    fsbox.add_path(cwd / "/tmp/sandbox1"); 
    fsbox.add_path(cwd / "/tmp/sandbox2");
    fsbox.add_path(cwd / "/tmp/user123/sandbox");

    return fsbox;
}

TEST_CASE("Create Filesystem Model", "[core-sbox-tests]") {
    cfs::fs_sandbox fsbox = makeSandbox();
    auto cwd = fs::current_path(); 

    SECTION("Test adding root directory" ) { 
        auto ec = fsbox.add_path("/"); 
        REQUIRE( 0 == ec.value() );
    }
    SECTION("Testing sandbox - adding a bad directory") {
        auto ec = fsbox.add_path("/f572d396fae9206628714fb2ce00f72e94f2258f"); 
        REQUIRE( ENOENT == ec.value() );
    }
 
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

TEST_CASE("Empty sandbox", "[core-sbox-tests]") { 
    cfs::fs_sandbox fsbox; 

    SECTION("Testing root directory") { 
        REQUIRE(false == fsbox.isAllowed("/") ); 
    }

}
TEST_CASE("Sandbox - no path entries", "[core-sbox-tests]") { 
    cfs::fs_sandbox fsbox; 

    SECTION("Testing root directory - no paths should deny all") { 
        REQUIRE(false == fsbox.isAllowed("/") ); 
    }
}

TEST_CASE("Root sandbox", "[core-sbox-tests]") {
    cfs::fs_sandbox fsbox( "/" );
    SECTION("Testing root directory - should allow root directory") { 
        REQUIRE(false == fsbox.isAllowed("/") ); 
    }
}



/********************
 * Do not test file operations on fs_sandbox - leave that to the module test cases.
 ********************
TEST_CASE("Misc file operations", "[core-fs-tests]") {
    cfs::fs_sandbox fsbox = makeSandbox();
    auto cwd = fs::current_path(); 

    SECTION("Testing file exists") {
        fs::path p ( cwd / "/tmp/sandbox1" ); 
        REQUIRE( false == fsbox.exists(p) );    
        REQUIRE( false == fsbox.is_directory(p) );    
    }
    SECTION("Testing file not exists") { 
        REQUIRE( false == fsbox.exists( fs::path("/f572d396fae9206628714fb2ce00f72e94f2258f")) );
    }
}
*/