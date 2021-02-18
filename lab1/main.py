import sys
import networkx as nx
import matplotlib.pyplot as plt

G = {}

def read_input(filename):
    with open(filename) as f:
        for line in f.read().splitlines():
            A = line[0]
            a = line[3]
            B = line[4] if len(line) > 4 else None

            if A not in G:
                G[A] = []

            G[A].append((a, B))


def backtrack(s, pos, A):
    #print(s, pos, A)
    if pos == len(s):
        return A == None

    if A == None or A not in G:
        return False

    for a, B in G[A]:
        if s[pos] == a:
            #print(a)
            if backtrack(s, pos+1, B):
                return True

    return False



def is_accepted(s):
    return backtrack(s, 0, 'S')


def check_expressions(filename):
    with open(filename, 'r') as f:
        for expression in f.read().splitlines():
            message = "Accepted" if is_accepted(expression) else "Not Accepted"

            print(expression + ": ", message)
    



def draw_graph():
    DG = nx.DiGraph()

    edges = []
    for A, arr in G.items():
        for a, B in arr:
            DG.add_edge(A,B if B else "$",title=a)

    edge_labels = nx.get_edge_attributes(DG, 'title')
    pos = nx.spring_layout(DG)
    nx.draw(DG, pos, with_labels=True)
    nx.draw_networkx_edge_labels(DG, pos, edge_labels=edge_labels)

    plt.show()



def main():
    read_input("productions.txt")
    check_expressions("check_strings.txt")
    draw_graph()


if __name__ == "__main__":
    main()

    

            

