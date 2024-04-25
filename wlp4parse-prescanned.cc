#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <deque>
#include <memory>
#include <variant>
#include <cassert>
#include <unordered_set>

using namespace std;

const std::string WLP4_CFG = R"END(.CFG
start BOF procedures EOF
procedures procedure procedures
procedures main
procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
params .EMPTY
params paramlist
paramlist dcl
paramlist dcl COMMA paramlist
type INT
type INT STAR
dcls .EMPTY
dcls dcls dcl BECOMES NUM SEMI
dcls dcls dcl BECOMES NULL SEMI
dcl type ID
statements .EMPTY
statements statements statement
statement lvalue BECOMES expr SEMI
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
statement WHILE LPAREN test RPAREN LBRACE statements RBRACE
statement PRINTLN LPAREN expr RPAREN SEMI
statement DELETE LBRACK RBRACK expr SEMI
test expr EQ expr
test expr NE expr
test expr LT expr
test expr LE expr
test expr GE expr
test expr GT expr
expr term
expr expr PLUS term
expr expr MINUS term
term factor
term term STAR factor
term term SLASH factor
term term PCT factor
factor ID
factor NUM
factor NULL
factor LPAREN expr RPAREN
factor AMP lvalue
factor STAR factor
factor NEW INT LBRACK expr RBRACK
factor ID LPAREN RPAREN
factor ID LPAREN arglist RPAREN
arglist expr
arglist expr COMMA arglist
lvalue ID
lvalue STAR factor
lvalue LPAREN lvalue RPAREN
)END";

