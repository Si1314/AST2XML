xml-tree-tool
=============

Clang-TinyXML based AST parsing tool

How to install
=============

LINUX

1.- Proceed with Clang's build install as stated here:
2.- Go to your llvm directory >>>>> cd (your llvm directory)/tools/clang/tools
3.- Clone the repository at a custom folder >>>>> git clone https://github.com/Si1314/xml-tree-tool.git astXMLtool
4.- Copy the Makefile
5.- Go to your build directory >>>>> cd (your build directory)/tools/clang/tools
6.- Make a specific directory for the tool (with the same name) >>>>> mkdir astXMLtool
7.- Paste in it the Makefile
8.- Go into the directory >>>>> cd astXMLtool
8.- Execute make command >>>> make

The tool will be compiled and linked as long as there are no issues

9.- Go back to your build directory >>> cd (your build directory)/Debug + Asserts/build

There shall be the tool. As well as the other tools provided in the clang package.
