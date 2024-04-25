
//// The dfa.h header defines a string constant containing a DFA file:

#include "dfa.h"

using namespace std;


//// Function that takes a DFA file (passed as a stream) and prints information about it.
void DFAprint(const std::string& in, DFA& dfa);

//// These helper functions are defined at the bottom of the file:
// Check if a string is a single character.
bool isChar(std::string s);
// Check if a string represents a character range.
bool isRange(std::string s);
// Remove leading and trailing whitespace from a string, and
// squish intermediate whitespace sequences down to one space each.
std::string squish(std::string s);
// Convert hex digit character to corresponding number.
int hexToNum(char c);
// Convert number to corresponding hex digit character.
char numToHex(int d);
// Replace all escape sequences in a string with corresponding characters.
std::string escape(std::string s);
// Convert non-printing characters or spaces in a string into escape sequences.
std::string unescape(std::string s);

DFA createDFAFromStream(std::istream& input);

const std::string DFAstring = R"(
.STATES
start
ID!
I!
IN!
INT!
N!
NU!
NUS!
NUM!
F!
IF!
E!
EL!
ELS!
ELSE!
W!
WA!
WAI!
WAIN!
WH!
WHI!
WHIL!
WHILE!
P!
PR!
PRI!
PRIN!
PRINT!
PRINTL!
PRINTLN!
R!
RE!
RET!
RETU!
RETUR!
RETURN!
NE!
NEW!
D!
DE!
DEL!
DELE!
DELET!
DELETE!
NUL!
NULL!
LPAREN!
RPAREN!
LBRACE!
RBRACE!
LBRACK!
RBRACK!
BECOMES!
PLUS!
MINUS!
STAR!
PCT!
AMP!
COMMA!
SLASH!
SEMI!
LT!
GT!
LE!
GE!
EQ!
EXC
NES!
?WHITESPACE!
?COMMENT!
.TRANSITIONS
start i I
I n IN
I f IF
IF a-z A-Z 0-9 ID
I a-e g-m o-z A-Z 0-9 ID
IN t INT
IN a-s A-Z u-z 0-9 ID
INT a-z A-Z 0-9 ID
start w W
W a WA
W b-g i-z A-Z 0-9 ID
WA i WAI
WA a-h j-z A-Z 0-9 ID
WAI n WAIN
WAI a-m o-z A-Z 0-9 ID
WAIN a-z A-Z 0-9 ID
W h WH
WH i WHI
WH a-h j-z A-Z 0-9 ID
WHI l WHIL
WHI a-k m-z A-Z 0-9 ID
WHIL e WHILE
WHIL a-d f-z A-Z 0-9 ID
WHILE a-z A-Z 0-9 ID
start e E
E l EL
E a-k m-z A-Z 0-9 ID
EL s ELS
EL a-r t-z A-Z 0-9 ID
ELS e ELSE
ELS a-d f-z A-Z 0-9 ID
ELSE a-z A-Z 0-9 ID
start p P
P r PR
P a-q s-z A-Z 0-9 ID
PR i PRI
PR a-h j-z A-Z 0-9 ID
PRI n PRIN
PRI a-m o-z A-Z 0-9 ID
PRIN t PRINT
PRIN a-s u-z A-Z 0-9 ID
PRINT l PRINTL
PRINT a-k m-z A-Z 0-9 ID
PRINTL n PRINTLN
PRINTL a-m o-z A-Z 0-9 ID
PRINTLN a-z A-Z 0-9 ID
start r R
R e RE
R a-d f-z A-Z 0-9 ID
RE t RET
RE a-s u-z A-Z 0-9 ID
RET u RETU
RET a-t v-z A-Z 0-9 ID
RETU r RETUR
RETU a-q s-z A-Z 0-9 ID
RETUR n RETURN
RETUR a-m o-z A-Z 0-9 ID
RETURN a-z A-Z 0-9 ID
start N NUS
start n N
N e NE
N A-Z a-d f-z 0-9 ID
NUS U NU
NUS a-z A-T V-Z 0-9 ID
NE w NEW
NEW a-z A-Z 0-9 ID
NE a-v x-z A-Z 0-9 ID
NU L NUL
NU A-K M-Z a-z 0-9 ID
NUL L NULL
NUL A-K M-Z a-z 0-9 ID
NULL a-z A-Z 0-9 ID
start d D
D e DE
D a-d f-z A-Z 0-9 ID
DE l DEL
DE a-k m-z A-Z 0-9 ID
DEL e DELE
DEL a-d f-z A-Z 0-9 ID
DELE t DELET
DELE a-s u-z A-Z 0-9 ID
DELET e DELETE
DELET a-d f-z A-Z 0-9 ID
DELETE a-z A-Z 0-9 ID
start ( LPAREN
start ) RPAREN
start { LBRACE
start } RBRACE
start [ LBRACK
start ] RBRACK
start = BECOMES
BECOMES = EQ
start + PLUS
start - MINUS
start * STAR
start / SLASH
start % PCT
start & AMP
start , COMMA
start ; SEMI
start < LT
start > GT
LT = LE
GT = GE
start ! EXC
EXC = NES
start   \s \t \n \r ?WHITESPACE
?WHITESPACE \s \t \n \r ?WHITESPACE
SLASH / ?COMMENT
?COMMENT \x00-\x09 \x0B \x0C \x0E-\x7F ?COMMENT
start a-c f-h j-m o q s-v x-z A-M O-Z ID
ID a-z A-Z 0-9 ID
start 0-9 NUM
NUM 0-9 NUM)";


