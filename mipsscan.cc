
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
LABELDEF!
dot
DOTID!
ZERO!
minus
DECINT!
zerox
HEXINT!
dollar
REGISTER!
COMMA!
LPAREN!
RPAREN!
cr
NEWLINE!
?WHITESPACE!
?COMMENT!
.TRANSITIONS
start a-z A-Z     ID
ID    a-z A-Z 0-9 ID
ID    :           LABELDEF
start . dot
dot   a-z A-Z     DOTID
DOTID a-z A-Z 0-9 DOTID
start  0   ZERO
start  1-9 DECINT
start  -   minus
minus  0-9 DECINT
DECINT 0-9 DECINT
ZERO   0-9 DECINT
ZERO   x   zerox
zerox  0-9 a-f A-F HEXINT
HEXINT 0-9 a-f A-F HEXINT
start    $   dollar 
dollar   0-9 REGISTER
REGISTER 0-9 REGISTER
start , COMMA 
start ( LPAREN
start ) RPAREN
start \n NEWLINE
start \r cr
cr    \n NEWLINE
start       \s \t ?WHITESPACE
?WHITESPACE \s \t ?WHITESPACE
start    ; ?COMMENT
?COMMENT \x00-\x09 \x0B \x0C \x0E-\x7F ?COMMENT)";


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



// int main() {
//   try {
//     std::stringstream s(DFAstring);
//     //createDFAFromStream(s);
//     DFAprint(s);

//   } catch(std::runtime_error &e) {
//     std::cerr << "ERROR: " << e.what() << "\n";
//     return 1;
//   }
//   return 0;
// }

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
    // Check if tokenKind starts with a "?"
    if (token.kind.find("?") != 0) {
      if (token.kind == "NEWLINE") {
        cout << token.kind << endl;
      } else {
      std::cout << token.kind << " " << token.lexeme << std::endl;
    }
    }
  }
  return 0;
}