const std::string WLP4_TRANSITIONS = R"END(.TRANSITIONS
0 BOF 45
1 AMP 35
1 ID 13
1 LPAREN 1
1 NEW 30
1 NULL 33
1 NUM 10
1 STAR 6
1 expr 86
1 factor 83
1 term 51
100 EQ 40
100 GE 37
100 GT 46
100 LE 34
100 LT 27
100 MINUS 3
100 NE 14
100 PLUS 74
102 AMP 35
102 ID 13
102 LPAREN 1
102 NEW 30
102 NULL 33
102 NUM 10
102 STAR 6
102 expr 11
102 factor 83
102 term 51
103 AMP 35
103 ID 13
103 LPAREN 1
103 NEW 30
103 NULL 33
103 NUM 10
103 RPAREN 44
103 STAR 6
103 arglist 125
103 expr 38
103 factor 83
103 term 51
104 AMP 35
104 ID 13
104 LPAREN 1
104 NEW 30
104 NULL 33
104 NUM 10
104 STAR 6
104 expr 72
104 factor 83
104 term 51
105 AMP 35
105 ID 13
105 LPAREN 1
105 NEW 30
105 NULL 33
105 NUM 10
105 STAR 6
105 factor 55
108 DELETE 57
108 ID 19
108 IF 53
108 LPAREN 4
108 PRINTLN 2
108 RBRACE 119
108 STAR 73
108 WHILE 43
108 lvalue 12
108 statement 20
109 DELETE 57
109 ID 19
109 IF 53
109 LPAREN 4
109 PRINTLN 2
109 RBRACE 120
109 STAR 73
109 WHILE 43
109 lvalue 12
109 statement 20
11 MINUS 3
11 PLUS 74
11 SEMI 23
110 DELETE 57
110 ID 19
110 IF 53
110 LPAREN 4
110 PRINTLN 2
110 RBRACE 121
110 STAR 73
110 WHILE 43
110 lvalue 12
110 statement 20
111 AMP 35
111 ID 13
111 LPAREN 1
111 NEW 30
111 NULL 33
111 NUM 10
111 STAR 6
111 factor 54
113 LBRACE 77
114 SEMI 69
115 INT 68
115 main 28
115 procedure 115
115 procedures 16
118 AMP 35
118 ID 13
118 LPAREN 1
118 NEW 30
118 NULL 33
118 NUM 10
118 STAR 6
118 arglist 65
118 expr 38
118 factor 83
118 term 51
119 ELSE 113
12 BECOMES 97
122 INT 17
122 dcl 15
122 paramlist 123
122 params 18
122 type 48
124 INT 17
124 dcl 63
124 statements 42
124 type 48
125 RPAREN 24
126 LBRACK 79
128 LPAREN 84
129 EOF 107
13 LPAREN 103
131 LPAREN 122
14 AMP 35
14 ID 13
14 LPAREN 1
14 NEW 30
14 NULL 33
14 NUM 10
14 STAR 6
14 expr 88
14 factor 83
14 term 51
15 COMMA 99
17 STAR 95
18 RPAREN 36
2 LPAREN 9
23 RBRACE 41
25 LBRACE 61
27 AMP 35
27 ID 13
27 LPAREN 1
27 NEW 30
27 NULL 33
27 NUM 10
27 STAR 6
27 expr 89
27 factor 83
27 term 51
29 DELETE 57
29 ID 19
29 IF 53
29 LPAREN 4
29 PRINTLN 2
29 RETURN 102
29 STAR 73
29 WHILE 43
29 lvalue 12
29 statement 20
3 AMP 35
3 ID 13
3 LPAREN 1
3 NEW 30
3 NULL 33
3 NUM 10
3 STAR 6
3 factor 83
3 term 60
30 INT 126
31 LBRACE 80
32 LBRACE 81
34 AMP 35
34 ID 13
34 LPAREN 1
34 NEW 30
34 NULL 33
34 NUM 10
34 STAR 6
34 expr 90
34 factor 83
34 term 51
35 ID 19
35 LPAREN 4
35 STAR 73
35 lvalue 93
36 LBRACE 5
37 AMP 35
37 ID 13
37 LPAREN 1
37 NEW 30
37 NULL 33
37 NUM 10
37 STAR 6
37 expr 91
37 factor 83
37 term 51
38 COMMA 118
38 MINUS 3
38 PLUS 74
39 RBRACE 106
4 ID 19
4 LPAREN 4
4 STAR 73
4 lvalue 94
40 AMP 35
40 ID 13
40 LPAREN 1
40 NEW 30
40 NULL 33
40 NUM 10
40 STAR 6
40 expr 87
40 factor 83
40 term 51
42 DELETE 57
42 ID 19
42 IF 53
42 LPAREN 4
42 PRINTLN 2
42 RETURN 52
42 STAR 73
42 WHILE 43
42 lvalue 12
42 statement 20
43 LPAREN 8
45 INT 68
45 main 28
45 procedure 115
45 procedures 129
46 AMP 35
46 ID 13
46 LPAREN 1
46 NEW 30
46 NULL 33
46 NUM 10
46 STAR 6
46 expr 92
46 factor 83
46 term 51
48 ID 101
49 RPAREN 31
5 dcls 98
50 RPAREN 32
51 PCT 67
51 SLASH 105
51 STAR 111
52 AMP 35
52 ID 13
52 LPAREN 1
52 NEW 30
52 NULL 33
52 NUM 10
52 STAR 6
52 expr 64
52 factor 83
52 term 51
53 LPAREN 7
57 LBRACK 78
58 SEMI 117
59 PCT 67
59 SLASH 105
59 STAR 111
6 AMP 35
6 ID 13
6 LPAREN 1
6 NEW 30
6 NULL 33
6 NUM 10
6 STAR 6
6 factor 26
60 PCT 67
60 SLASH 105
60 STAR 111
61 dcls 124
62 NULL 58
62 NUM 82
63 BECOMES 62
64 MINUS 3
64 PLUS 74
64 SEMI 39
67 AMP 35
67 ID 13
67 LPAREN 1
67 NEW 30
67 NULL 33
67 NUM 10
67 STAR 6
67 factor 56
68 ID 131
68 WAIN 128
7 AMP 35
7 ID 13
7 LPAREN 1
7 NEW 30
7 NULL 33
7 NUM 10
7 STAR 6
7 expr 100
7 factor 83
7 term 51
7 test 49
70 MINUS 3
70 PLUS 74
70 SEMI 22
71 MINUS 3
71 PLUS 74
71 RPAREN 114
72 MINUS 3
72 PLUS 74
72 SEMI 21
73 AMP 35
73 ID 13
73 LPAREN 1
73 NEW 30
73 NULL 33
73 NUM 10
73 STAR 6
73 factor 66
74 AMP 35
74 ID 13
74 LPAREN 1
74 NEW 30
74 NULL 33
74 NUM 10
74 STAR 6
74 factor 83
74 term 59
75 COMMA 96
76 RPAREN 25
77 statements 110
78 RBRACK 104
79 AMP 35
79 ID 13
79 LPAREN 1
79 NEW 30
79 NULL 33
79 NUM 10
79 STAR 6
79 expr 85
79 factor 83
79 term 51
8 AMP 35
8 ID 13
8 LPAREN 1
8 NEW 30
8 NULL 33
8 NUM 10
8 STAR 6
8 expr 100
8 factor 83
8 term 51
8 test 50
80 statements 108
81 statements 109
82 SEMI 116
84 INT 17
84 dcl 75
84 type 48
85 MINUS 3
85 PLUS 74
85 RBRACK 47
86 MINUS 3
86 PLUS 74
86 RPAREN 112
87 MINUS 3
87 PLUS 74
88 MINUS 3
88 PLUS 74
89 MINUS 3
89 PLUS 74
9 AMP 35
9 ID 13
9 LPAREN 1
9 NEW 30
9 NULL 33
9 NUM 10
9 STAR 6
9 expr 71
9 factor 83
9 term 51
90 MINUS 3
90 PLUS 74
91 MINUS 3
91 PLUS 74
92 MINUS 3
92 PLUS 74
94 RPAREN 130
96 INT 17
96 dcl 76
96 type 48
97 AMP 35
97 ID 13
97 LPAREN 1
97 NEW 30
97 NULL 33
97 NUM 10
97 STAR 6
97 expr 70
97 factor 83
97 term 51
98 INT 17
98 dcl 63
98 statements 29
98 type 48
99 INT 17
99 dcl 15
99 paramlist 127
99 type 48
)END";

