# __AST2XML__

A simple C++ parser that returns a simplified version of Clang's AST in XML format.

###Function declaration

Takes into account the return type, line, parameters and the body of the function declaration. 

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

Takes into account the variable type, line and the initialization expresion.

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

###Assignments & Operators

Assignments take into account the name of the recipient, the expresion to be assigned and the code position. Also the type of the operation and the operator type if needed.

Operators supported are:

        + , - , / , * , ++ , -- , += , -= , /= , *= , && , || , ! , +(sign) , -(sign)
---
        a = 1;
        b += a;
        c++;
        d = -c;
        e = !f;
---
        <assignment name="a" line="72">
            <const value="1"/>
        </assignment>
        <assignmentOperator name="b" type="arithmetic" operator="+" line="73">
            <variable name="a"/>
        </assignmentOperator>
        <unaryOperator name="c" operator="+" line="74"/>
        <assignment name="d" line="75">
            <signOperator type="-">
                <variable name="c"/>
            </signOperator>
        </assignment>
        <assignment name="e" line="76">
            <notOperator>
                <variable name="f"/>
            </notOperator>
        </assignment>


###If 

Takes into account the condition expresion, the **then** and/or **else** structure and the code position.

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

###While loop

Takes into account the condition expresion, the body of the loop and the code position.

        while(a>0){
            ...
        }
---
        <while line="15">
            <binaryOperator type="comparison" operator="&gt;">
                <variable name="a"/>
                <const value="0"/>
            </binaryOperator>
            <body>
                ...
            </body>
        </while>

###For loop

Takes into account code position, the initialization of the control variable, the control variable's condition and step and the loop's body. 

        for(int i=0;i<10;i++){
            ...
        }
---
        <for line="135">
            <declarations>
                <declaration type="int" name="i" line="135">
                    <const value="0"/>
                </declaration>
            </declarations>
            <binaryOperator type="comparison" operator="&lt;">
                <variable name="i"/>
                <const value="10"/>
            </binaryOperator>
            <unaryOperator name="i" operator="+" line="135"/>
            <body>
                ...
            </body>
        </for>

###Calls

Functions behave like expresions. Takes into account the name of the function, the type and the arguments required.

        a = foo(b,c);
---
        <assignment name="a" line="94">
            <callFunction name="foo" type="int">
                <arg>
                    <variable name="b"/>
                </arg>
                <arg>
                    <variable name="c"/>
                </arg>
            </callFunction>
        </assignment>

###Builtins

In order to work with the project 
        https://github.com/si1314/PFC 
there are included some builtin functions to mask i/o behaviours. Those are the ones included in the files 
        BuiltinsIO.h
        BuiltinsSTD.h

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

##User manual

In order to execute the tool use this command:

        $./ast2xml <C++file> -- <XMLfile>