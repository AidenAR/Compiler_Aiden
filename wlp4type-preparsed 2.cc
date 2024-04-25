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
    std::vector<std::string> rhs;
    std::string kind;
    std::string lexeme;
    std::vector<Node*> children;
    bool isTerminal;
    std::string type; 

    Node(std::string rule) : rule(rule), isTerminal(false), type("untyped") {}

    ~Node() {
        for(auto &c : children) {
            delete c;
        }
    }

// Return the n-th child of the tree such that the rule LHS 
// (for nonterminal nodes) or the token kind (for terminal nodes)
//  matches the string.
Node* getChild(std::string str, int n = 1) {
    int count = 0;
    for (auto &child : children) {
        if ((child->lhs == str || child->kind == str) && ++count == n) {
            return child;
        }
    }
    return nullptr; // return null pointer if no such child is found
}



void print(std::string prefix = "", bool isLast = true, std::ostream &out = std::cout) {
    if (!prefix.empty()) {
        // Remove the single whitespace at the start of each line
        out << prefix.substr(2);
        out << (isLast ? "╰─" : "├─");
    }

    if (!kind.empty()) {
        out << kind << " ";
    }

    // Check if lexeme is empty, if it is, print lhs, else print lexeme
    if (lexeme.empty()) {
        out << lhs << "\n";
    } else {
        out << lexeme << "\n";
    }

    // Update the prefix for children
    std::string childPrefix = prefix + (isLast ? "  " : "│ ");

    // Print the children
    for (size_t i = 0; i < children.size(); ++i) {
        children[i]->print(childPrefix, i == children.size() - 1, out);
    }
}

};


std::vector<Node*> getChildren(Node* node, const std::string& type) {
    std::vector<Node*> children;
    for (auto& child : node->children) {
        if (child->lhs == type) {
            children.push_back(child);
        }
    }
    return children;
}


string line;

//Create Parse Tree
Node* build_tree() {
    getline(std::cin, line);
    // cout << "line:" << line << endl;
    Node* node = new Node(line);
    istringstream iss(line);
    iss >> node->lhs;
    string symbol;

    while (iss >> symbol) {
         if(symbol == ".EMPTY") {
            continue;
        }
        // cout << "symbol: " << symbol << endl;
        node->rhs.push_back(symbol);
        if (isupper(symbol[0])) {
            node->isTerminal = false;
            getline(std::cin, line);
            // cout << "Upper: " << line << endl;
            Node* leaf = new Node(line);
            istringstream iss(line);
            iss >> leaf->kind >> leaf->lexeme;
            node->children.push_back(leaf);
        } else {
            node->isTerminal = true; 
            node->children.push_back(build_tree());
        }
    }
    return node;
}


// A Variable struct or class, which has a name field and a type field.
//  It should have a constructor function which, 
// given a tree whose root rule has dcl on the left-hand side, 
// produces a Variable instance with the name and type filled in.
struct Variable {
    std::string name;
    std::string type;

    Variable(Node* node) {
        if (node->lhs == "dcl") {
            Node* typeNode = node->getChild("type", 1);
            type = typeNode->children[0]->lexeme;
            // node->print();
            // typeNode->print();
            // cout << type << endl;
            // cout << typeNode->children.size() << endl;
            if (typeNode->children.size() > 1) {
                // cout << node->getChild("type",1) -> children[1] << endl;
                // cout << node->getChild("type",1) -> children[1] -> lexeme << endl;
                type += "*";
            }
            name = node->getChild("ID", 1)->lexeme;
        }
    }


    ~Variable() {
        // Destructor code...
    }
    
// struct Variable {
//     std::string name;
//     std::string type;

//     Variable(Node* node) {
//         // cout << node->lhs << endl;
//         if (node->lhs == "dcl") {
//             // cout << "Yes" << endl;
//             type = node->getChild("type", 1) -> children[0]-> lexeme;
//             name = node->getChild("ID", 1) -> lexeme;
//             // type = node->children[0]->children[0] ->kind; 
//             // name = node->children[1]->lexeme;
//         }
//     }



};


// struct VariableTable {
//     std::map<std::string, Variable*> varTable;

//     void Add(Variable* var) {
//         if (varTable.find(var->name) != varTable.end()) {
//             throw std::runtime_error("Duplicate variable declaration");
//         }
//         varTable[var->name] = var;
//     }

