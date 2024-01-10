# Compiler_WLP4
A mini compiler for the WLP4 language

WLP4 is a subset of C++ which stands for “Waterloo, Language, Plus, Pointers, Plus, Procedures”.
This language includes functions, integers, pointers, arrays, conditional statements, and while loops and is used in Waterloo's CS241 Course.

For more info see:
1. https://student.cs.uwaterloo.ca/~cs241/wlp4/WLP4tutorial.html
2. https://student.cs.uwaterloo.ca/~cs241/wlp4/WLP4.html

Compiler Steps:
1) Scanning
Tokenizes/scans given WLP4 code which implemented through the Simplified Maximal Munch Algorithm (consume input until it gets "stuck", at which point it is determined whether or not the input is in an accepting state. If in an accepting state, a token will be produced. Otherwise, the input will be rejected. The algorithm scanned through the code uses an NFA, for which the lexical syntax used can be found here).

2) Parsing
Parsing is implemented using Pushdown Automata to determine if the code can be recognized using a Context Free Grammar (CFG). The context-free syntax of WLP4 can be found here. The final product upon parsing a CFG generates a derivation of the input string, which uniquely defines a parse tree used to represent the structure of the program. For the purposes of this compiler, a Bottom-Up Parsing algorithm is used, specifically the SLR(1) parser.

3) Context-Sensitive Analysis
The next step of the compiler is to determie if the code follows the context-sensitive rules of WLP4. Examples of common rules used by popular languages are:

Not declaring multiple variables with the same name
Not using a variable prior to its declaration
The full set of semantic/type-inference rules can be found here, and the full set of context-sensitive rules can be found here. To accomplish this, the compiler parses through the parse tree generated from the last step to ensure each rule is followed.

4) Code Generation
The final part of the compilation process is code generation, and completing the process of translating WLP4 source code to MIPS assembly language. Basic features are supported including code generation for the main procedure, integer variables and constants, declarations, assignment, arithmetic, control flow, and printing, and additional support for pointers, other procedures, and dynamic memory allocation is also implemented.
