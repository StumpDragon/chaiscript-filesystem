Master Status: [![Linux Build Status](https://travis-ci.org/StumpDragon/chaiscript-filesystem.png?branch=master)](https://travis-ci.org/StumpDragon/chaiscript-filesystem) 

# chaiscript-filesystem
-----------------------

ChaiScript is a lightweight scripting library for C++14 and greater.  It allows you to create custom code objects and functions, but out of the box comes with almost no library support.  This project aims to add filesystem support for the library. 

One of the key features of this project is sandboxing.  The module allows you to specify directories that limit the access to the filesystem to the scripting language.  This would become useful if the application chooses to allow end-users to provide scripts of their own creation, that may attempt malicious actions (accessing /etc/passwd, rm -rf /, etc). 

### Requirements: 
--------------
* ChaiScript - currently only the develop stream
* Boost Filesystem - 

### Using: 
--------------



### Example: 
------------

Open a file: 

```
import chaifs; 
var f = chaifs.create("/tmp/example.txt"); 

```