class Token {
public:
    std::string kind;
    std::string lexeme;

    Token(const std::string& kind, const std::string& lexeme) : kind(kind), lexeme(lexeme) {}
};

const std::string STATES      = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT       = ".INPUT";
const std::string UNDEFINED   = "UNDEFINED";



int main() {
    // std::vector<Token> tokens4;
    DFA myDFA;
  try {
  //  myDFA.init(DFAstring);
    std::string line;
    DFAprint(DFAstring, myDFA);
    // myDFA.printStates();
    // myDFA.printTransitions();
    while (std::getline(std::cin, line)) {
      std::istringstream s(line); // Use std::istringstream to parse each line
      //createDFAFromStream(s);
      // string input4 = "lw ?";
      myDFA.simplifiedMaximalMunch(line, myDFA);
    }
  } catch(std::runtime_error &e) {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  } catch (const std::invalid_argument& e) {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
    } catch (const std::out_of_range& e) {
      std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
     }

  for (const Token& token : myDFA.tokens) {
    if (token.kind.find("?") != 0 && token.kind != "SPACE"  && token.kind != "NEWLINE" ) {
        std::cout << token.kind << " " << token.lexeme << std::endl;
    }
}


  
  return 0;
}


void DFAprint(const std::string& in, DFA& dfa) {
  std::istringstream iss(in); // Create a stringstream from the input string
  std::string s;
  // Skip blank lines at the start of the file
  while(true) {
   if (!std::getline(iss, s)) {
      throw std::runtime_error
        ("Expected " + STATES + ", but found end of input.");
    }
    s = squish(s);
    if (s == STATES) {
      break;
    }
    if (!s.empty()) {
      throw std::runtime_error
        ("Expected " + STATES + ", but found: " + s);
    }
  }
  // Print states
  // std::cout << "States:" << '\n';
  bool initial = true;
  while(true) {
     if (!std::getline(iss, s)) {
      throw std::runtime_error
        ("Unexpected end of input while reading state set: " 
         + TRANSITIONS + "not found.");
    }
    if (s == TRANSITIONS) {
      break; 
    } 
    // Process an individual state
    bool accepting = false;
    if (s.back() == '!' && s.length() > 1) {
      accepting = true;
      s.pop_back();
    }
    dfa.addState(s);
    if (accepting) {
      dfa.setAccepting(s, accepting);
    }
    // std::cout << s 
    //           << (initial   ? " (initial)"   : "") 
    //           << (accepting ? " (accepting)" : "") << '\n';
    // Only the first state is marked as initial
    initial = false;
  }
  // Print transitions
  //std::cout << "Transitions:" << '\n';
  //std::getline(iss, s); // Skip .TRANSITIONS header
  while(true) {
   if (!std::getline(iss, s)) {
      // We reached the end of the file
      break;
    }
    s = squish(s);
    if (s == INPUT) {
      break;
    } 
    // Split the line into parts
    std::string lineStr = s;
    std::stringstream line(lineStr);
    std::vector<std::string> lineVec;
    while(line >> s) {
      // cout << "line" << s;
      lineVec.push_back(s);
    }
    if(lineVec.empty()) {
      // Skip blank lines
      continue;
    }
    if (lineVec.size() < 3) {
      throw std::runtime_error
        ("Incomplete transition line: " + lineStr);
    }
    // Extract state information from the line
    std::string fromState = lineVec.front();
    std::string toState = lineVec.back();
    // cout << "from" << fromState;
    // cout << "to" << toState;
    // Extract character and range information from the line
    std::vector<char> charVec;
    for(int i = 1; i < lineVec.size()-1; ++i) {
      std::string charOrRange = escape(lineVec[i]);
      if (isChar(charOrRange)) {
        char c = charOrRange[0];
        if (c < 0 || c > 127) {
          throw std::runtime_error
            ("Invalid (non-ASCII) character in transition line: " + lineStr + "\n"
             + "Character " + unescape(std::string(1,c)) + " is outside ASCII range");
        }
        charVec.push_back(c);
      } else if (isRange(charOrRange)) {
        for(char c = charOrRange[0]; charOrRange[0] <= c && c <= charOrRange[2]; ++c) {
          charVec.push_back(c);
        }
      } else {
        throw std::runtime_error
          ("Expected character or range, but found "
           + charOrRange + " in transition line: " + lineStr);
      }
    }
   // vector<char> validChars = charVec;
    dfa.addTransition(fromState, charVec, toState);
    // Print a representation of the transition line
    // std::cout << fromState << ' ';
    // for ( char c : charVec ) {
    //   std::cout << unescape(std::string(1,c)) << ' ';
    // }
    // std::cout << toState << '\n';
   }
  // We ignore .INPUT sections, so we're done
  // dfa.printStates();
  // dfa.printTransitions();
  //  cout << dfa.nextState("start", '(') << endl;
  //  cout << dfa.nextState("start", ')') << endl;
  //cout << dfa.nextState("start", 'A') << endl;
   //cout << dfa.nextState("start", 'l') << endl;
  // string input1 = "lis $4";
  // std::vector<Token> tokens = dfa.simplifiedMaximalMunch(input1);

  // string input2 = "lw $3, 0xfffc($4) ; what does this do?";
  // std::vector<Token> tokens2 = dfa.simplifiedMaximalMunch(input2);

  // for (const Token& token : tokens) {
  //     std::cout << token.kind << token.lexeme << std::endl;
  // }

  // for (const Token& token : tokens2) {
  //     std::cout << token.kind << " " << token.lexeme << std::endl;
  // }

  // string input4 = "lw ?";
  // std::vector<Token> tokens4 = dfa.simplifiedMaximalMunch(input4);

  // for (const Token& token : tokens4) {
  //     std::cout << token.kind << token.lexeme << std::endl;
  // }
}