//     Variable* Get(std::string name, bool checkL = true) {
//     auto it = varTable.find(name);
//     if (it == varTable.end() && checkL) {
//         throw std::runtime_error("Use of undeclared variable");
//     } else if (it == varTable.end() && !checkL) {
//         return nullptr;
//     }
//     return it->second;
// }


struct VariableTable {
    std::map<std::string, std::shared_ptr<Variable>> varTable;

    void Add(std::shared_ptr<Variable> var) {
        if (varTable.find(var->name) != varTable.end()) {
            // cout << var->name << endl;
            throw std::runtime_error("Duplicate variable declaration");
        }
        varTable[var->name] = var;
    }

    std::shared_ptr<Variable> Get(std::string name, bool checkL = true) {
        auto it = varTable.find(name);
        if (it == varTable.end() && checkL) {
            //  cout << "UND" << endl;
            //  cout << name << endl;
            throw std::runtime_error("Use of undeclared variable");
        } else if (it == varTable.end() && !checkL) {
            return nullptr;
        }
        return it->second;
    }



    // Variable* Get(std::string name, bool checkL= true) {
    //     if (varTable.find(name) == varTable.end() && checkL) {
    //         // cout << "name" << name << endl;
    //         throw std::runtime_error("Use of undeclared variable");
    //     } else if (varTable.find(name) == varTable.end() && checkL == false) {
    //         return nullptr;
    //     }
    //     return varTable[name];
    // }

    //  ~VariableTable() {
    //     // Delete all Variable objects in varTable
    //     for (auto& pair : varTable) {
    //         delete pair.second;
    //     }
    //  }
};



// A Procedure struct or class, which has a name field, a signature field (vector or list of parameter types), 
// and a local symbol table field which is an instance of the VariableTable struct/class. 
// It should have a constructor function which,
//  given a tree whose root rule has procedure or main on the left-hand side,
//   produces a Procedure instance with the name, signature, 
//   and local symbol table filled in.
struct Procedure {
    std::string name;
    std::vector<std::string> signature;
    VariableTable localSymbolTable;



    void extractDcl(Node* node) {

        if (node->lhs == "dcl") {
            std::shared_ptr<Variable> var = std::make_shared<Variable>(node);
        //    Variable* var = new Variable(node);
            // signature.push_back(var->type);
            localSymbolTable.Add(var);
            // delete var;
         } else {
            for (auto &child : node->children) {
                extractDcl(child);
            }
        }
    }




   void checkDcls(Node* node) {
    if (node->rule == "dcls dcls dcl BECOMES NUM SEMI") {
        // Extract the type from the dcl child node
        Node* dclNode = node->getChild("dcl");
        Node* dclNodetype = dclNode->getChild("type");
        std::string type = dclNodetype->rhs[0];
        // dclNodetype->rhs[0]
        // cout << dclNode->rule << endl;
        // cout << dclNodetype->rhs[0] << endl;
        if (type != "INT") {
            // dclNodetype->print();
            // cout << type << endl;
            throw std::runtime_error("Semantic error: Initialization value is a number but declared type is not int");
        }
    } else if (node->rule == "dcls dcls dcl BECOMES NULL SEMI") {
        Node* dclNodeS = node->getChild("dcl");
        Node* dclNodetypeS = dclNodeS->getChild("type");
        std::string typeS = "untyped";
    //    dclNodetypeS->print();
    //      cout <<  dclNodetypeS->children[0]->lexeme << endl;
    //       cout <<  dclNodetypeS->children[1]->lexeme << endl;
         if ( dclNodetypeS->children.size() > 1) {
            if ((dclNodetypeS->children[0]->lexeme == "int") && (dclNodetypeS->children[1]->lexeme == "*")) {
                typeS = "int*";
            }
                if (typeS != "int*") {
            //     dclNodetypeS->print();
            //    cout <<  dclNodetypeS->children[0]->lexeme << endl;
            //    cout <<  dclNodetypeS->children[1]->lexeme << endl;
                throw std::runtime_error("Semantic error: Initialization value is NULL but declared type is not int*");
            }
        } else { 
                throw std::runtime_error("Semantic error: Initialization value is NULL but declared type is not int*");
                }
    }
    else {
        for (auto &child : node->children) {
            checkDcls(child);
        }
    }
    }



