#!/bin/sh

java ../tools/REx -backtrack -javascript -tree -main grammar/Cpp.ebnf
mv -f Cpp.js Cpp-main.js
java ../tools/REx -backtrack -javascript -tree grammar/Cpp.ebnf
java -jar ../tools/rr.war grammar/Cpp.ebnf > Cpp.xhtml