//// Helper functions

bool isChar(std::string s) {
  return s.length() == 1;
}

bool isRange(std::string s) {
  return s.length() == 3 && s[1] == '-';
}

std::string squish(std::string s) {
  std::stringstream ss(s);
  std::string token;
  std::string result;
  std::string space = "";
  while(ss >> token) {
    result += space;
    result += token;
    space = " ";
  }
  return result;
}

int hexToNum(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('a' <= c && c <= 'f') {
    return 10 + (c - 'a');
  } else if ('A' <= c && c <= 'F') {
    return 10 + (c - 'A');
  }
  // This should never happen....
  throw std::runtime_error("Invalid hex digit!");
}

char numToHex(int d) {
  return (d < 10 ? d + '0' : d - 10 + 'A');
}

std::string escape(std::string s) {
  std::string p;
  for(int i=0; i<s.length(); ++i) {
    if (s[i] == '\\' && i+1 < s.length()) {
      char c = s[i+1]; 
      i = i+1;
      if (c == 's') {
        p += ' ';            
      } else
      if (c == 'n') {
        p += '\n';            
      } else
      if (c == 'r') {
        p += '\r';            
      } else
      if (c == 't') {
        p += '\t';            
      } else
      if (c == 'x') {
        if(i+2 < s.length() && isxdigit(s[i+1]) && isxdigit(s[i+2])) {
          if (hexToNum(s[i+1]) > 8) {
            throw std::runtime_error(
                "Invalid escape sequence \\x"
                + std::string(1, s[i+1])
                + std::string(1, s[i+2])
                +": not in ASCII range (0x00 to 0x7F)");
          }
          char code = hexToNum(s[i+1])*16 + hexToNum(s[i+2]);
          p += code;
          i = i+2;
        } else {
          p += c;
        }
      } else
      if (isgraph(c)) {
        p += c;            
      } else {
        p += s[i];
      }
    } else {
       p += s[i];
    }
  }  
  return p;
}

std::string unescape(std::string s) {
  std::string p;
  for(int i=0; i<s.length(); ++i) {
    char c = s[i];
    if (c == ' ') {
      p += "\\s";
    } else
    if (c == '\n') {
      p += "\\n";
    } else
    if (c == '\r') {
      p += "\\r";
    } else
    if (c == '\t') {
      p += "\\t";
    } else
    if (!isgraph(c)) {
      std::string hex = "\\x";
      p += hex + numToHex((unsigned char)c/16) + numToHex((unsigned char)c%16);
    } else {
      p += c;
    }
  }
  return p;
}



//ctor
DFA::DFA() {
   states.clear();
    accepting_states.clear();
    transitions.clear();

}


void DFA::addState(const string& state) {
    states.insert(state);
}


void DFA::setAccepting(const string& state, bool isAccepting) {
    accepting_states[state] = isAccepting;
}


void DFA::addTransition(const std::string& from, const std::vector<char>& validChars, const std::string& to) {
    Transition transition;
    transition.fromState = from;
    transition.toState = to;
    transition.validChars = validChars;

    transitions.push_back(transition);
    transition_chars[from] = validChars;
}

DFA::Transition DFA::findTransition(const std::string& from, char input) {
  //  cout << "from" << from << endl;
  //  cout << "in" << input << endl;
    for (const Transition& transition : transitions) {
        if (transition.fromState == from) {
            for (char c : transition.validChars) {
                if (c == input) {
                  //  cout << "to: "<< transition.toState << endl;
                    return transition;
                }
            }
        }
    }
    // Return an empty transition or handle error as needed
    return Transition{};
}


bool DFA::isAcceptingState(const string& state) {
    return accepting_states[state];
}

std::string DFA::nextState(const std::string& currentState, char input) {
    Transition transition = findTransition(currentState, input);
    //if no next state
    if (transition.fromState.empty()) {
      return UNDEFINED;
    } else {
    return transition.toState;
    }
}


void DFA::printStates() {
        std::cout << "States:" << std::endl;
        for (const std::string& state : states) {
            std::cout << state;
            if (accepting_states[state]) {
                std::cout << " (accepting)";
            }
            std::cout << std::endl;
        }
    }


void DFA::printTransitions() {
    std::cout << "Transitions:" << std::endl;
    for (const Transition& transition : transitions) {
        const std::string& fromState = transition.fromState;
        const std::vector<char>& validChars = transition.validChars;
        const std::string& toState = transition.toState;

        std::cout << fromState << " [ ";
        for ( char c : validChars ) {
        std::cout << unescape(std::string(1,c)) << ' ';
        }
        std::cout << "] " << toState << std::endl;

        // std::cout << fromState << " [ ";
        // for (char inputChar : validChars) {
        //     std::cout << inputChar << " ";
        // }
        // std::cout << "] " << toState << std::endl;
    }
}

bool isNumeric(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> std::noskipws >> num;  // Attempt to read a number with optional leading whitespace
    return iss.eof() && !iss.fail();
}

// std::vector<Token> DFA::simplifiedMaximalMunch(const string& input, DFA& dfa) {
//     string remainingInput = input;
//     cout << "input: " << input << endl;
//     string tokenRead = "";
//     string currentState = "start"; // Initial state

//     while (!remainingInput.empty()) {
//         // cout << "rem:" << remainingInput << endl;
//         char nextChar = remainingInput[0];
//         // cout << "next: " << nextChar << endl;
//         if (nextChar == '\n') {
//         // Handle newline character
//         // dfa.tokens.push_back(Token("NEWLINE" , ""));
//             currentState = "start"; // Transition to the new state
//             tokenRead = "";
//             remainingInput = remainingInput.substr(1);
//         }
//        else if (nextChar == ' ') {
//         remainingInput = remainingInput.substr(1);
//         cout << "rem: " << remainingInput << endl;
//         if (currentState == "?COMMENT") {
//           // break;
//           // dfa.tokens.push_back(Token(currentState, tokenRead));
//           // currentState = "start";
//           // tokenRead = "";
//           nextChar = remainingInput[0];
//         }
//         // nextChar = remainingInput[0];
//          cout << "next: " << nextChar << endl;
//          cout << "read1: " << tokenRead << endl;
//        }
//        cout << "remaining" << remainingInput << endl;
//         string next = dfa.nextState(currentState, nextChar);
//         cout << "nextStatee" << next << endl;

//        if (next != UNDEFINED) {
//             tokenRead += nextChar;
//            cout << "read: " << tokenRead << endl;
//             if (tokenRead.find("$") == 0) {
//                try {
//                 int registerNumber = std::stoi(tokenRead.substr(1));  // Parse the numeric part after "$"
//                 if (registerNumber < 0 || registerNumber > 31) {
//                    throw std::runtime_error("Invalid register number: " + tokenRead);
//                 }
//             } catch (const std::invalid_argument& e) {
//                 // Not a valid integer following "$"
//             } catch (const std::out_of_range& e) {
//                 // Value out of range for integer
//             }
//             }
             

//             currentState = next;
//             remainingInput = remainingInput.substr(1);
//         } else {
          
//           // cout << "tokenread++ " << tokenRead << endl;
//           int64_t x;
//           long u;   
//           std::stringstream ss;
// //           if (currentState == "HEXINT") {
// //             // std::string hexValue = tokenRead.substr(2);
// //             // cout << "hv: " << hexValue << endl;
// //             try {
// //         u = std::stol(tokenRead,0, 16);
// //         // std::cout << "u: " << u << std::endl;
// //         // cout << "help" << stol("0xFFFFFFFF", 0, 16) << endl;

// //         if (u > std::stol("0xFFFFFFFF", 0, 16)) {
// //             throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
// //         }
// //     } catch (const std::out_of_range& e) {
// //         throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
// //     }
//         // }
// if (currentState == "NUM") {
//     ss.str(tokenRead);
//     ss >> x;
//     // cout << "tok: :" << tokenRead << endl;
//     if (x < -2147483648 || x > 2147483647) {
//        throw std::runtime_error("ERROR: Out of range NUM: " + tokenRead);
//         // std::cerr << "ERROR: Out of range DEC: " << tokenRead << std::endl;
//         // exit(1);
//     }
// }
//           //  cout << "tokenread+ " << tokenRead << endl;
//           //  cout << currentState << endl;
//             if (isAcceptingState(currentState)) {
//               // cout << "token: " << tokenRead << endl;
//               if (currentState == "ZERO") {
//                  dfa.tokens.push_back(Token("DECINT", tokenRead));
//               } else {
//               dfa.tokens.push_back(Token(currentState, tokenRead));
//               }
//               // cout << "check1 " << currentState << tokenRead << endl;
//               tokenRead = "";
//               currentState = "start"; // Reset to initial state
//             } else {
//               cout << tokenRead << endl;
//                //cout << currentState << endl;
//               throw std::runtime_error("Scanning failure1");
//             }
//         }
//     }
//     if (isAcceptingState(currentState)) {
//       //  cout << "tok1: :" << tokenRead << endl;
//       // cout << "tokenread+ " << tokenRead << endl;
//           int64_t x;
//           long int u;   
//           std::stringstream ss;
//           if (currentState == "HEXINT") {
//             // std::string hexValue = tokenRead.substr(2);
//             // cout << "hv: " << hexValue << endl;
//             try {
//         u = std::stol(tokenRead,0, 16);
//         // std::cout << "u: " << u << std::endl;
//         // cout << "help" << stol("0xFFFFFFFF", 0, 16) << endl;

//         if (u > std::stol("0xFFFFFFFF", 0, 16)) {
//             throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
//         }
//     } catch (const std::out_of_range& e) {
//         throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
//     }
// } else if (currentState == "NUM") {
//     ss.str(tokenRead);
//     ss >> x;
//     // cout << "tok: :" << tokenRead << endl;
//     if (x < -2147483648 || x > 2147483647) {
//        throw std::runtime_error("ERROR: Out of range NUM: " + tokenRead);
//         // std::cerr << "ERROR: Out of range DEC: " << tokenRead << std::endl;
//         // exit(1);
//     }
// }
//        dfa.tokens.push_back(Token(currentState, tokenRead));
//         // dfa.tokens.push_back(Token("NEWLINE" , ""));
// }
//      else {

//       //  cout << "tok: :" << tokenRead << endl;
//       //  cout << "cur: " << currentState << endl;
//   //       for (const Token& token : dfa.tokens) {
//   //   // Check if tokenKind starts with a "?"
//   //   if (token.kind.find("?") != 0) {
//   //     if (token.kind == "NEWLINE") {
//   //       cout << token.kind << endl;
//   //     } else {
//   //     std::cout << token.kind << " " << token.lexeme << std::endl;
//   //   }
//   //   }
//   // }
//       //  cout << "next: " << next << endl;
//       //  throw std::runtime_error("Scanning failure2");
//     }
//   return dfa.tokens;
// }



