# __AST2XML__

A simple C++ parser that returns a simplified version of Clang's AST in XML format.

## Installation

Ideally AST2XML would work on any system. But we are still working on it.

### Linux

This has been tested only on Ubuntu 13.

1. Proceed with Clang's build install as stated here:

		http://clang.llvm.org/get_started.html

2. Go to your llvm directory 

		$cd (your llvm directory)/tools/clang/tools

3. Clone the repository at a custom folder 

		$git clone https://github.com/Si1314/AST2XML.git ast2XMLtool

4. Copy the Makefile

5. Go to your build directory 
		
		cd (your build directory)/tools/clang/tools

6. Make a specific directory for the tool (with the same name) 

		$mkdir astXMLtool

7. Paste in it the Makefile

8. Go into the directory 

		$cd astXMLtool

8. Execute make command 

		$make

The tool will be compiled and linked as long as there are no issues

9. Go back to your build directory 

		$cd (your build directory)/Debug + Asserts/build

There should be the tool. As well as the other tools provided in the clang package.

## Contents

###Functions

Takes in consideration the return type, line, parameters and the body of the function declaration. 

		int foo(int a,int b, ... ){
			...
		}

		<function name="dameMayor" type="int" line="3">
    		<params>
        		<param type="int" name="a"/>
       			<param type="int" name="b"/>
       			...
    		</params>
    		<body>
    			...
    		</body>
    	</function>