   Procedure(Node* node) {
    // cout << node->lhs << endl;
    if (node->lhs == "main") {
        name = "wain";
        checkDcls(node);
        extractDcl(node);

        // Check that the second parameter of wain is of type int
        if (signature.size() > 1 && signature[1] != "int") {
            throw std::runtime_error("Semantic error: The second parameter of wain must be of type int");
        }
    } else if (node->lhs == "procedure") {
        name = node->getChild("ID", 1)->lexeme;
        checkDcls(node);
        extractDcl(node);
    } else if (node->lhs == "procedures" && node->getChild("main")) {
        name = "wain";
        checkDcls(node);
        extractDcl(node);
    // } else if (node->lhs == "procedures" && node->getChild("main")) {
    //     name = "wain";
    //     checkDcls(node);
    //     extractDcl(node);
    } else {
        Node* n =  node->getChild("ID", 1);
        // n->print();
        if (n) {
         name = node->getChild("ID", 1)->lexeme;
        }
        checkDcls(node);
        extractDcl(node);
    }
}

  ~Procedure() {
        // Delete all Variable objects in localSymbolTable
        // for (auto& pair : localSymbolTable.varTable) {
        //     delete pair.second;
        // }
    }

};


 //   Procedure(Node* node) : name(node->lhs) {
    // //   Procedure(Node* node) {
    //     if (node->lhs == "procedure" || node->lhs == "main" || node->lhs == "procedures" ) {
    //         // name = node->getChild("ID", 1)->lexeme;
    //         extractDcl(node);
    //     }
    // }

// A ProcedureTable struct or class, 
// which is a wrapper for a map of strings (procedure names) to i
// instances of the Procedure struct/class. It should support the following operations 
// ADD and Get
struct ProcedureTable {
    std::map<std::string, Procedure*> procTable;

    void Add(Procedure* proc) {
        if (procTable.find(proc->name) != procTable.end()) {
            // cout << proc->name << endl;
            throw std::runtime_error("Duplicate procedure declaration");
        }
        procTable[proc->name] = proc;
    }

    Procedure* Get(std::string name, bool getC=true) {
        if (procTable.find(name) == procTable.end() && getC) {
            throw std::runtime_error("Use of undeclared procedure");
        } else if (procTable.find(name) == procTable.end() && getC == false) {
             return nullptr;
        }
        return procTable[name];
    }

        ~ProcedureTable() {
        // Delete all Procedure objects in procTable
        for (auto& pair : procTable) {
            delete pair.second;
        }
    }
};


// void collectProcedures(Node* root, ProcedureTable& pt) {
//     // Get the 'procedures' child node
//     Node* proceduresNode = root->getChild("procedures", 1);

//     // Check if the 'procedures' node exists
//     if (proceduresNode != nullptr) {
//         // Iterate over the children of the 'procedures' node in order
//         for (auto &child : proceduresNode->children) {
//                 // Create a new Procedure instance and add it to the ProcedureTable
//                 Procedure* proc = new Procedure(child);
//                 pt.Add(proc);
//             }
//         }
//     }



