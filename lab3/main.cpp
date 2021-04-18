#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#define N 10000
#define x first
#define y second

using namespace std;

map<char, set<string>> G;
set<char> Vt, Vn;
int nn, nt, nr;


bool isTerminal(string s) {
    return s.size()==1 && Vt.find(s[0]) != Vt.end();
}

bool isTerminal(char c) {
    return Vt.find(c) != Vt.end();
}

char getNotUsedNonterminal() {
    for (char c='A'; ; c++) {
        if (Vn.find(c) == Vn.end()) return c;
    }
}

void read() {
    ifstream cin("variant10.in");
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
}

void eliminateEpsProductions() {
    map<char, bool> eliminatedAt; // to avoid infinite loops
    queue<char> Ne;

    for (char nonterm : Vn) {
        if (G[nonterm].find("e") != G[nonterm].end()) Ne.push(nonterm);
    }

    while (Ne.size()) {
        char A = Ne.front();
        Ne.pop();

        G[A].erase("e");
        eliminatedAt[A] = true;

        for (char X : Vn) {
            set<string> newProductions;
            for (string Y : G[X]) {
                if (Y.find(A) == string::npos) continue;

                int occur = 0;
                for (int i=0; i<Y.size(); i++) {
                    if (Y[i] == A) {
                        occur++;
                    }
                }
                // for each subset of |occur| elements
                for (int i=0; i<(1<<occur) - 1; i++) {
                    set<int> subset;
                    for (int j=0; (1<<j)<=i; j++) {
                        if (i & (1<<j)) subset.insert(j);
                    }

                    int countA = 0;
                    string result;
                    for (char c : Y) {
                        if (c!=A || subset.find(countA++) != subset.end()) result+=string(1, c);
                    }

                    if (result == "") {
                        if (eliminatedAt[X]) continue;

                        result = "e";
                        Ne.push(X);
                    }
                    newProductions.insert(result);
                }
            }

            G[X].insert(newProductions.begin(), newProductions.end());
        }
    }
}


void eliminateUnitProductions() {
    // eliminate unit productions
    queue<pair<char, char>> unitProd;

    for (char X: Vn) {
        for (string Y : G[X]) {
            if (Y.size() == 1 && Vn.find(Y[0]) != Vn.end()) {
                unitProd.push({X, Y[0]});
            }
        }
    }

    while (unitProd.size()) {
        pair<char, char> prod = unitProd.front();
        unitProd.pop();
        G[prod.x].erase(string(1, prod.y)) ;

        for (auto derivation : G[prod.y]) {
            G[prod.x].insert(derivation);
        }
    }
}

void eliminateInaccessibleSymbols(char start) {
    set<char> accessible, notAccessible;
    queue<char> Q;

    accessible.insert(start);
    Q.push(start);

    while (Q.size()) {
        char x = Q.front();
        Q.pop();

        for (string prod : G[x]) {
            for (auto c : prod) {
                if (accessible.find(c) == accessible.end()) {
                    accessible.insert(c);
                }
            }
        }
    }

    set_difference(Vn.begin(), Vn.end(), accessible.begin(), accessible.end(),
                   inserter(notAccessible, notAccessible.end()));

    for (char c : notAccessible) {
        G.erase(c);
        Vn.erase(c);
    }
}

void removeNonProductiveSymbols() {
    set<char> productive, nonProductive;
    productive.insert(Vn.begin(), Vn.end());

    for (char X : Vn) {
        for (string Y : G[X]) {
            for (char ch : Y) { // todo template method if set contains element
                if (productive.find(ch) != productive.end()) {
                    productive.insert(X);
                }
            }
        }
    }

    set_difference(Vn.begin(), Vn.end(), productive.begin(), productive.end(),
                   inserter(nonProductive, nonProductive.end()));

    for (char c : nonProductive) {
        G.erase(c);
        Vn.erase(c);
    }
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

void convertToChomsky() {
    map<string, char> Substitution;

    // replace terminals in productions if needed
    for (char nonterm : Vn) {
        set<string> modifiedProds;

        for (string prod : G[nonterm]) {
            if (isTerminal(prod)) {
                modifiedProds.insert(prod);
                continue;
            }

            string modifiedProd = "";
            for (char c : prod) {
                if (isTerminal(c)) {
                    char newC;

                    if (Substitution.find(string(1, c)) != Substitution.end()) {
                        newC = Substitution[string(1, c)];
                    } else {
                        newC = Substitution[string(1, c)] = getNotUsedNonterminal();
                        Vn.insert(newC);
                        G[newC].insert(string(1, c));
                    }
                    modifiedProd += string(1, newC);
                } else modifiedProd += string(1, c);
            }

            modifiedProds.insert(modifiedProd);
        }
        G[nonterm] = modifiedProds;
    }

    // replace sequences of 3+ nonterminals
    for (char nonterm : Vn) {
        set<string> modifiedProds;

        for (string prod : G[nonterm]) {
            if (prod.size()==1 || prod.size()==2) {
                modifiedProds.insert(prod);
                continue;
            }

            for (int i=0; prod.size() > 2; ) {
                string str = prod.substr(i, i+2);
                char replacement;

                if (Substitution.find(str) != Substitution.end()) {
                    replacement = Substitution[str];
                } else {
                    replacement = getNotUsedNonterminal();
                    Substitution[str] = replacement;
                    Vn.insert(replacement);
                    G[replacement].insert(str);
                }
                prod.replace(i, 2, string(1, replacement));
            }

            modifiedProds.insert(prod);
        }

        G[nonterm] = modifiedProds;
    }
}

int main() {
    read();

    cout<<"Initial grammar:\n";
    printRules();

    eliminateEpsProductions();
    cout<<"After eliminating Eps Productions:\n";
    printRules();

    eliminateUnitProductions();
    cout<<"After eliminating Unit Productions:\n";
    printRules();

    eliminateInaccessibleSymbols('S');
    cout<<"After eliminating Inaccessible Symbols:\n";
    printRules();

    removeNonProductiveSymbols();
    cout<<"After removing non productive symbols:\n";
    printRules();

    cout<<"Conversion to Chomsky Normal Form:\n";
    convertToChomsky();
    printRules();

    return 0;
}
