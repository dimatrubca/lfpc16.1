#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#include <iomanip>      // std::setw
#define N 10000
#define x first
#define y second

using namespace std;

const char START = 'S';

map<char, set<string>> G;
set<char> Vt, Vn;
map<char, set<char>> F, L;
map<pair<char, char>, char> M;
vector<pair<char, string>> appliedRules;
int nn, nt, nr; // number of nonterminals/terminals/rules
string inputString;


bool isTerminal(char c) {
    return Vt.find(c) != Vt.end();
}

void read() {
    ifstream cin("input.txt");
    cin>>nn>>nt>>nr;

    for (int i=0; i<nn; i++) {
        char s; cin>>s;
        Vn.insert(s);
    }

    for (int i=0; i<nt; i++) {
        char c; cin>>c;
        Vt.insert(c);
    }

    for (int i=0; i<nr; i++) {
        char a;
        string b;
        cin>>a>>b;

        G[a].insert(b);
    }
    cin>>inputString;
}

void printRules() {
    cout<<"\nVn = {";

    bool isFirst = true;
    for (auto s : Vn) {
        if (!isFirst) cout<<", ";
        else isFirst = false;

        cout<<s;
    }
    cout<<"}\n";

    cout<<"Vt = {";

    isFirst = true;
    for (auto s : Vt) {
        if (!isFirst) cout<<", ";
        else isFirst = false;

        cout<<s;
    }
    cout<<"}\n";

    cout<<"P = {\n";
    for (auto nonterm : Vn) {
        isFirst = true;
        for (auto deriv : G[nonterm]) {
            if (!isFirst) cout<<" | ";
            else {
                isFirst = false;
                cout<<"\t"<<nonterm<<" -> ";
            }

            cout<<deriv;
        }
        cout<<"\n";
    }
    cout<<"}\n\n";
}

void computeFirstSet() {
    for (auto A : Vn) {
        for (string prod : G[A]) {
            F[A].insert(prod[0]);
        }
    }

    bool flag = 1;
    while (flag) {
        flag = 0;

        for (auto A : Vn) {
            for (auto B : F[A]) {
                if (!isTerminal(B)) {
                    int cnt = F[A].size();
                    F[A].insert(F[B].begin(), F[B].end());

                    if (F[A].size() != cnt) flag = 1;
                }
            }
        }
    }
}

void computeLastSet() {
    for (auto A : Vn) {
        for (string prod : G[A]) {
            L[A].insert(prod[prod.size() - 1]);
        }
    }

    bool flag = 1;
    while (flag) {
        flag = 0;

        for (auto A : Vn) {
            for (auto B : L[A]) {
                if (!isTerminal(B)) {
                    int cnt = L[A].size();
                    L[A].insert(L[B].begin(), L[B].end());

                    if (L[A].size() != cnt) flag = 1;
                }
            }
        }
    }
}

void printFirstLastSet() {
    for (auto nonterm : Vn) {
        cout<<"F["<<nonterm<<"] = {";

        bool isFirst = true;
        for (auto elem : F[nonterm]) {
            if (!isFirst) cout<<", ";
            isFirst = false;

            cout<<elem;
        }
        cout<<"}\n";
    }

    for (auto nonterm : Vn) {
        cout<<"L["<<nonterm<<"] = {";

        bool isFirst = true;
        for (auto elem : L[nonterm]) {
            if (!isFirst) cout<<", ";
            isFirst = false;

            cout<<elem;
        }
        cout<<"}\n";
    }
    cout<<"\n\n";
}

void addPrecendRelation(char a, char b, char prec) {
    if (M.find({a, b}) != M.end() && M[{a, b}] != prec) {
        cout<<"There is more than one precendence relation between symbols "<<a<<" and "<<b<<"!\n";
        exit(0);
    }

    M[{a, b}] = prec;
}

