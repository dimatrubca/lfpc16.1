# Laboratory Task No. 1 Regular Grammar to Finite Automato
## Variant: 10

## Tasks: 
1. Write a program which converts regular grammar to Finite Automaton (FA).
2. Using Finite Automaton (FA) check if some input string is accepted by FA  (meaning you could generate that string by traversing FA)
3. Using some graphic library plot FA graph (Bonus Point)

## Implementation:
The grammar (production rules) is parsed from the file *productions.txt*<br />
To present the graph (FA), I have used a hashmap, where to each key (the nonterminal simbol on the left side of the production) corresponds a list of pairs consisting of the terminal and nonterminal (or None if doesn't exist) symbols.<br />
Each string line from the file *check_strings.txt* is then checked whether it's accepted by the FA, by traversing the graph.<br />
To draw the graph, *networkx* library is used.

## Results:
**Checked words**

<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab1/images/checked_words.png" width="350">


**FA graph** <br/><br/><br/>
<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab1/images/graph.png" width="550">

## Determine the grammar type by the Chromsky classification
<img src="https://github.com/dimatrubca/lfpc16.1/blob/master/lab1/images/task.png" width="150">
According to Noam Chomosky, there are 4 types of grammar. The given grammar can be identified as being of type 4 and represents **right linear grammar**, as the nonterminal symbols in the result of each production rule are on the right side.



