CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

include("cmake/HunterGate.cmake")

# 1. Try to detect hunter by:
#   * cmake variable
#   * environment variable
#   * in HOME (`.hunter` directory)
#   * in SYSTEMDRIVE (`.hunter` directory, windows only)
#   * in USERPROFILE (`.hunter` directory, windows only)
# 2. if failed, download it
# 3. set HUNTER_GATE_* variables
# 4. include master file: 'include(${HUNTER_SELF}/cmake/Hunter)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.227.tar.gz"
    SHA1 "808b778a443fcdf19c2d18fea8fa4bb59d16596a"
)


PROJECT (chaiscript-filesystem)

hunter_add_package(Boost COMPONENTS system filesystem) 
find_package(Boost CONFIG REQUIRED system filesystem)  

hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

INCLUDE_DIRECTORIES(
	libs/chaiscript/include
	include
   ) 


SET(TEST_SRCS
       	tests/chai.cpp 
       	tests/filesystem_sandbox.cpp
       	tests/filesystem_module.cpp
   ) 

if(MINGW OR CYGWIN) 
    set_property(SOURCE tests/chai.cpp APPEND PROPERTY COMPILE_FLAGS "-O3" ) 
    set_property(SOURCE tests/filesystem_module.cpp APPEND PROPERTY COMPILE_FLAGS "-O3" ) 
    #set_property(SOURCE tests/chai.cpp APPEND PROPERTY COMPILE_FLAGS "-Wa,-mbig-obj" ) 

  #add_definitions(-O3)
endif()
if(CMAKE_COMPILER_IS_GNUXX)
    #ENABLE Experimental Filesystem support
    ADD_DEFINITIONS(-lstdc++fs)
endif()

#ADD_DEFINITIONS(-std=c++14)
ADD_EXECUTABLE(chai ${TEST_SRCS}) 
target_link_libraries(chai Catch::Catch Boost::system Boost::filesystem)


enable_testing()

add_test(chai-test1 chai)