std::vector<Token> DFA::simplifiedMaximalMunch(const std::string& input, DFA& dfa) {
    std::string remainingInput = input;
    // std::cout << "input: " << input << std::endl;

    while (!remainingInput.empty()) {
        char nextChar = remainingInput[0];

        if (nextChar == '\n') {
            dfa.tokens.push_back(Token("NEWLINE", "\n"));
            remainingInput = remainingInput.substr(1);
        } else if (nextChar == ' ') {
            // Consider space as a token (if needed)
            dfa.tokens.push_back(Token("SPACE", " "));
            remainingInput = remainingInput.substr(1);
        } else {
            std::string tokenRead = "";
            std::string currentState = "start"; // Initial state

            while (!remainingInput.empty()) {
                nextChar = remainingInput[0];
                std::string next = dfa.nextState(currentState, nextChar);

                if (next != UNDEFINED) {
                    tokenRead += nextChar;
                    currentState = next;
                    remainingInput = remainingInput.substr(1);
                } else {
                    if (isAcceptingState(currentState)) {

                      //check Num Range
                      int64_t x;
                      std::stringstream ss;
                      if (currentState == "NUM") {
                        ss.str(tokenRead);
                        ss >> x;
                       if ( x > 2147483647 ) {
                        throw std::runtime_error("ERROR: Out of range NUM: " + tokenRead);
                       }
                      }

                    //   //check leading 0s
                    //   if (currentState == "NUM") {
                    //     if (tokenRead.length() == 1) {
                    //         // If the lexeme is just "0," it's a valid NUM.
                    //         dfa.tokens.push_back(Token("NUM", tokenRead));
                    //     } else if (tokenRead[0] != '0') {
                    //         // If the first character is not '0', it's a valid NUM.
                    //         dfa.tokens.push_back(Token("NUM", tokenRead));
                    //     } else {
                    //         // Leading zeroes are not allowed.
                    //         throw std::runtime_error("ERROR: Useless leading zeroes in NUM: " + tokenRead);
                    //     }
                    // }

                           //check leading 0s
              if (currentState == "NUM") {
                if (tokenRead.length() != 1 && tokenRead[0] == '0') {
                  throw std::runtime_error("ERROR: Useless leading zeroes in NUM: " + tokenRead);
                }
            }


                      //ID states
                      if (currentState == "I") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "IN") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "N") {
                        dfa.tokens.push_back(Token("ID", tokenRead));
                      }
                      else if (currentState == "NU") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "NUS") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "F") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "E") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "EL") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "ELS") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "W") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "WA") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "WAI") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "WH") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "WHI") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "WHIL") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "P") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "PR") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "PRI") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "PRIN") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "PRINT") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "PRINTL") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "R") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "RE") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "RET") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "RETU") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "RETUR") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "NE") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "D") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "DE") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "DEL") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "DELE") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "DELET") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                      else if (currentState == "NUL") {
                        dfa.tokens.push_back(Token("ID",  tokenRead));
                      }
                       else if (currentState == "NES") {
              dfa.tokens.push_back(Token("NE",  tokenRead));
            }
                      else {
                        dfa.tokens.push_back(Token(currentState, tokenRead));
                      }
                      tokenRead = "";
                      currentState = "start"; // Reset to initial state
                    } else {
                        throw std::runtime_error("Scanning failure");
                    }
                }
            }
            if (isAcceptingState(currentState)) {

              //check Num Range
              int64_t x;
              std::stringstream ss;
              if (currentState == "NUM") {
                ss.str(tokenRead);
                ss >> x;
                if ( x > 2147483647 ) {
                throw std::runtime_error("ERROR: Out of range NUM: " + tokenRead);
                }
              }

               //check leading 0s
              if (currentState == "NUM") {
                if (tokenRead.length() != 1 && tokenRead[0] == '0') {
                  throw std::runtime_error("ERROR: Useless leading zeroes in NUM: " + tokenRead);
                }
            }

              //ID 
               if (currentState == "I") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "IN") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "N") {
                  dfa.tokens.push_back(Token("ID", tokenRead));
                }
                else if (currentState == "NU") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "NUS") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "F") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "E") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "EL") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "ELS") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "W") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "WA") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "WAI") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "WH") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "WHI") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "WHIL") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "P") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "PR") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "PRI") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "PRIN") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "PRINT") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "PRINTL") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "R") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "RE") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "RET") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "RETU") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "RETUR") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "NE") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "D") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "DE") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "DEL") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "DELE") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "DELET") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "NUL") {
                  dfa.tokens.push_back(Token("ID",  tokenRead));
                }
                else if (currentState == "NES") {
              dfa.tokens.push_back(Token("NE",  tokenRead));
            }
                else {
                  dfa.tokens.push_back(Token(currentState, tokenRead));
                }
            } else {
                throw std::runtime_error("Scanning failure");
            }
        }
    }

    return dfa.tokens;
}




ScanningFailure::ScanningFailure(std::string message):
  message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }

