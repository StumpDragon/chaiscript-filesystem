#include <cstdio> 
#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <chaiscript/chaiscript_basic.hpp>
#include <chaiscript/utility/utility.hpp>
#include <chaiscript/dispatchkit/bootstrap_stl.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using std::string;

class Test { 
public: 
	Test() {
	}
	std::string test() { 
		return("test"); 
	}
	int value() { 
		return 101; 
	}
	std::string returnString(const std::string& r) { 
		return r; 
	}
	

};

class TestChaiModule : public chaiscript::Module { 
public: 
	TestChaiModule() : chaiscript::Module() {
		chaiscript::utility::add_class<Test>(*this, "Test",
			{ chaiscript::constructor<Test()>(), 
			  chaiscript::constructor<Test(const Test&)>() 
			}, //ctors
			{ 
			   { chaiscript::fun(&Test::test), "test" },
			   { chaiscript::fun(&Test::value), "value" },
			   { chaiscript::fun(&Test::returnString), "returnString" },
			}
		);
	}
};

class ChaiEngine : public chaiscript::ChaiScript { 
        static std::string helloWorld(const std::string &t_name) {
          return "Helloworld (helloword func...) --" + t_name + "!";
        }
	chaiscript::ModulePtr testModule = chaiscript::ModulePtr(new TestChaiModule());

public:
	ChaiEngine() { 
		// TODO: I believe this is no longer necessary with recent versions of ChaiScript.
                //auto chailib = chaiscript::Std_Lib::library(); 
		this->add(chaiscript::fun(&ChaiEngine::helloWorld), "helloWorld");
		this->add(testModule);
	}; 

};



TEST_CASE( "testing basic chai file", "[file-tests]" ) {
  //chaiscript::ChaiScript chai;
  ChaiEngine chai;

  chai.eval_file("test.chai"); 

}

TEST_CASE( "testing Test Module", "[eval-tests]" ) {
   ChaiEngine chai; 

   int i = chai.eval<int>("var t = Test(); t.value();");
   REQUIRE(i == 101);
}

TEST_CASE( "Testing string method", "[eval-tests]" ) { 
	ChaiEngine chai; 

	std::string r = chai.eval<std::string>("var t = Test(); t.returnString(\"asdf\");");
	REQUIRE(r.length() == 4 );
}