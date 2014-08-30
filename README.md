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

###Function declaration

Takes in consideration the return type, line, parameters and the body of the function declaration. 

        int foo(int a,int b, ... ){
            ...
        }
 ---
        <function name="foo" type="int" line="3">
            <params>
                <param type="int" name="a"/>
                <param type="int" name="b"/>
                ...
            </params>
            <body>
                ...
            </body>
        </function>

###Variable declaration

Takes in consideration the variable type, line and initialization expresion.

        int a,b;
        int c = 1;
---
        <declarations>
            <declaration type="int" name="a" line="3"/>
            <declaration type="int" name="b" line="3"/>
        </declarations>
        <declarations>
            <declaration type="int" name="c" line="4">
                <const value="1"/>
            </declaration>
        </declarations>

###Assignments

Takes in consideration name of the recipient, expresion to be assigned and code position. Also the type of the operation and the operator type if needed.

        a = 1;
        c += d;
---
        <assignment name="a" line="72">
            <const value="1"/>
        </assignment>
        <assignmentOperator name="c" type="arithmetic" operator="+" line="73">
            <variable name="d"/>
        </assignmentOperator>

###If structure

Takes in consideration the condition expresion, **then** and/or **else** structure and code position.

        if(a==0){
            ...
        }else{
            ...
        }
---
        <if line="6">
            <binaryOperator type="comparison" operator="==">
                <variable name="a"/>
                <const value="0"/>
            </binaryOperator>
            <then>
                <body>
                ...
                </body>
            </then>
            <else>
                <body>
                ...
                </body>
            </else>
        </if>