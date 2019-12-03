# Graph-Subset-Mapping

## Scenario

You are an investigative agency working on uncovering a drug mafia. You have got telephone records of various telephone numbers which are believed to be associated with this mafia. You have also got a set of emails related to the mafia. However, you do not know which telephone number corresponds to which email address. The goal is to automatically figure out the mapping between emails and phones if it exists. To solve this problem (for our assignment), you make a few assumptions.

1. Some people are net savvy and use emails. All people know how to use phones. People who use emails regularly use both emails and phones to communicate with each other.
2. If a person X emailed a person Y, he also called Y on phone at some point. If X did not email a person Y (and both of them have email addresses), he did not call Y either.
3. Each person has exactly one email address and exactly one phone number.

You abstract out the problem by creating two graphs – Gphone and Gemail. There exists a directed edge between two nodes in Gphone (or in Gemail) if the first phone number (or email address) called (or emailed) the second. Your goal is find a mapping from emails to phone numbers. Gemail is the smaller graph because fewer people are net-savvy

## Problem Statement

There are two directed graphs G and G’. The graphs do not have self-edges. Find aone-one mapping M from nodes in G to nodes in G’ such that there is an edge from v1 to v2 in G if ando nly if there is an edge from M(v1) to M(v2) in G

Here, this problem is solved using miniSAT, a complete SAT solver for this problem. The code will read two graphs in the given input format. It will then convert the mapping problem into a CNF SAT formula. The SAT formula will be the input to miniSAT, which will return with a variable assignment that satisfies the formula (or an answer "no", signifying that the problem is unsatisfiable). The code will then take the SAT assignment and convert it into a mapping from nodes of G to nodes of G'. And finally output this mapping in the given output format.

A problem generator is provided that takes inputs of the sizes of G and G’ and generates random problems with those parameters

## Input format

Nodes are represented by positive integers starting from 1. Each line represents an edge from the first node to the second. Both graphs are presented in the single file, the larger first. The line with "0 0" is the boundary between the two. An Example is:

    1 2
    1 3
    1 4
    2 4
    3 4
    0 0
    1 2
    3 2
    
## Output format

The mapping will map each node of G into a node id for G’. The first numbers on each line represent a
node as numbered in the smaller graph G, and the second number represents the node of the larger
graph G’ to which it is mapped. The output of the same problem is

    1 2
    2 4
    3 3
    
If the problem is unsatisfiable output a 0

## Implementation

**graph_to_cnf.cpp** takes the graph as the input and obtains the CNF clauses to be passed to the MiniSAT program. It first of all, stores the given graphs in the form of adjacency list. Then, a SATmatrix was defined for mapping the jth mail-user to the ith phone-user. Then, the unary clauses, row clauses, edge consistency clauses, one-per-row and one-per-column clauses etc are obtained and stored in a SAT input file, which is to be used by the MiniSAT. The number of clauses was kept more, while the size of each clause is less for reducing the computational speed. 

Then this file is passed to the SAT solver and it outputs a solved file. And then it is passed to the **sol_to_mapping.cpp** to obtain the final mapping. 
