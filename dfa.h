#ifndef DFA_H
#define DFA_H


#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <cstdint>
#include <ostream>


// g++ -g -Wall -std=c++17 dfa-print.cc  -o dfas
//./dfas  dfa1.h   




struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Combine the hash values for the pair
        return h1 ^ h2;
    }
};

class Token;

class DFA {
public:
    DFA();

    //void DFAprint(const std::string& in);
    // void init(const std::string& in);

    void addState(const std::string& state);
    void setAccepting(const std::string& state, bool isAccepting);
    void addTransition(const std::string& from, const std::vector<char>& validChars, const std::string& to);

    std::vector<Token> simplifiedMaximalMunch(const std::string& input,  DFA& dfa);
    bool isAcceptingState(const std::string& state);
    std::string nextState(const std::string& currentState, char input);

    void printStates();
    void printTransitions();
    std::vector<Token> tokens;

private:
    struct Transition {
            std::string fromState;
            std::vector<char> validChars;
            std::string toState;
        };
    std::unordered_set<std::string> states;
    std::unordered_map<std::string, bool> accepting_states;
    std::unordered_map<std::string, std::vector<char>> transition_chars;
    std::vector<Transition> transitions;// Utility function to find a transition
    Transition findTransition(const std::string& from, char input);
};



/* An exception class thrown when an error is encountered while scanning.
 */
class ScanningFailure {
    std::string message;

  public:
    ScanningFailure(std::string message);

    // Returns the message associated with the exception.
    const std::string &what() const;
};



#endif // DFA_H
