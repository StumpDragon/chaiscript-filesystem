#include <string>
#include <chaiscript/filesystem.hpp> 
#include <catch.hpp> 

using std::string;
namespace cfs = chaiscript::filesystem;
namespace fs  = boost::filesystem;

TEST_CASE("Testing Chaiscript Filesystem Module", "[chaiscript-filesystem]") {
    chaiscript::ChaiScript chai; 
    auto fs_module = chaiscript::ModulePtr(new cfs::fs_module());
    chai.add(fs_module);

    SECTION("Access basic filesystem object") {
        bool a = chai.eval<bool>(R"(var fs = chai_fs(); fs.exists("/a-random-act-of-kindness");)");
        REQUIRE(a == false);
    }

    SECTION("Current working path" ) { 
        cfs::fs_sandbox sbox;
        std::string cwd = sbox.current_path();
        std::string s = chai.eval<std::string>(R"("Hello world")"
                                               //"var fs = chai_fs(); \n"
                                               //"var s = fs.current_path(); \n"
                                               //"print(\"Script Current Path:\" + s;)\n "
                                               //"s;\n"
                                              );
        //REQUIRE( s.length() > 0 );

        //REQUIRE( cwd == s );
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