void buildSPMatrix() {
    cout<<"Matrix of Simple Precendence:\n";
    for (auto nonterm : Vn) {
        for (string prod : G[nonterm]) {
            for (int i=0; i+1 < prod.size(); i++) {
                char x1 = prod[i], x2 = prod[i + 1];
                addPrecendRelation(x1, x2, '=');

                if (!isTerminal(x2)) {
                    for (auto c : F[x2]) {
                        addPrecendRelation(x1, c, '<');
                    }
                }

                if (!isTerminal(x1) && isTerminal(x2)) {
                    for (auto c : L[x1]) {
                        addPrecendRelation(c, x1, '>');
                    }
                }

                if (!isTerminal(x1) && !isTerminal(x2)) {
                    for (auto c1 : L[x1]) {
                        for (auto c2 : F[x2]) {
                            addPrecendRelation(c1, c2, '>');
                        }
                    }
                }
            }
        }
    }

    for (auto s : F[START]) {
        addPrecendRelation('$', s, '<');
    }

    for (auto s : L[START]) {
        addPrecendRelation(s, '$', '>');
    }
}

void showSPMatrix() {
    set<char> allSymbols;
    allSymbols.insert(Vn.begin(), Vn.end());
    allSymbols.insert(Vt.begin(), Vt.end());
    allSymbols.insert('$');

    cout<<setw(4)<<" ";
    for (auto s : allSymbols) cout<<setw(4)<<s; cout<<'\n';
    for (auto s1 : allSymbols) {
        cout<<setw(4)<<s1;
        for (auto s2 : allSymbols) {
            cout<<setw(4)<<M[{s1, s2}];
        }
        cout<<'\n';
    }
    cout<<"\n\n";
}

//result is undefined if passed container.rend()
template <class ReverseIterator>
typename ReverseIterator::iterator_type make_forward(ReverseIterator rit)
{
    return --(rit.base()); // move result of .base() back by one.
}

char findMatch(string handler) {
    for (auto nonterm : Vn) {
        for (string prod : G[nonterm]) {
            if (handler == prod) return nonterm;
        }
    }

    cout<<"error\n";
    exit(0);
}

string removePrecendCharacters(string str) {
    str.erase(std::remove(str.begin(), str.end(), '<'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '='), str.end());
    str.erase(std::remove(str.begin(), str.end(), '>'), str.end());

    return str;
}

void logInfo(vector<char> Stack, string input, int idx) {
    char precendence = M[{Stack.back(), input[idx]}];
    string action = "";
    string stackStr(Stack.begin(), Stack.end());

    if (Stack.size() == 3 && Stack[2] == START && idx == input.size() - 1) {
        action = "ACCEPT";
    } else if (precendence == '>') action = "REDUCE";
    else action = "SHIFT";

    cout<<left<<setw(15)<<stackStr<<setw(15)<<precendence<<setw(15)<<input.substr(idx)<<setw(10)<<action<<'\n';
}


void simplePrecendenceParsing(string input) {
    vector<char> S; // stack
    input += "$";
    S.push_back('$');
    int idx = 0;

    cout<<left<<setw(15)<<"STACK"<<setw(15)<<"PRECENDENCE"<<setw(15)<<"INPUT"<<setw(10)<<"ACTION";
    cout<<'\n';
    while (!(S.size() == 3 && S[2]==START && idx == input.size() - 1)) {
        if (idx > input.size() - 1) {
            cout<<"Invalid word!";
            exit(0);
        }
        char precendence = M[{S.back(), input[idx]}];


        logInfo(S, input, idx);

        if (precendence == '<' || precendence == '=') {
            // shift
            S.push_back(precendence);
            S.push_back(input[idx]);
            idx++;
        } else {
            // reduce
            auto reverse_it = find(S.rbegin(), S.rend(), '<');
            auto it = make_forward(reverse_it);
            string handler(it + 1, S.end());
            handler=removePrecendCharacters(handler);
            char subst = findMatch(handler);

            appliedRules.push_back({subst, handler});

            S.erase(it, S.end());
            char prec = M[{S.back(), subst}];
            S.push_back(prec);
            S.push_back(subst);

            string test(S.begin(), S.end());
        }
    }
    logInfo(S, input, idx);


}

void printAppliedRules() {
    cout<<"\nAplied Rules ["<<inputString<<"]:\n";

    for (auto it = appliedRules.rbegin(); it != appliedRules.rend(); it++ ) {
        cout<<it->x<<" -> "<<it->y<<'\n';
    }
}
int main() {
    read();

    cout<<"Initial grammar:\n";
    printRules();

    computeFirstSet();
    computeLastSet();
    printFirstLastSet();
    buildSPMatrix();
    showSPMatrix();

    simplePrecendenceParsing(inputString);
    printAppliedRules();


    return 0;
}