const std::string WLP4_REDUCTIONS = R"END(.REDUCTIONS
10 36 BECOMES
10 36 COMMA
10 36 EQ
10 36 GE
10 36 GT
10 36 LE
10 36 LT
10 36 MINUS
10 36 NE
10 36 PCT
10 36 PLUS
10 36 RBRACK
10 36 RPAREN
10 36 SEMI
10 36 SLASH
10 36 STAR
101 14 BECOMES
101 14 COMMA
101 14 RPAREN
106 4 EOF
107 0 .ACCEPT
112 38 BECOMES
112 38 COMMA
112 38 EQ
112 38 GE
112 38 GT
112 38 LE
112 38 LT
112 38 MINUS
112 38 NE
112 38 PCT
112 38 PLUS
112 38 RBRACK
112 38 RPAREN
112 38 SEMI
112 38 SLASH
112 38 STAR
116 12 DELETE
116 12 ID
116 12 IF
116 12 INT
116 12 LPAREN
116 12 PRINTLN
116 12 RETURN
116 12 STAR
116 12 WHILE
117 13 DELETE
117 13 ID
117 13 IF
117 13 INT
117 13 LPAREN
117 13 PRINTLN
117 13 RETURN
117 13 STAR
117 13 WHILE
120 19 DELETE
120 19 ID
120 19 IF
120 19 LPAREN
120 19 PRINTLN
120 19 RBRACE
120 19 RETURN
120 19 STAR
120 19 WHILE
121 18 DELETE
121 18 ID
121 18 IF
121 18 LPAREN
121 18 PRINTLN
121 18 RBRACE
121 18 RETURN
121 18 STAR
121 18 WHILE
122 5 RPAREN
123 6 RPAREN
124 15 DELETE
124 15 ID
124 15 IF
124 15 LPAREN
124 15 PRINTLN
124 15 RBRACE
124 15 RETURN
124 15 STAR
124 15 WHILE
127 8 RPAREN
13 35 BECOMES
13 35 COMMA
13 35 EQ
13 35 GE
13 35 GT
13 35 LE
13 35 LT
13 35 MINUS
13 35 NE
13 35 PCT
13 35 PLUS
13 35 RBRACK
13 35 RPAREN
13 35 SEMI
13 35 SLASH
13 35 STAR
130 48 BECOMES
130 48 COMMA
130 48 EQ
130 48 GE
130 48 GT
130 48 LE
130 48 LT
130 48 MINUS
130 48 NE
130 48 PCT
130 48 PLUS
130 48 RBRACK
130 48 RPAREN
130 48 SEMI
130 48 SLASH
130 48 STAR
15 7 RPAREN
16 1 EOF
17 9 ID
19 46 BECOMES
19 46 COMMA
19 46 EQ
19 46 GE
19 46 GT
19 46 LE
19 46 LT
19 46 MINUS
19 46 NE
19 46 PCT
19 46 PLUS
19 46 RBRACK
19 46 RPAREN
19 46 SEMI
19 46 SLASH
19 46 STAR
20 16 DELETE
20 16 ID
20 16 IF
20 16 LPAREN
20 16 PRINTLN
20 16 RBRACE
20 16 RETURN
20 16 STAR
20 16 WHILE
21 21 DELETE
21 21 ID
21 21 IF
21 21 LPAREN
21 21 PRINTLN
21 21 RBRACE
21 21 RETURN
21 21 STAR
21 21 WHILE
22 17 DELETE
22 17 ID
22 17 IF
22 17 LPAREN
22 17 PRINTLN
22 17 RBRACE
22 17 RETURN
22 17 STAR
22 17 WHILE
24 43 BECOMES
24 43 COMMA
24 43 EQ
24 43 GE
24 43 GT
24 43 LE
24 43 LT
24 43 MINUS
24 43 NE
24 43 PCT
24 43 PLUS
24 43 RBRACK
24 43 RPAREN
24 43 SEMI
24 43 SLASH
24 43 STAR
26 40 BECOMES
26 40 COMMA
26 40 EQ
26 40 GE
26 40 GT
26 40 LE
26 40 LT
26 40 MINUS
26 40 NE
26 40 PCT
26 40 PLUS
26 40 RBRACK
26 40 RPAREN
26 40 SEMI
26 40 SLASH
26 40 STAR
28 2 EOF
33 37 BECOMES
33 37 COMMA
33 37 EQ
33 37 GE
33 37 GT
33 37 LE
33 37 LT
33 37 MINUS
33 37 NE
33 37 PCT
33 37 PLUS
33 37 RBRACK
33 37 RPAREN
33 37 SEMI
33 37 SLASH
33 37 STAR
38 44 RPAREN
41 3 INT
44 42 BECOMES
44 42 COMMA
44 42 EQ
44 42 GE
44 42 GT
44 42 LE
44 42 LT
44 42 MINUS
44 42 NE
44 42 PCT
44 42 PLUS
44 42 RBRACK
44 42 RPAREN
44 42 SEMI
44 42 SLASH
44 42 STAR
47 41 BECOMES
47 41 COMMA
47 41 EQ
47 41 GE
47 41 GT
47 41 LE
47 41 LT
47 41 MINUS
47 41 NE
47 41 PCT
47 41 PLUS
47 41 RBRACK
47 41 RPAREN
47 41 SEMI
47 41 SLASH
47 41 STAR
5 11 DELETE
5 11 ID
5 11 IF
5 11 INT
5 11 LPAREN
5 11 PRINTLN
5 11 RETURN
5 11 STAR
5 11 WHILE
51 28 COMMA
51 28 EQ
51 28 GE
51 28 GT
51 28 LE
51 28 LT
51 28 MINUS
51 28 NE
51 28 PLUS
51 28 RBRACK
51 28 RPAREN
51 28 SEMI
54 32 COMMA
54 32 EQ
54 32 GE
54 32 GT
54 32 LE
54 32 LT
54 32 MINUS
54 32 NE
54 32 PCT
54 32 PLUS
54 32 RBRACK
54 32 RPAREN
54 32 SEMI
54 32 SLASH
54 32 STAR
55 33 COMMA
55 33 EQ
55 33 GE
55 33 GT
55 33 LE
55 33 LT
55 33 MINUS
55 33 NE
55 33 PCT
55 33 PLUS
55 33 RBRACK
55 33 RPAREN
55 33 SEMI
55 33 SLASH
55 33 STAR
56 34 COMMA
56 34 EQ
56 34 GE
56 34 GT
56 34 LE
56 34 LT
56 34 MINUS
56 34 NE
56 34 PCT
56 34 PLUS
56 34 RBRACK
56 34 RPAREN
56 34 SEMI
56 34 SLASH
56 34 STAR
59 29 COMMA
59 29 EQ
59 29 GE
59 29 GT
59 29 LE
59 29 LT
59 29 MINUS
59 29 NE
59 29 PLUS
59 29 RBRACK
59 29 RPAREN
59 29 SEMI
60 30 COMMA
60 30 EQ
60 30 GE
60 30 GT
60 30 LE
60 30 LT
60 30 MINUS
60 30 NE
60 30 PLUS
60 30 RBRACK
60 30 RPAREN
60 30 SEMI
61 11 DELETE
61 11 ID
61 11 IF
61 11 INT
61 11 LPAREN
61 11 PRINTLN
61 11 RETURN
61 11 STAR
61 11 WHILE
65 45 RPAREN
66 47 BECOMES
66 47 COMMA
66 47 EQ
66 47 GE
66 47 GT
66 47 LE
66 47 LT
66 47 MINUS
66 47 NE
66 47 PCT
66 47 PLUS
66 47 RBRACK
66 47 RPAREN
66 47 SEMI
66 47 SLASH
66 47 STAR
69 20 DELETE
69 20 ID
69 20 IF
69 20 LPAREN
69 20 PRINTLN
69 20 RBRACE
69 20 RETURN
69 20 STAR
69 20 WHILE
77 15 DELETE
77 15 ID
77 15 IF
77 15 LPAREN
77 15 PRINTLN
77 15 RBRACE
77 15 RETURN
77 15 STAR
77 15 WHILE
80 15 DELETE
80 15 ID
80 15 IF
80 15 LPAREN
80 15 PRINTLN
80 15 RBRACE
80 15 RETURN
80 15 STAR
80 15 WHILE
81 15 DELETE
81 15 ID
81 15 IF
81 15 LPAREN
81 15 PRINTLN
81 15 RBRACE
81 15 RETURN
81 15 STAR
81 15 WHILE
83 31 COMMA
83 31 EQ
83 31 GE
83 31 GT
83 31 LE
83 31 LT
83 31 MINUS
83 31 NE
83 31 PCT
83 31 PLUS
83 31 RBRACK
83 31 RPAREN
83 31 SEMI
83 31 SLASH
83 31 STAR
87 22 RPAREN
88 23 RPAREN
89 24 RPAREN
90 25 RPAREN
91 26 RPAREN
92 27 RPAREN
93 39 BECOMES
93 39 COMMA
93 39 EQ
93 39 GE
93 39 GT
93 39 LE
93 39 LT
93 39 MINUS
93 39 NE
93 39 PCT
93 39 PLUS
93 39 RBRACK
93 39 RPAREN
93 39 SEMI
93 39 SLASH
93 39 STAR
95 10 ID
98 15 DELETE
98 15 ID
98 15 IF
98 15 LPAREN
98 15 PRINTLN
98 15 RBRACE
98 15 RETURN
98 15 STAR
98 15 WHILE
)END";

const std::string WLP4_DFA = WLP4_TRANSITIONS+WLP4_REDUCTIONS;
const std::string WLP4_COMBINED = WLP4_CFG+WLP4_DFA;



//Rule Struct
//lhs -> rhs
struct Rule {
    int ruleNumber;
    std::string lhs;
    std::vector<std::string> rhs;
};

//CFG is a vector of Rules
std::vector<Rule> CFG;

unordered_set<string> nonterminals;

void parseCFG(const std::string& cfgString) {
    std::istringstream iss(cfgString);
    std::string line;
    int ruleNumber = 0; 
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        if (line == ".CFG") {continue;}
        Rule rule;
        rule.ruleNumber = ruleNumber++;
        lineStream >> rule.lhs;
        nonterminals.insert(rule.lhs);
        std::string symbol;
        while (lineStream >> symbol) {
            if (symbol == ".EMPTY") {
                continue;
            }
            rule.rhs.push_back(symbol);
        }
        CFG.push_back(rule);
    }
}


//SLR(1) DFA, struct that contains two maps, for transitions and reductions.

struct StateSymbolPair {
    int state;
    std::string symbol;
};

//transitions map: <state, symbol> keys and state values.
// <FromState, symbol> -> ToState

//reductions map: <state, symbol> keys and rule-number values.
// <state-number, rule-number(0 to in dfa), and elements in follow set of lhs of the rule tag>
//<state-number, symbol> -> rule


struct SLR {
    std::map<std::pair<int, std::string>, int> transitions;
    std::map<std::pair<int, std::string>, int> reductions;
};


// 0 BOF 45
void populateTrans(std::map<std::pair<int, std::string>, int>& map, const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        if (line == ".TRANSITIONS") {continue;}
        int fromState;
        std::string symbol;
        int toState;
        lineStream >> fromState >> symbol >> toState;
        map[std::make_pair(fromState, symbol)] = toState;
    }
}


// 10 36 BECOMES
void populateRed(std::map<std::pair<int, std::string>, int>& map, const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        if (line == ".REDUCTIONS") {continue;}
        int curState;
        std::string followSetSym;
        int ruleNumber;
        lineStream >> curState >> ruleNumber >> followSetSym;
        map[std::make_pair(curState, followSetSym)] = ruleNumber;
    }
}



void printMaps(const SLR& slr) {
    std::cout << "Transitions:\n";
    for (const auto& transition : slr.transitions) {
        std::cout << "<" << transition.first.first << ", " << transition.first.second << "> -> " << transition.second << "\n";
    }

    std::cout << "\nReductions:\n";
    for (const auto& reduction : slr.reductions) {
        std::cout << "<" << reduction.first.first << ", " << reduction.first.second << "> -> " << reduction.second << "\n";
    }
}


// Token structure
struct Token {
    std::string lexeme;
    std::string kind;
};


// Define the Node structure
struct Node {
   std::variant<Token, Rule> value;
   std::vector<std::unique_ptr<Node>> children;

    Node(Token token) : value(token) {}
    Node(Rule rule) :  value(rule) {}

    void print(std::ostream &out = std::cout) {
    std::ostringstream oss;
    if (std::holds_alternative<Token>(value)) {
        Token& token = std::get<Token>(value);
        oss << token.kind << " " << token.lexeme;
    } else if (std::holds_alternative<Rule>(value)) {
        Rule& rule = std::get<Rule>(value);
        oss << rule.lhs << " ";
        if (rule.rhs.empty()) {
            oss << ".EMPTY";
        } else {
            for (const auto& symbol : rule.rhs) {
                oss << symbol << " ";
            }
        }
    }

    // Trim trailing spaces
    std::string str = oss.str();
    str.erase(str.find_last_not_of(" \n\r\t")+1);
    out << str << "\n";

    // Print the children
    for (size_t i = 0; i < children.size(); ++i) {
        children[i]->print(out);
    }
}

};




//SET UP STACKS
// State stack for keeping track of progress in the DFA
std::vector<int> stateStack = {0};  // Initialize with the initial state of the SLR(1) DFA

// Tree stack for partial parse trees
std::vector<std::unique_ptr<Node>> treeStack;  // Initialize as empty



void reduceTrees(std::vector<std::unique_ptr<Node>>& treeStack, const Rule& rule) {
    // Create a new tree node storing the CFG rule (lhs)
    auto newNode = std::make_unique<Node>(rule);

    // Let len be the length of the right-hand side of the CFG rule
    int len = rule.rhs.size();

    // Copy the last len trees from the tree stack into the new node's children

      for (int i = 0; i < len; ++i) {
        newNode->children.insert(newNode->children.begin(), std::move(treeStack.back()));
        treeStack.pop_back();
    }


    // Pop the last len trees from the tree stack
    // for (int i = 0; i < len; ++i) {
    //     treeStack.pop_back();
    // }

    // Push the new node to the tree stack
    treeStack.push_back(std::move(newNode));
}


void reduceStates(Rule rule, SLR& slr) {
    int len = rule.rhs.size();

    // Pop the last len states from the state stack
    for (int i = 0; i < len; i++) {
         if (!stateStack.empty()) {
            stateStack.pop_back();
         }
    }

    // Look at the current top of the state stack
    int currentState = stateStack.back();

    //Create the key for the transitions map
    std::pair<int, std::string> key = std::make_pair(currentState, rule.lhs);

    // Check if the transition exists
    if (slr.transitions.count(key) > 0) {
        // Look up the transition in the SLR(1) DFA corresponding to this state and symbol
        int nextState = slr.transitions[key];
        //  std::cout << "NEXT: " << nextState << endl;

        // Push the resulting next state from this transition to the top of the state stack
        stateStack.push_back(nextState);
    } else {
        // Handle the case where the transition doesn't exist
        std::cout << "No transition exists for the current state and symbol." << std::endl;
    }
}




void shift(SLR& slr, std::deque<Token>& tokens) {
    // Create a new tree node corresponding to the first token of unread input
    auto node = std::make_unique<Node>(tokens.front());
    
    // Push this node to the tree stack
    treeStack.push_back(std::move(node));
    
    // Look at the current top of the state stack
    int currentState = stateStack.back();

    
    // Check if there is a DFA transition from this state, with the symbol being the kind of the first token of unread input
    auto transition = slr.transitions.find({currentState, tokens.front().kind});
    
    if (transition == slr.transitions.end()) {
        // If there is no such transition, print an error message containing ERROR to standard error and stop the program
        std::cerr << "ERROR: No transition from state " << currentState << " with symbol " << tokens.front().kind << std::endl;
        //to change
        // exit(EXIT_FAILURE);
    } else {
        // If there is a transition to a new state, push that new state to the state stack
        stateStack.push_back(transition->second);
    }
    
    // Consume the first token from unread input
    tokens.pop_front();
}




void tokensToTrees(std::deque<Token>& tokens, std::vector<std::unique_ptr<Node>>& treeStack) {
    for (const auto& token : tokens) {
        // if ((token.kind == "EOF") || (token.kind == "BOF")) {continue;}
        auto newNode = std::make_unique<Node>(token);
        treeStack.push_back(std::move(newNode));
    }
}




int main() {

    try {
    //populate maps
    SLR slr;
    populateTrans(slr.transitions, WLP4_TRANSITIONS);
    populateRed(slr.reductions, WLP4_REDUCTIONS);

    //Parse CFG and create vector
    parseCFG(WLP4_CFG);

    // std::cout << "RULES" << std::endl;
    // for (const auto& rule : CFG) {
    //     cout << rule.ruleNumber << " ";
    //     std::cout << rule.lhs << " -> ";
    //     for (const auto& symbol : rule.rhs) {
    //         std::cout << symbol << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // // Print maps
    // printMaps(slr);

    //Read input and Augment 
    //Set up Tokens 
    std::deque<Token> tokens;
    std::string line;

     tokens.push_back({"BOF", "BOF"});

    // Read from standard input line by line
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string kind, lexeme;
        iss >> kind >> lexeme;
        // cout << "kind: " << kind << " " << "lexeme: " << lexeme << endl;


        tokens.push_back({lexeme, kind});
    }

     tokens.push_back({"EOF", "EOF"});

    // Print the contents of the deque
    // std::cout << "\nDeque\n";
    // for (const auto& token : tokens) {
    //     std::cout << token.kind << " " << token.lexeme << "\n";
    // }


    // Convert tokens to trees
    // tokensToTrees(tokens, treeStack);


    // Print the contents of the tree stack before calling reduceTrees
    // std::cout << "\nBefore tree stack:\n";
    // for (const auto& tree : treeStack) {
    //     tree->print();
    // }
    //  cout << endl;

    // Rule r;
    // r.lhs = "start";
    // r.rhs = {"BOF", "procedures", "EOF"};

    // Rule e;
    // e.lhs = "dcls";
    // e.rhs = {};

    // void reduceStates(Rule rule, SLR& slr)



    // Reduce the trees
    // reduceTrees(treeStack, r);
    // reduceTrees(treeStack, r);

    // Print the reduced tree stack
    // std::cout << "\nReduced tree stack:\n";
    // for (const auto& node : treeStack) {
    //     node->print();
    // }


    // stateStack.push_back(1);
    // stateStack.push_back(0);
    // stateStack.push_back(2);
    // stateStack.push_back(4);
    //  //Top of stack

    // // Print the state stack
    //  cout << "B4 State Stack" << endl;
    // for (int state : stateStack) {
    //     std::cout << state << " ";
    // }
    // cout << endl;

    // slr.transitions[std::make_pair(0, "BOF")] = 45;
    // slr.transitions[std::make_pair(1, "AMP")] = 35;
    // slr.transitions[std::make_pair(1, "ID")] = 13;
    // // Add more transitions as per your example...

    // // Initialize a rule
    // Rule rule1;
    // rule1.lhs = "ID";
    // rule1.rhs = {"AMP", "LPAREN", "NEW"};

    // // Call the reduceStates function
    // reduceStates(rule1, slr);

    // // // Print the state stack
    // cout << "AFTER" << endl;
    // for (int state : stateStack) {
    //     std::cout << state << " ";
    // }
    // std::cout << std::endl;



    // slr.transitions[{0, "BOF"}] = 1;
    // slr.transitions[{1, "EOF"}] = 2;
    // shift(slr, tokens);
    // shift(slr, tokens);
    //  assert(stateStack.at(stateStack.size() - 2) == 1);
    // assert(stateStack.back() == 2);
    // Check if the tokens were consumed
    // assert(tokens.empty());
    // assert(stateStack.back() == 1);
    // assert(tokens.size() == 1);

    //  std::cout << "\nAfter tree stack:\n";
    // for (const auto& tree : treeStack) {
    //     tree->print();
    // }


    while (true) {

        int state = stateStack.back();
        std::string tokenKind = tokens.front().kind;

        // Check for reductions
        while (slr.reductions.count({state, tokenKind})) {
            std::vector<Rule>::size_type ruleIndex = slr.reductions[{state, tokenKind}];
            if (ruleIndex >= 0 && ruleIndex < CFG.size()) {
                Rule rule = CFG[ruleIndex];
        
                reduceTrees(treeStack, rule);
                reduceStates(rule, slr);
            }
            state = stateStack.back();
        }


        // No reduction found, try to shift

        // Call the shift function
        shift(slr, tokens);



        // If all input is consumed and no error was produced, the parse was successful

    //  if (tokens.front().kind == "EOF" && tokens.front().lexeme == "EOF" && tokens.size() == 1) {
        // tokens.pop_front();
        //  cout << "success" << endl;
            // break;


    if (tokens.size() == 0) {
        // cout << "success" << endl;
        break;
    }

}

  // 

//    std::cout << "\nDeque4\n";
//     for (const auto& token : tokens) {
//         std::cout << token.kind << " " << token.lexeme << "\n";
//     }            

// After the main parsing loop, reduce by the start rule
reduceTrees(treeStack, CFG[0]);

// std::cout << "v" << endl;;
//  if (!treeStack.empty()) {
//         treeStack.back()->print();
//     }
    //  treeStack.back()->print();

    //    std::cout << "\nReduced tree stack:\n";
    for (const auto& node : treeStack) {
        node->print();
    }

    } catch(std::runtime_error &e) {
                std::cerr << "ERROR: " << e.what() << "\n";
                return 1;
 }



    
    return 0;
}



// g++ -g -Wall -std=c++17 wlp4parse-prescanned.cc -o wlparser 
// ./wlparser < test.in 