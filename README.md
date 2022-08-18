# generation-graph-and-max-allow-width

## Files are received as

\<number of vertices\> \<number of edges\>

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>

...

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>


## Graphs are generated and written as

\<ID of Capital\> \<ID of Zodanga\>

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>

...

\<vertex number\> : \<its neighbor's number\>-\<edge width\> ... \<its neighbor's number\>-\<edge width\>


## Test

opt.exe -t \<id_cap 1 - 5\> \<id_zod 1 - 5\> bars.txt
