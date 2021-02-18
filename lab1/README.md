# Laboratory Task No. 1 Regular Grammar to Finite Automato
## Variant: 10

## Tasks: 
1. Write a program which converts regular grammar to Finite Automaton (FA).
2. Using Finite Automaton (FA) check if some input string is accepted by FA  (meaning you could generate that string by traversing FA)
3. Using some graphic library plot FA graph (Bonus Point)

## Implementation:
The grammar (production rules) is parsed from the file *input.txt*<br />
To present the graph (FA), I have used a hashmap, where to each key (the nonterminal simbol on the left side of the production) corresponds a list of pairs consisting of the terminal and nonterminal (or None if doesn't exist) symbols.<br />
Each string line from the file *check_strings.txt* is then checked whether it's accepted by the FA, by traversing the graph.<br />
To draw the graph, *networkx* library is used.

## Results:
