#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <map>
#include <memory>
using namespace std;



struct Node {
    std::string rule;
    std::string lhs;
    std::string kind;
    std::string lexeme;
    std::string type;
    std::vector<std::string> rhs;
    std::vector<std::shared_ptr<Node>> children;


// Return the n-th child of the tree such that the rule LHS 
// (for nonterminal nodes) or the token kind (for terminal nodes)
//  matches the string.
    std::shared_ptr<Node> getChild(const std::string& str, int n = 1) {
    int count = 0;
    for (const auto& child : children) {
        // cout << child->lhs << endl;
        // cout << child->kind << endl;
        if ((child->lhs == str || child->kind == str) && ++count == n) {
            return child;
        }
    }
    return nullptr; // return null pointer if no such child is found
}

};


std::map<std::string, int> offsetTable;

void printOffsetTable() {
    std::cout << "Offset Table:\n";
    std::cout << "Name\tOffset\n";
    for (const auto& entry : offsetTable) {
        std::cout << entry.first << "\t" << entry.second << "\n";
    }
}


int tC = 0;
int lC = 0;

std::shared_ptr<Node> constructTree() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return nullptr;
    }

    std::stringstream ss(line);
    std::string part;
    std::shared_ptr<Node> node = std::make_shared<Node>();

    ss >> node->lhs;
    node->rule += node->lhs;

    while (ss >> part) {
        if (part == ".EMPTY") {
            break;
        } else if (part == ":") {
            ss >> node->type;
            break;
        } else {
            node->rhs.push_back(part);
            node->rule += " " + part;
        }
    }

    for (const auto& symbol : node->rhs) {
        if (isupper(symbol[0])) {  // Terminal
            std::getline(std::cin, line);
            std::stringstream ss(line);
            std::shared_ptr<Node> child = std::make_shared<Node>();
            ss >> child->kind >> child->lexeme;
            node->children.push_back(child);
        } else {  // Non-terminal
            node->children.push_back(constructTree());
        }
    }

    return node;
}


void print(const std::shared_ptr<Node>& node, std::string prefix = "", bool isLast = true, std::ostream &out = std::cout) {
    if (!prefix.empty()) {
        // Remove the single whitespace at the start of each line
        out << prefix.substr(2);
        out << (isLast ? "╰─" : "├─");
    }

    if (!node->kind.empty()) {
        out << node->kind << " ";
    }

    // Check if lexeme is empty, if it is, print lhs, else print lexeme
    if (node->lexeme.empty()) {
        out << node->lhs << "\n";
    } else {
        out << node->lexeme << "\n";
    }

    // Update the prefix for children
    std::string childPrefix = prefix + (isLast ? "  " : "│ ");

    // Print the children
    for (size_t i = 0; i < node->children.size(); ++i) {
        print(node->children[i], childPrefix, i == node->children.size() - 1, out);
    }
}


// void print(const std::shared_ptr<Node>& node, std::string prefix = "", bool isLast = true, std::ostream &out = std::cout) {
//     if (!prefix.empty()) {
//         // Remove the single whitespace at the start of each line
//         out << prefix.substr(2);
//         out << (isLast ? "╰─" : "├─");
//     }

//     out << "Node: " << node->lhs << "\n";
//     out << "Rule: " << node->rule << "\n";

//     if (!node->kind.empty()) {
//         out << "Kind: " << node->kind << "\n";
//     }

//     // Check if lexeme is empty, if it is, print lhs, else print lexeme
//     if (!node->lexeme.empty()) {
//         out << "Lexeme: " << node->lexeme << "\n";
//     }

//     if (!node->type.empty()) {
//         out << "Type: " << node->type << "\n";
//     }

//     // Update the prefix for children
//     std::string childPrefix = prefix + (isLast ? "  " : "│ ");

//     // Print the children
//     for (size_t i = 0; i < node->children.size(); ++i) {
//         print(node->children[i], childPrefix, i == node->children.size() - 1, out);
//     }
// }

// void printNode(const std::shared_ptr<Node>& node, std::ostream &out = std::cout) {
//     // Your logic to print a single node goes here
//     // You can use node->rule, node->lhs, etc.
//     // For example:
//     out << "Rule: " << node->rule << ", LHS: " << node->lhs << std::endl;
// }

// void print(const std::vector<std::shared_ptr<Node>>& nodes, std::string prefix = "", bool isLast = true, std::ostream &out = std::cout) {
//     for (size_t i = 0; i < nodes.size(); ++i) {
//         std::cout << prefix;
//         if (isLast) {
//             std::cout << "└── ";
//             prefix += "    ";
//         } else {
//             std::cout << "├── ";
//         }

