# generation-graph-and-max-flow

The program has two modes:
* with the -t flag and a file with a graph structure in the format below: finds the maximum flow between two given vertices
* with the -g flag and a given number of vertices and edges: generates a random connected graph with random edge capacities, writes to a file in the format below, randomly selects two vertices and finds the maximum flow between them.

Asymptotic behavior of generating a random connected graph: O(m * log n)

Asymptotics of finding the maximum flow: O((m + n) * log m)

The essence of the algorithm for generating a random connected graph:
1) Building a tree
2) Adding remaining edges

A neighbor search tree is maintained for each vertex. Adding a neighbor occurs randomly (at each vertex, the side on which to add a neighbor is proportionally chosen), in O(log n) time (the height of the tree).

The maximum flow is found by sorting all edges by their capacity and using binary search. At each step of the search, the capacity of all edges is reduced by the capacity of the current edge, and the existence of the path is checked using dfs.

## Files are received as

\<number of vertices\> \<number of edges\>

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>

...

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>


## Graphs are generated and written as

\<number of the first vertex\> \<number of the second vertex\>

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>

...

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>

## How to start

* opt.exe -t \<number of the first vertex\> \<number of the second vertex\> \<file name\>

* opt.exe -g \<number of vertices\> \<number of edges\> \<file name\>
  
## Test

opt.exe -t \<number of the first vertex 1 - 5\> \<number of the second vertex 1 - 5\> bars.txt

or any generation to an existing file txt