void annotateTypes(Node* node, Procedure* currentProcedure, ProcedureTable& pt) {
    // First, recursively call annotateTypes on each child
    for (auto &child : node->children) {
        annotateTypes(child, currentProcedure, pt);
    }

    // Then, implement the type computation rules

    // The type of a NUM is int.
    if (node->kind == "NUM") {
        node->type = "int";

       // The type of a NULL token is int*.
    } else if (node->kind == "NULL") {
        node->type = "int*";

    //ID Nodes
    //  } else if (node->lhs == "dcl" && (node->rhs[0] == "type" || node->rhs[0] == "ID")) {
    //      node->type = currentProcedure->localSymbolTable.Get(node->children[0]->lexeme)->type;


    } else if (node->lhs == "ID") {
    node->type = currentProcedure->localSymbolTable.Get(node->lexeme)->type;

        // type of a factor deriving NUM or NULL is the same as the type of that token.
    } else if (node->lhs == "factor" && (node->rhs[0] == "NUM" || node->rhs[0] == "NULL")) {
        if (node->rhs[0] == "NUM") {
            node->type = "int";
         } else {
            node->type = "int*";
         }
        // node->type = node->rhs[0]->currentProcedure->localSymbolTable.Get(node->children[0]->lexeme)->type;

        // factor/lvalue  derives ID, the derived ID must have a type, and the type 
        // of the factor is the same as the type of the ID.        
    } else if ((node->lhs == "factor" || node->lhs == "lvalue") && node->rhs[0] == "ID" && node->rhs.size() == 1) {
        // node->print();
        // node->getChild("ID")->print() ;
        Node* idChild = node->getChild(node->rhs[0]);
        // cout << node->getChild(node->rhs[0])->lexeme << endl;
        if (idChild) {
            // idChild->print();
            // cout << idChild->lexeme << endl;
            std::shared_ptr<Variable> varptr = currentProcedure->localSymbolTable.Get(idChild->lexeme);
            if (varptr) {
                // cout << "YUP" << var->name << endl;
                // cout << var->type << endl;
                Variable var = *varptr;
                node->type = var.type;
                // cout << node->type << endl;
            } else {
        throw std::runtime_error("Variable not found in symbol table");
    }
            //  cout << currentProcedure->localSymbolTable.Get( idChild->lexeme )->type << endl;
            // node->type = currentProcedure->localSymbolTable.Get( idChild->lexeme )->type;
            // cout << node->type << endl;
        }
        //  type of a factor deriving LPAREN expr RPAREN is the same as the type of the expr.        
    } else if (node->lhs == "factor" && node->rhs[0] == "LPAREN" && node->rhs[1] == "expr" && node->rhs[2] == "RPAREN") {
        node->type = node->children[1]->type;

        // The type of an lvalue deriving LPAREN lvalue RPAREN is the same as the type of the derived lvalue.
    } else if (node->lhs == "lvalue" && node->rhs[0] == "LPAREN" && node->rhs[1] == "lvalue" && node->rhs[2] == "RPAREN") {
        node->type = node->children[1]->type;

        // type of a factor deriving AMP lvalue is int*.
        //  The type of the derived lvalue (i.e. the one preceded by AMP) must be int.
    } else if (node->lhs == "factor" && node->rhs[0] == "AMP" && node->rhs[1] == "lvalue") {
        if (node->children[1]->type == "int") {
            node->type = "int*";
        } else {
            throw std::runtime_error("ERROR: Type mismatch error5");
        }

        // The type of a factor or lvalue deriving STAR factor is int. 
        // The type of the derived factor (i.e. the one preceded by STAR) must be int*.        
    } else if ((node->lhs == "factor" || node->lhs == "lvalue") && node->rhs[0] == "STAR" && node->rhs[1] == "factor") {
    //     node->children[1]->print();
    //    cout << node->children[1]->type << endl;;
        if (node->children[1]->type == "int*") {
            node->type = "int";
        } else {
            throw std::runtime_error("ERROR: Type mismatch error4");
        }

        // The type of a factor deriving NEW INT LBRACK expr RBRACK is int*. 
        // The type of the derived expr must be int.
    } else if (node->lhs == "factor" && node->rhs[0] == "NEW" && node->rhs[1] == "INT"  && node->rhs[2] == "LBRACK" && node->rhs[3] == "expr" && node->rhs[4] == "RBRACK") {
    //           node->children[3]->print();
    //    cout << node->children[3]->type << endl;;
        if (node->children[3]->type == "int") {
            node->type = "int*";
        } else {
            throw std::runtime_error("ERROR: Type mismatch error3");
        }

        // The type of a factor deriving ID LPAREN RPAREN is int.x
        //  The procedure whose name is ID must have an empty signature.
    } else if (node->lhs == "factor" && node->rhs[0] == "ID" && node->rhs[1] == "LPAREN" && node->rhs[2] == "RPAREN") {
      std::string procedureName = node->children[0]->lexeme;
       node->type = "int";

    // Check if the ID's lexeme is in the local VariableTable for the current procedure
    if (currentProcedure->localSymbolTable.Get(procedureName, false) != nullptr) {
        // Handle the case where ID refers to a local variable
        // throw std::runtime_error("ID refers to a local variable, not a procedure");
        return;
    } else {
        // Check if the ID's lexeme is in the top-level ProcedureTable
        Procedure* calledProcedure = pt.Get(procedureName, false);

        // If the lexeme is not in the ProcedureTable, it's an error
        if (calledProcedure == nullptr) {
            //  cout << procedureName << endl;
            throw std::runtime_error("Use of undeclared procedure");
        }

        // If the rule is factor → ID LPAREN RPAREN, the call has no arguments
        if (!calledProcedure->signature.empty()) {
            // cout << "YERDSHHSSDH" << endl;
            throw std::runtime_error("ERROR: Type mismatch error: Procedure call with no arguments, but signature is not empty");
        }
    }

        // The type of a factor deriving ID LPAREN arglist RPAREN is int. 
        // The procedure whose name is ID must have a signature whose length is equal to the number of expr strings (separated by COMMA)
        // that are derived from arglist. 
        // Furthermore, the types of these expr strings must exactly match, in order, the types in the procedure's signature.
    } else if (node->lhs == "factor" && node->rhs[0] == "ID" && node->rhs[1] == "LPAREN" && node->rhs[2] == "arglist" && node->rhs[3] == "RPAREN") {
        // cout << node->children[0]->lexeme << endl;
        Procedure* proc = pt.Get(node->children[0]->lexeme);
        std::vector<Node*> args = getChildren(node->children[2], "expr");

        if (proc->signature.size() == args.size()) {
            for (size_t i = 0; i < args.size(); ++i) {
                if (proc->signature[i] != args[i]->type) {
                    // cout << i << endl;
                    // cout << "sig: " << proc->signature[i] << endl;
                    // cout << "args: " << args[i]->type << endl;
                    throw std::runtime_error("ERROR: Type mismatch error7");
                }
            }
            node->type = "int";
        } else {
            throw std::runtime_error("ERROR: Type mismatch error17");
        }

        // The type of a term deriving factor is the same as the type of the derived factor.
    } else if (node->lhs == "term" && node->rhs.size() == 1  && node->rhs[0] == "factor") {
        node->type = node->getChild("factor")->type;

        // The type of a term directly deriving anything other than just factor is int.
        //  The term and factor directly derived from such a term must have type int.
    } else if (node->lhs == "term" && node->rhs.size() > 1) {
        if (node->children[0]->type == "int" && node->children[2]->type == "int") {
            node->type = "int";
        } else {
            throw std::runtime_error("ERROR: Type mismatch error2");
        }

        // The type of an expr deriving term is the same as the type of the derived term
    } else if (node->lhs == "expr" && node->rhs.size() == 1 && node->rhs[0] == "term") {
        node->type = node->getChild("term")->type;

        // cout <<  node->type << endl;
        // node->children[0]->print();

    // When expr derives expr PLUS term:
    // The derived expr and the derived term may both have type int, in which case the type of the expr deriving them is int.
    // The derived expr may have type int* and the derived term may have type int, in which case the type of the expr deriving them is int*.
    // The derived expr may have type int and the derived term may have type int*, in which case the type of the expr deriving them is int*.

    //     When expr derives expr MINUS term:
    // The derived expr and the derived term may both have type int, in which case the type of the expr deriving them is int.
    // The derived expr may have type int* and the derived term may have type int, in which case the type of the expr deriving them is int*.
    // The derived expr and the derived term may both have type int*, in which case the type of the expr deriving them is int.
   } else if (node->lhs == "expr" && node->rhs.size() > 1) {
    if (node->children[0]->type == "int" && node->children[2]->type == "int") {
        // The derived expr and the derived term may both have type int
        node->type = "int";
    } else if (node->children[0]->type == "int*" && node->children[2]->type == "int" && node->rhs[1] == "PLUS") {
        // The derived expr may have type int* and the derived term may have type int
        node->type = "int*";
    } else if (node->children[0]->type == "int" && node->children[2]->type == "int*" && node->rhs[1] == "PLUS") {
        // The derived expr may have type int and the derived term may have type int*
        node->type = "int*";
    } else if (node->children[0]->type == "int*" && node->children[2]->type == "int" && node->rhs[1] == "MINUS") {
        // The derived expr may have type int* and the derived term may have type int
        node->type = "int*";
    } else if (node->children[0]->type == "int*" && node->children[2]->type == "int*" && node->rhs[1] == "MINUS") {
        // The derived expr and the derived term may both have type int*
        node->type = "int";
    } else {
        throw std::runtime_error("ERROR: Type mismatch error1");
    }
} else {
    node->type = "untyped";
    // node->type = node->children[0]->type;
    }
}


