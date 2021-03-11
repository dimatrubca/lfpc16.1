#include <string>
#include <sstream>
#include <ctype.h>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

ifstream fin("input.txt");

struct transitionNode {
    string symbol;
    string resultState;
};


// key -> <state, transition>, value -> resultingStates
map<pair<string, string>, vector<string> > NFA;
map<pair<string, string>, string> DFA;

// map NFA state names to DFA states
map<string, set<string>> substates;

string q0NFA, q0DFA;
set<string> finalStatesDFA;
set<string> finalStatesNFA;

set<string> statesNFA;
set<string> statesDFA;

set<string> inputSymbols;


vector<string> split(const string& s, char delimeter) {
    vector<string> splits;
    istringstream ss(s);
    string str;
    while (getline(ss, str, delimeter)) {
        splits.push_back(str);
    }
    return splits;
}

string getSubstrBetween(string str, char firstChar, char lastChar) {
    auto first = str.find(firstChar);
    auto last = str.find(lastChar);

    return str.substr(first + 1, last - first - 1);
}

void parseFinalStates() {
    string s;
    getline(fin, s);

    s = getSubstrBetween(s, '{', '}');
    vector<string> states = split(s, ',');

    for (string state : states) {
        finalStatesNFA.insert(state);
    }
}


void parseTransitions() {
    string s;

    while (getline(fin, s)) {
        string str = getSubstrBetween(s, '(', ')');
        vector<string> splits = split(str, ',');

        string state = splits[0];
        string symbol = splits[1];
        string resultState = s.substr(s.rfind("=") + 1);

        NFA[{state, symbol}].push_back(resultState);
        inputSymbols.insert(symbol);
    }
}

bool areDisjoint(const set<string> &set1, set<string> &set2)
{
    for (string item : set1) {
        if (set2.find(item) != set2.end()) return false;
    }

    return true;
}

void convertToDFA() {
    string s;

    queue<string> statesQueue;
    q0DFA = "{" + q0NFA + "}";
    statesQueue.push(q0DFA);
    substates[q0DFA].insert(q0NFA);

    while (statesQueue.size()) {
        string currState = statesQueue.front();
        statesQueue.pop();

        set<string> currStates = substates[currState];
        for (string symbol : inputSymbols) {
            set<string> resultingStates;

            for (string state : currStates) {
                for (string resultingState : NFA[{state, symbol}]) {
                    resultingStates.insert(resultingState);
                }
            }

            string newStateName = "";
            for (auto state: resultingStates) {
                if (newStateName != "") newStateName+=", ";
                newStateName += state;
            }

            if (newStateName == "") newStateName = "{qDie}";
            else newStateName = "{" + newStateName + "}";

            DFA[{currState, symbol}] = newStateName;

            if (substates.find(newStateName) == substates.end()) {
                substates[newStateName].insert(resultingStates.begin(), resultingStates.end());
                statesQueue.push(newStateName);
            }

        }
    }

    // get final states
    for (auto keyVal : substates) {
        string stateName = keyVal.first;
        set<string> mappedStates = keyVal.second;

        if (!areDisjoint(mappedStates, finalStatesNFA)) finalStatesDFA.insert(stateName);
    }
}



void printDFATable() {
    int maxWidth = 4;

    for (auto keyVal : DFA) {
        string state = keyVal.first.first;

        maxWidth = max(maxWidth, (int) state.size() + 4);
    }

    cout<<setw(maxWidth)<<"DFA";
    for (string symbol : inputSymbols) cout<<setw(maxWidth)<<symbol;
    cout<<'\n';

    for (auto keyVal : substates) {
        string stateName = keyVal.first;
        string modStateName = stateName;
        set<string> substates = keyVal.second;

        if (finalStatesDFA.find(stateName) != finalStatesDFA.end()) modStateName = "*" + modStateName;
        if (stateName == q0DFA) modStateName = "->" + modStateName;

        cout<<setw(maxWidth)<<modStateName;
        for (string symbol : inputSymbols) {
            cout<<setw(maxWidth)<<DFA[{stateName, symbol}];
        }
        cout<<'\n';
    }
}

void read() {
    getline(fin, q0NFA);
    parseFinalStates();
    parseTransitions();
}

int main() {
    read();
    convertToDFA();
    printDFATable();

    return 0;
}
