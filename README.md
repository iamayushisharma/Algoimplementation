# Algoimplementation
We use three algorithms with different time complexities
1)A depth-first search strategy to efficiently enumerate maximal cliques in an undirected graph. The main finding is that O(3ⁿ/³) is the worst-case time complexity, which is ideal for the maximum number of cliques a graph can have.
2)This study employs a graph's degeneracy, a metric of sparsity, in order to improve clique enumeration for sparse graphs. It alters the Bron–Kerbosch technique and achieves a time complexity of O(dn³d/³), where d is the graph's degeneracy.
3)This is a different approach by using arboricity, a parameter with respect to the number of edge-disjoint spanning forests in a graph. An algorithm that lists all k-cliques in O(a(G)^(k-2) * m) time, where a(G) is the arboricity of the graph.
