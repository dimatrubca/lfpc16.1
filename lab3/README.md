# Laboratory Task No. 3 Chomsky Normal Form (CNF)

## Variant: 10
**Input format**:

First line contains 3 integers nn, nt, nr, where nn is the number of nonterminals, nt - number of terminals, nr - number of rules

The second line contains nn characters, specifying the nonterminal symbols from the grammar

The third line contains nt characters, specifying the terminals from the grammar

The next nr line contain one character (from the nonterminal set), followed by a string, specifying the production it forms.


## Manual deduction of the Chomsky Normal Form:
<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab3/images/manual1.jpg" width="500">
<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab3/images/manual2.jpg" width="500">
<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab3/images/manual3.jpg" width="500">


2. Write program which converts nondeterministic finite automato (NFA) to deterministic finite automato (DFA)

The program can be found inside lab2.cpp file. There is a separate function for parsing the input data, converting the NFA to DFA, as well as for drawing the table for the obtained DFA.

3. Display converted automato in form of graph or transition table

<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab2/images/dfa_table.png" width="450">