//         // Print the current node
//         printNode(nodes[i], out);

//         // Recursively print the children of the current node
//         print(nodes[i]->children, prefix, i == nodes.size() - 1, out);
//     }
// }





void Add(int d, int s, int t) {
    std::cout << "add $" << d << ", $" << s << ", $" << t << "\n";
}

void Sub(int d, int s, int t) {
    std::cout << "sub $" << d << ", $" << s << ", $" << t << "\n";
}

void Mult(int s, int t) {
    std::cout << "mult $" << s << ", $" << t << "\n";
}

void Multu(int s, int t) {
    std::cout << "multu $" << s << ", $" << t << "\n";
}

void Div(int s, int t) {
    std::cout << "div $" << s << ", $" << t << "\n";
}

void Divu(int s, int t) {
    std::cout << "divu $" << s << ", $" << t << "\n";
}

void Beq(int s, int t, const std::string& label) {
    std::cout << "beq $" << s << ", $" << t << ", " << label;
}

void Mfhi(int d) {
    std::cout << "mfhi $" << d << "\n";
}

void Mflo(int d) {
    std::cout << "mflo $" << d << "\n";
}

void Lis(int d, int value) {
    std::cout << "lis $" << d << "\n";
    std::cout << ".word " << value << "\n";
}

void Lis(int d, const std::string& label) {
    std::cout << "lis $" << d << "\n";
    std::cout << ".word " << label << "\n";
}

void Slt(int d, int s, int t) {
    std::cout << "slt $" << d << ", $" << s << ", $" << t << "\n";
}

void Sltu(int d, int s, int t) {
    std::cout << "sltu $" << d << ", $" << s << ", $" << t << "\n";
}


void Jr(int s) {
    std::cout << "jr $" << s << "\n";
}

void Jalr(int s) {
    std::cout << "jalr $" << s << "\n";
}

void Bne(int s, int t, int i) {
    std::cout << "bne $" << s << ", $" << t << ", " << i * 4 << "\n";
}

void Lw(int t, int i, int s) {
    std::cout << "lw $" << t << ", " << i << "($" << s << ")\n";
}

void Sw(int t, int i, int s) {
    if ( i == 0) {
          std::cout << "sw $" << t << ", " << i  << "($" << s << ")\n";
    } else {
        std::cout << "sw $" << t << ", "  << "-" << i  << "($" << s << ")\n";
    }
}

void Constant(int value) {
    // int constantValue = value;
    std::cout << "lis $3\n";
    std::cout << ".word " << value << "\n";
}


void Word(int i) {
    std::cout << ".word " << i << "\n";
}

void Word(const std::string& label) {
    std::cout << ".word " << label << "\n";
}

void Label(const std::string& name) {
    std::cout << name << ":\n";
}

void push(int s) {
    //  std::cout << "lis $4\n";      // Load constant 4 into $4
    // std::cout << ".word " << 4 << "\n";
    std::cout << "sw $" << s << ", -4($30)\n";
    std::cout << "sub $30, $30, $4\n";
}

void pop(int d) {
    std::cout << "add $30, $30, $4\n";
    std::cout << "lw $" << d << ", -4($30)\n";
}

void pop() {
    std::cout << "add $30, $30, $4\n";
}

void loadImmediate(int d, int value) {
    std::cout << "li $" << d << ", " << value << "\n";
}


void extractDcls(const std::shared_ptr<Node>& node, std::vector<std::shared_ptr<Node>>& dclsNodes) {
    if (!node) {
        return;
    }

    // Check if the node represents a dcls rule
    if (node->lhs == "dcls") {
        dclsNodes.push_back(node);
    }

    // Recursively process children
    for (const auto& child : node->children) {
        extractDcls(child, dclsNodes);
    }
}






void genCase(const std::shared_ptr<Node>& node) {
    std::string rule = node->rule;
    // cout << 
    // cout << rule << endl;

    //3A:
    if (rule == "expr term") {
        genCase(node->getChild("term"));
    } else if (rule == "term factor") {
        genCase(node->getChild("factor"));
    } 
    else if (rule == "expr expr PLUS term") {
        genCase(node->getChild("expr"));
        push(3);
        genCase(node->getChild("term"));
        pop(5);

        if (node->getChild("term")->type == "int*" && node->getChild("expr")->type == "int") {
        
        // Multiply offset by 4
        Mult(5, 4);
        Mflo(5);
        Add(3, 5, 3);

        } else if (node->getChild("expr")->type == "int*" && node->getChild("term")->type == "int") {
            Mult(3, 4);
            Mflo(3);
            Add(3, 5, 3);
        } else if (node->getChild("expr")->type == "int" && node->getChild("term")->type == "int" ){
            Add(3, 5, 3);
        }

    } else if (rule == "expr expr MINUS term") {
       genCase(node->getChild("expr"));
        push(3);
        genCase(node->getChild("term"));
         pop(5);

        if (node->getChild("expr")->type == "int*" && node->getChild("term")->type == "int"){
          
            Mult(3, 4);
            Mflo(3);
            Sub(3, 5, 3);
            
         } else if(node->getChild("expr")->type == "int" && node->getChild("term")->type == "int"){
            Sub(3, 5, 3);
         
         } else if (node->getChild("expr")->type == "int*" && node->getChild("term")->type == "int*") {

        genCase(node->getChild("expr"));
        push(3);
        genCase(node->getChild("term"));
        pop(5);
        Sub(3, 5, 3);
        // Divide the result by 4
        Div(3, 4);
        Mflo(3);
    }
    
        
    } else if (rule == "factor NULL") {
    // Load NULL (1) into $3
     Add(3, 11, 0);
}
     else if (rule == "term term STAR factor") {
        genCase(node->getChild("term"));
        push(3);
        genCase(node->getChild("factor"));
        pop(5);
        Mult(5, 3);
        Mflo(3);
    } else if (rule == "term term SLASH factor") {
        genCase(node->getChild("term"));
        push(3);
        genCase(node->getChild("factor"));
        pop(5);
        Div(5,3);
        Mflo(3);
    } else if (rule == "term term PCT factor") {
        genCase(node->getChild("term"));
        push(3);
        genCase(node->getChild("factor"));
        pop(5);
        Div(5,3);
        Mfhi(3);
    } else if (rule == "factor ID") {
        string v = node->getChild("ID")->lexeme;
        // cout << v << endl;
        cout << "lw $3, " << offsetTable[v] << "($29)" << endl;
    } else if (rule == "factor NUM") {
        cout << "lis $3" << endl << ".word " << node->getChild("NUM")->lexeme << endl;
    } else if (rule == "factor LPAREN expr RPAREN") {
        genCase(node->getChild("expr"));
    }

    //3B
    else if(rule == "statements statements statement"){
        genCase(node->getChild("statements"));
        genCase(node->getChild("statement"));
    
    }
    //assn here 
    else if(rule == "statement lvalue BECOMES expr SEMI"){
         genCase(node->getChild("lvalue"));
            push(3);
            genCase(node->getChild("expr")); 
             pop(5);
             Sw(3, 0, 5);

    } else  if(rule == "lvalue ID"){
            string v = node->getChild("ID")->lexeme;
             cout << "lis $3" << endl;
            cout << ".word " << offsetTable[v] << endl;
             Add(3, 3, 29);
           
    } else if(rule == "lvalue LPAREN lvalue RPAREN") {
        genCase(node->getChild("lvalue"));
        // // Load address into $3
        // Add(3, 3, 0);
     
    } else if (rule == "statement PRINTLN LPAREN expr RPAREN SEMI" ) {
        genCase(node->getChild("expr"));
        Add(1,3,0); //Move the result of the expression to $1
        push(31);
        Lis(5, "print");
        Jalr(5);
        pop(31);

    } else if (rule == "factor AMP lvalue") {
        if (node->getChild("lvalue")->rule == "lvalue ID")  {
            genCase(node->getChild("lvalue"));
        } else if (node->getChild("lvalue")->rule == "lvalue STAR factor")  {
            genCase(node->getChild("factor"));
        } 
    } else if (rule == "factor STAR factor") {
        genCase(node->getChild("factor"));
        // // Load value at address into $3
        Lw(3, 0, 3);
    } else if (rule == "lvalue STAR factor") {
        genCase(node->getChild("factor"));

    } else if (rule == "test expr EQ expr") {
        genCase(node->getChild("expr", 1));
        push(3);
        genCase(node->getChild("expr", 2));
        pop(5);
        
        if (node->getChild("expr", 1)->type == "int*" && node->getChild("expr", 2)->type == "int*") {
            Sltu(6, 5, 3);
            Sltu(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
        } else if (node->getChild("expr", 1)->type == "int" && node->getChild("expr", 2)->type == "int") {
            Slt(6, 5, 3);
            Slt(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
        }
    } else if (rule == "test expr NE expr") {
        genCase(node->getChild("expr", 1));
        push(3);
        genCase(node->getChild("expr", 2));
        pop(5);

        if (node->getChild("expr", 1)->type == "int*" && node->getChild("expr", 2)->type == "int*") {
            Sltu(6, 5, 3);
            Sltu(7, 3, 5);
            Add(3,6,7);
        } else if (node->getChild("expr", 1)->type == "int" && node->getChild("expr", 2)->type == "int") {
            Slt(6, 5, 3);
            Slt(7, 3, 5);
            Add(3,6,7);
        }
    } else if (rule == "test expr LT expr") {
        genCase(node->getChild("expr", 1));
        push(3);
        genCase(node->getChild("expr", 2));
        pop(5);

        if (node->getChild("expr", 1)->type == "int*" && node->getChild("expr", 2)->type == "int*") {
        Sltu(6, 5, 3);
        Add(3,0,6);
        }  else if (node->getChild("expr", 1)->type == "int" && node->getChild("expr", 2)->type == "int") {
            Slt(6, 5, 3);
            Add(3,0,6);
        }

    } else if (rule == "test expr LE expr") {
        genCase(node->getChild("expr", 1));
        push(3);
        genCase(node->getChild("expr", 2));
        pop(5);

        if (node->getChild("expr", 1)->type == "int*" && node->getChild("expr", 2)->type == "int*") {
            Sltu(6, 5, 3);
            Sltu(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
            Add(3,3,6);
        } else if (node->getChild("expr", 1)->type == "int" && node->getChild("expr", 2)->type == "int") {
            Slt(6, 5, 3);
            Slt(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
            Add(3,3,6);
        }
    } else if (rule == "test expr GE expr") {
        genCase(node->getChild("expr", 1));
        push(3);
        genCase(node->getChild("expr", 2));
        pop(5);

         if (node->getChild("expr", 1)->type == "int*" && node->getChild("expr", 2)->type == "int*") {
            Sltu(6, 5, 3);
            Sltu(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
            Add(3,3,7);

         } else if (node->getChild("expr", 1)->type == "int" && node->getChild("expr", 2)->type == "int") {
            Slt(6, 5, 3);
            Slt(7, 3, 5);
            Add(3,6,7);
            Sub(3,11,3);
            Add(3,3,7);

         }
     
    } else if (rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        static int toopCounter = 0;
        int currentToopCounter = toopCounter++;
        genCase(node->getChild("test", 1));
        Beq(3, 0, "else");
        cout << currentToopCounter << endl;
        currentToopCounter++;
        genCase(node->getChild("statements", 1));
        currentToopCounter--;
        Beq(0, 0, "endif");
        cout << currentToopCounter << endl;
        cout << "else" << currentToopCounter << ":" << endl;
        currentToopCounter++;
         genCase(node->getChild("statements", 2));
        currentToopCounter--;
        cout << "endif" << currentToopCounter << ":" << endl;
    } else if (rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        static int loopCounter = 0;
        int currentLoopCounter = loopCounter++;

        cout << "loopStart" << currentLoopCounter << ":" << endl;
        genCase(node->getChild("test", 1));
        Beq(3, 0, "endLoop");
        cout << currentLoopCounter << endl;
        currentLoopCounter++;
        genCase(node->getChild("statements", 1));
        currentLoopCounter--;
        Beq(0, 0, "loopStart");
        cout << currentLoopCounter << endl;
        cout << "endLoop" << currentLoopCounter << ":" << endl;
    }
}



void generateCode(std::shared_ptr<Node>& mainNode) {
    cout << ".import print" << endl;
     // Load the value 4 into $4
    Lis(4, 4);
    Lis(11, 1);
    Sub(29, 30, 4);

     auto targNode = (mainNode->getChild("procedures"))->getChild("main");
    // Construct the offset table and generate code for parameters
    int offset = 8;  // Start offset for parameters
    // Get the first dcl child
    auto firstDclNode = targNode->getChild("dcl");
    // print(firstDclNode);
    std::string firstVarName = firstDclNode->getChild("ID")->lexeme;
    offsetTable[firstVarName] = offset;
    offset -= 4;
    push(offset / 4);
    // Sw(offset / 4 + 1, offset, 30);

    // Get the second dcl child
    auto secondDclNode = targNode->getChild("dcl", 2);
    std::string secondVarName = secondDclNode->getChild("ID")->lexeme;
    offsetTable[secondVarName] = offset;
    offset -= 4;
    push(offset / 4 + 2);
    // cout << "Off: " << offset / 4 + 1 << endl;
    // Sw(offset / 4 + 1, offset, 30);

    // Set the frame pointer
    Sub(29, 30, 4);


    // Traverse the dcls subtree
    offset = 0;  // Reset offset for non-parameter variables

    // print(mainNode->getChild("dcls"));
    std::vector<std::shared_ptr<Node>> dclsNodes;
    extractDcls(targNode, dclsNodes);

    // for (const auto& dclsNode : dclsNodes) {
    //     cout << endl;
    //        cout << endl;
    //      print(dclsNode);
    //     // Add any additional processing or printing here
    // }

    // Process the declarations in the correct order
 for (auto it = dclsNodes.rbegin(); it != dclsNodes.rend(); ++it) {
    auto dclNode = (*it)->getChild("dcl");
    if (dclNode == nullptr) {continue;}
    // cout << "CHILD" << endl;
    // print(dclNode);
    // cout << endl;
    // cout << endl;
    // cout << "DCLS" << endl;
    //  print(*it);

    auto idNode = dclNode->getChild("ID");
    if (idNode != nullptr) {
        std::string varName = idNode->lexeme;
        // cout << "var: " << varName << endl; 
        offsetTable[varName] = offset;
        offset -= 4;
    } else {
        continue;
    }

    // size_t currentIndex = std::distance(dclsNodes.rbegin(), it);
    // cout << currentIndex << endl;
    auto numNode = (*it)->getChild("NUM");
    if (numNode) {
        int initValue = std::stoi(numNode->lexeme);
        // cout << "in: " <<  initValue << endl;
         Lis(3, initValue);
         push(3);
    } else {
        auto nullNode = (*it)->getChild("NULL");
        if (nullNode) {
               Lis(3, 1);  // Use the constant 1 to represent NULL
               push(3);
        }
    }
 }


  auto statementsNode = targNode->getChild("statements");
  auto exprNode = targNode->getChild("expr");

  genCase(statementsNode);
  genCase(exprNode);


//   cout << "statNode" << endl;
//   print(statementsNode);

 // Clean up the stack
    // for (const auto& var : offsetTable) {
    //      pop();
    // }

//end
Jr(31);
    }








int main() {
    std::shared_ptr<Node> root = constructTree();
    // print(root);

    generateCode(root);
    // printOffsetTable();
    
    return 0;
}





// g++ -g -Wall -std=c++17 wlp4gen-preanalyzed.cc -o cgen
//  ./cgen < p5n
// ./runtests.bash test.wlp4 
// marmoset_submit CS241 P5 wlp4gen-preanalyzed.cc





     //  genCase(node->getChild("lvalue"));
        //  push(3);
        //  if (node->getChild("lvalue")->rule == "lvalue STAR factor") {
        //     push(3);
        //     genCase(node->getChild("expr")); 
        //     // genCase(node->getChild("lvalue"));
        //     pop(5);
        //     Sw(3, 0, 5);
        //  } else if (node->getChild("lvalue")->rule == "lvalue ID")  {
        //      genCase(node->getChild("lvalue"));
        //     push(3);
        //     genCase(node->getChild("expr")); 
        //      pop(5);
        //      Sw(3, 0, 5);

        //     // genCase(node->getChild("expr"));
        //     // auto lnode = node->getChild("lvalue");
        //     // string t = lnode->getChild("ID")->lexeme;
        //     // Sw(3, offsetTable[t], 29);
        //     // push(3);
        //     // pop(5);
        //     // Sw(3, 0, 5);
        //  }
        // genCase(node->getChild("expr")); 
        // pop(5);
        // Sw(3, 0, 5);













     // Example usage
    // loadImmediate(4, 4); // Load constant 4 into $4
    // push(5);            // Push the value in $5 to the stack
    // pop(6);             // Pop the top item of the stack into $6
    // pop();              // Pop the top item of the stack and discard it
    // Add(3, 1, 0);       // Example Add instruction
    // Label("loop");      // Example Label instruction
    // Beq(3, 2, "end");    // Example Beq instruction
    // Add(3, 3, 1);
    // Beq(0, 0, "loop");
    // Label("end");
    // Jr(31);




//test expr EQ expr
// cgen(expr1)
// push(3)
// lis(8)
// word(1)
// cgen(expr2)
// pop(5)
// if both int*:
// sltu 
// else slt