# Laboratory Task No. 1 Regular Grammar to Finite Automato
## Variant: 10

## Tasks: 
1. Write a program which converts regular grammar to Finite Automaton (FA).
2. Using Finite Automaton (FA) check if some input string is accepted by FA  (meaning you could generate that string by traversing FA)
3. Using some graphic library plot FA graph (Bonus Point)

## Implementation:
The grammar (production rules) is parsed from the file input.txt<br />
To present the graph of the grammar, I have used a hashmap, where to each key (the nonterminal simbol on the left side of the production) corresponds a list of pairs consisting of the terminal and nonterminal (or None if doesn't exist) symbols.

 