void checkStatements(Node* node, Procedure* currentProcedure) {

    // Recursively call checkStatements on each child
    for (auto &child : node->children) {
        checkStatements(child, currentProcedure);
    }

    if (node->lhs == "statement") {
        if (node->rhs[0] == "lvalue" && node->rhs[1] == "BECOMES" && node->rhs[2] == "expr" && node->rhs[3] == "SEMI") {
            // Check that the types of lvalue and expr are the same
            if (node->getChild("lvalue")->type != node->getChild("expr")->type) {
                throw std::runtime_error("ERROR: Semantic error: The types of lvalue and expr must be the same");
            }
        } else if (node->rhs[0] == "PRINTLN" && node->rhs[1] == "LPAREN" && node->rhs[2] == "expr"  && node->rhs[3] == "RPAREN"  && node->rhs[4] == "SEMI") {
            // Check that the type of expr is int
            if (node->getChild("expr")->type != "int") {
                throw std::runtime_error("ERROR: Semantic error: The type of expr must be int");
            }
        } else if (node->rhs[0] == "DELETE" && node->rhs[1] == "LBRACK" && node->rhs[2] == "RBRACK" && node->rhs[3] == "expr" && node->rhs[4] == "SEMI") {
            // Check that the type of expr is int*
            if (node->getChild("expr")->type != "int*") {
                throw std::runtime_error("ERROR: Semantic error: The type of expr must be int*");
            }
        }
    } else if (node->lhs == "test") {
        // Check that the types of the two expr nodes are the same
        if (node->getChild("expr", 1)->type != node->getChild("expr", 2)->type) {
            throw std::runtime_error("ERROR: Semantic error: The types of the two expr nodes must be the same");
        }
    }
}


// collectProcedures which traverses the procedures subtree of the parse
//  tree root and creates or fills out a ProcedureTable with all the procedures 
//  in the program by calling the Procedure constructor on the appropriate subtrees, 
//  then adding the newly constructed Procedure to the table using the Add operation.
void collectProcedures(Node* root, ProcedureTable& pt) {
    // Check if the current node is a 'procedure'
    if (root->lhs == "procedure" || root->lhs == "main" ) {
        // Create a new Procedure instance and add it to the ProcedureTable
        Procedure* proc = new Procedure(root);
        pt.Add(proc);

        // Call annotateTypes for the procedure
        annotateTypes(root, proc, pt);

        checkStatements(root, proc);  // Call checkStatements here

        // Check that the return type of the expr derived from procedure is int
        Node* exprNode = root->getChild("expr");
        if (exprNode->type != "int") {
            // cout <<  exprNode->type << endl;
            throw std::runtime_error("ERROR: Semantic error: The return type of the procedure must be int");
        }
    }

    // Iterate over the children of the current node
    for (auto &child : root->children) {
        // Recursively call collectProcedures on each child
        collectProcedures(child, pt);
    }


        //  The expr in the sequence directly derived from main must have type int.
        // The second dcl in the sequence directly derived from main must derive a type that derives INT.
        // Check for the 'main' node
            if (root->lhs == "main") {
                // The second dcl in the sequence directly derived from main must derive a type that derives INT
                Node* dclNodeM = root->getChild("dcl", 2);
                Node* dclNodeM2 = dclNodeM->getChild("type");
                // dclNodeM2->print();
                // cout << dclNodeM2->type << endl;
                // cout << dclNodeM2->rule << endl;
                // cout << dclNodeM2->lhs << endl;
                //  cout << dclNodeM2->rhs[0] << endl;
                // if (dclNodeM2->type != "int") 
                if (dclNodeM2->rhs[0] != "INT") {
                    throw std::runtime_error("ERROR: Semantic error: The second dcl in the sequence directly derived from main must derive a type that derives INT");
                }

                // The expr in the sequence directly derived from main must have type int
                Node* exprNode = root->getChild("expr");
                // exprNode->print();
                if (exprNode->type != "int") {
                    // cout << exprNode->type << endl;

                    throw std::runtime_error("ERROR: Semantic error: The expr in the sequence directly derived from main must have type int");
                }
            }
            // delete proc;
        }






void printNodeProperties(Node* node, int depth = 0) {
    // Print indentation
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    // Print node properties
    std::cout << "Rule: " << node->rule << "\n";
    std::cout << "LHS: " << node->lhs << "\n";
    std::cout << "RHS: ";
    for (const auto& s : node->rhs) {
        std::cout << s << " ";
    }
    std::cout << "\n";
    std::cout << "Kind: " << node->kind << "\n";
    std::cout << "Lexeme: " << node->lexeme << "\n";
    std::cout << "Is Terminal: " << (node->isTerminal ? "Yes" : "No") << "\n";
    std::cout << "Type: " << node->type << "\n";

    // Recursively print properties of child nodes
    for (auto &child : node->children) {
        printNodeProperties(child, depth + 1);
    }
}



int main() {
   
        Node* root = build_tree();
         try {
        // root->print();
        // cout << endl;
        // Node* child = root->getChild("procedures", 2);
        // if (child != nullptr) {
        //     cout << child->lhs << endl; // This will print the lhs of the child if it's not null
        // } else {
        //     cout << "Child not found" << endl;
        // }

        // Variable var(root);

        //  std::cout << "Name: " << var.name << ", Type: " << var.type << std::endl;



        // // Create a new VariableTable
        // VariableTable vt;

        // Variable var(root);

        // // Add the Variable to the VariableTable
        // vt.Add(&var);

        // // Retrieve the Variable from the VariableTable
        // //a in table
        // Variable* retrievedVar = vt.Get("a");

        // // Print the name and type of the retrieved Variable
        // std::cout << "Name: " << retrievedVar->name << ", Type: " << retrievedVar->type << std::endl;


         ProcedureTable procTable1;

        //   printNodeProperties(root);

        // cout << endl;
         // Call collectProcedures to fill out the ProcedureTable
        collectProcedures(root, procTable1);

//         // Iterate over the ProcedureTable and print each procedure
// for (auto &proc : procTable1.procTable) {
//         cout << "proc name: " << proc.first << endl;
//         cout << "proc signatures: " << proc.second->signature.size() << endl;
//         for (auto &sig : proc.second->signature) {
//             cout << "sig: " << sig << endl;
//         }
//        // Print local symbol table information
//     cout << "proc symbolTable size: " << proc.second->localSymbolTable.varTable.size() << endl;
//     for (auto &var : proc.second->localSymbolTable.varTable) {
//         cout << "var name: " << var.second->name << endl;
//         cout << "var type: " << var.second->type << endl;
//     }

//     cout << endl;
//     cout << endl;


// }
 delete root;

    } catch(std::runtime_error &e) {
         delete root;
                std::cerr << "ERROR: " << e.what() << "\n";
                return 1;
 }

    return 0;



}



// g++ -g -Wall -std=c++17 wlp4type-preparsed.cc -o seman
// ./seman < preparsed1.in 




//  marmoset_submit CS241 P4 wlp4type-preparsed.cc 












// Procedure(Node* node) : name(node->lhs) {
    //     if (node->lhs == "procedure" || node->lhs == "main" || node->lhs == "procedures" ) {

    //         // name = node->getChild("ID", 1)->lexeme;

    //         Node* params = node->getChild("params", 1);

    //         //parameters are in the params subtree for non-wain procedures
    //         if (params) {
    //             for (auto &child : params->children) {

    //                 if (child->lhs == "dcl") {
    //                     Variable* var = new Variable(child);
    //                     signature.push_back(var->type);
    //                     localSymbolTable.Add(var);
    //                 }
    //             }
    //         }

    //         // the two dcl children of the main node are the parameters.
    //         if (node->lhs == "main") {
    //             for (int i = 0; i < node->children.size(); i++) {
    //                 if (node->children[i]->lhs == "dcl") {
    //                     Variable* var = new Variable(node->children[i]);
    //                     signature.push_back(var->type);
    //                     localSymbolTable.Add(var);
    //                 }
    //             }
    //         }
    //         // if (node->lhs == "main") {
    //         //     for (int i = 0; i < 2; i++) {
    //         //         Node* dcl = node->getChild("dcl", i);
                    
    //         //         if (dcl) {
    //         //         Variable* var = new Variable(dcl);
    //         //         signature.push_back(var->type);
    //         //         localSymbolTable.Add(var);
    //         //     }
    //         //     }
    //         // }

    //         //non-parameter local variables are in the dcls subtree
    //         Node* dcls = node->getChild("dcls", 1);

    //         if (dcls) {
    //             for (auto &child : dcls->children) {
    //                 if (child->lhs == "dcl") {
    //                     Variable* var = new Variable(child);
    //                      signature.push_back(var->type);
    //                     localSymbolTable.Add(var);
    //                 }
    //             }
    //         }

    //     }
    // }