void DFAprint(const std::string& in, DFA& dfa) {
  std::istringstream iss(in); // Create a stringstream from the input string
  std::string s;
  //DFA dfa;
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
  //dfa.printTransitions();
  //  cout << dfa.nextState("start", '(') << endl;
  //  cout << dfa.nextState("start", ')') << endl;
  //cout << dfa.nextState("start", 'A') << endl;
   //cout << dfa.nextState("start", 'l') << endl;
  //cout << "yes" << endl;
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

// // Initialize the DFA from a string
// void DFA::init(const std::string& in) {
//     states.clear();
//     accepting_states.clear();
//     transitions.clear();

//     // Call DFAprint here
//     DFAprint(in);
// }


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

std::vector<Token> DFA::simplifiedMaximalMunch(const string& input, DFA& dfa) {
  // DFA dfa;
  // dfa.printStates();
 // dfa.printTransitions();
    // vector<Token> tokens;
    string remainingInput = input;
    // cout << input << endl;
    //cout << input << endl;
    string tokenRead = "";
    string currentState = "start"; // Initial state

    while (!remainingInput.empty()) {
        // cout << "rem:" << remainingInput << endl;
        char nextChar = remainingInput[0];
        // cout << "next: " << nextChar << endl;
        if (nextChar == '\n') {
        cout << "yup: " <<  endl;
        // Handle newline character
        dfa.tokens.push_back(Token("NEWLINE" , ""));
            currentState = "start"; // Transition to the new state
            tokenRead = "";
            remainingInput = remainingInput.substr(1);
        }
       else if (nextChar == ' ') {
        remainingInput = remainingInput.substr(1);
        // cout << "rem: " << remainingInput << endl;
        if (tokenRead == ".word") {
          dfa.tokens.push_back(Token(currentState, tokenRead));
          currentState = "start";
          // cout << "tok: " << tokenRead << endl;
          tokenRead = "";
          nextChar = remainingInput[0];
          // cout << "help: " <<  tokenRead << endl;
        }
       }
        string next = dfa.nextState(currentState, nextChar);
        // cout << "next" << next << endl;

       if (next != UNDEFINED) {
            tokenRead += nextChar;
          //  cout << "read: " << tokenRead << endl;
            if (tokenRead.find("$") == 0) {
               try {
                int registerNumber = std::stoi(tokenRead.substr(1));  // Parse the numeric part after "$"
                if (registerNumber < 0 || registerNumber > 31) {
                   throw std::runtime_error("Invalid register number: " + tokenRead);
                }
            } catch (const std::invalid_argument& e) {
                // Not a valid integer following "$"
            } catch (const std::out_of_range& e) {
                // Value out of range for integer
            }
            }
             

            currentState = next;
            remainingInput = remainingInput.substr(1);
        } else {
          
          // cout << "tokenread++ " << tokenRead << endl;
          int64_t x;
          long u;   
          std::stringstream ss;
          if (currentState == "HEXINT") {
            // std::string hexValue = tokenRead.substr(2);
            // cout << "hv: " << hexValue << endl;
            try {
        u = std::stol(tokenRead,0, 16);
        // std::cout << "u: " << u << std::endl;
        // cout << "help" << stol("0xFFFFFFFF", 0, 16) << endl;

        if (u > std::stol("0xFFFFFFFF", 0, 16)) {
            throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
        }
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
    }
} else if (currentState == "DECINT") {
    ss.str(tokenRead);
    ss >> x;
    // cout << "tok: :" << tokenRead << endl;
    if (x < -2147483648 || x > 4294967295) {
       throw std::runtime_error("ERROR: Out of range DEC: " + tokenRead);
        // std::cerr << "ERROR: Out of range DEC: " << tokenRead << std::endl;
        // exit(1);
    }
}
          //  cout << "tokenread+ " << tokenRead << endl;
          //  cout << currentState << endl;
            if (isAcceptingState(currentState)) {
              // cout << "token: " << tokenRead << endl;
              if (currentState == "ZERO") {
                 dfa.tokens.push_back(Token("DECINT", tokenRead));
              } else {
              dfa.tokens.push_back(Token(currentState, tokenRead));
              }
              // cout << "check1 " << currentState << tokenRead << endl;
              tokenRead = "";
              currentState = "start"; // Reset to initial state
            } else {
              // cout << tokenRead << endl;
               //cout << currentState << endl;
              throw std::runtime_error("Scanning failure");
            }
        }
    }
    if (isAcceptingState(currentState)) {
      //  cout << "tok1: :" << tokenRead << endl;
      // cout << "tokenread+ " << tokenRead << endl;
          int64_t x;
          long int u;   
          std::stringstream ss;
          if (currentState == "HEXINT") {
            // std::string hexValue = tokenRead.substr(2);
            // cout << "hv: " << hexValue << endl;
            try {
        u = std::stol(tokenRead,0, 16);
        // std::cout << "u: " << u << std::endl;
        // cout << "help" << stol("0xFFFFFFFF", 0, 16) << endl;

        if (u > std::stol("0xFFFFFFFF", 0, 16)) {
            throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
        }
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("ERROR: Out of range HEX: " + tokenRead);
    }
} else if (currentState == "DECINT") {
    ss.str(tokenRead);
    ss >> x;
    // cout << "tok: :" << tokenRead << endl;
    if (x < -2147483648 || x > 4294967295) {
       throw std::runtime_error("ERROR: Out of range DEC: " + tokenRead);
        // std::cerr << "ERROR: Out of range DEC: " << tokenRead << std::endl;
        // exit(1);
    }
}
       dfa.tokens.push_back(Token(currentState, tokenRead));
        dfa.tokens.push_back(Token("NEWLINE" , ""));
}
     else {
       throw std::runtime_error("Scanning failure");
    }
  return dfa.tokens;
}



ScanningFailure::ScanningFailure(std::string message):
  message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }
