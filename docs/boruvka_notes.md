# Notes on Borůvka's algorithm

These notes summarize the algorithm I am implementing for the
MST-based single-link variant of the project.

## Sources

1. Wikipedia: <https://en.wikipedia.org/wiki/Bor%C5%AFvka%27s_algorithm>
2. Rajasekaran, S. (2005). Efficient Parallel Hierarchical Clustering
   Algorithms. IEEE TPDS 16(6), 497–502.

## Description

Borůvka's algorithm is a greedy algorithm for finding a Minimum
Spanning Tree (MST) of a connected weighted graph, or a Minimum
Spanning Forest if the graph is disconnected.

It is also known as **Sollin's algorithm**, this is the name
Rajasekaran uses throughout §3.2, and it is common in the parallel
computing literature (according to wiki)

I saw other algorithms that solve the same problem as Borůvka, namely Prim and Kruskal, but we use Borůvka because its phase structure is naturally parallel: each phase processes all current components independently.

## How it works

The algorithm builds the MST in phases. Each phase:

1. For every current component (initially, every single vertex),
   find the minimum-weight edge connecting it to a different
   component.
2. Add all of these edges to the forest at once.
3. Merge the components connected by the added edges.

Each phase at least halves the number of components, so the
algorithm terminates in O(log V) phases. When only one component
remains, the forest is the MST.

A tie-breaking rule (for example lexicographic on endpoints) is needed
when two edges have equal weight to avoid creating cycles.

## Pseudocode (adapted from Wikipedia)
algorithm Borůvka(G = (V, E)):
F := (V, {})   # forest with no edges
while F has more than one component:
for each component C of F:
cheapest[C] := the minimum-weight edge from C to a different component, or None
for each component C with cheapest[C] != None:
add cheapest[C] to F
return F

## Complexity

O(E log V) where E is the number of edges and V the number of
vertices. In our setting (Euclidean distance on n points) E = n(n-1)/2,
so we get O(n^2 log n) sequential time.

## Connection to our project

The whole reason we use Borůvka here is because
**single-link hierarchical clustering reduces to MST construction**
(Rajasekaran §2.3, Lemma 3.5).

Given n points in ℝᵈ, build the complete graph G where:
- vertices are the input points
- edge (i, j) has weight equal to the Euclidean distance d(p_i, p_j).

Then:
- Compute the Euclidean MST of G using Borůvka.
- Sort the MST's n-1 edges by weight (ascending).
- Walking through the sorted edges in order produces the single-link
  dendrogram: edge (i, j) of weight w corresponds to merging the
  cluster containing i with the cluster containing j at height w.

This works because the single-link distance between two clusters
is exactly the weight of the lightest edge between them (which is
precisely an MST edge.)

## Why Borůvka §3.2 of Rajasekaran and not §4

Rajasekaran presents two algorithm families:

- **§3 (worst-case guarantees)**: adapts Sollin/Borůvka to the
  AROB and PRAM models.
- **§4 (expected-case)**: uses graph sparsification, but **assumes
  points are uniformly distributed in space**.

We follow §3 as Rajasekaran himself notes that §4's bounds "may not
be of practical interest since the assumption of uniform distribution
of points in the space made in these algorithms may not hold." Real
clustering datasets are non-uniform by definition (that is why they
cluster), so §3's framework matches our setting better.

Also, we're not implementing §3.2 literally (it targets the AROB model) but rather the **same underlying algorithm (Borůvka/Sollin)** that §3.2 builds on, adapted to shared-memory CPU using `std::thread`.

## Parallelization plan (for the parallel version)

- The minimum-edge search in each phase is the natural parallel
  step: assign components to threads, each thread finds its
  component's cheapest outgoing edge.
- Union-find (merging components after each phase) is done
  sequentially between phases in the first version. Concurrent
  union-find is a possible later optimization.

## Optimizations to consider later

- Removing edges that connect two vertices already in the same
  component, so they are not re-scanned in later phases. (idea gotten from wikipedia in pseudocode paragraph)
- maybe add others later 