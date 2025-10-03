from typing import List, Tuple

class UnionFind:
    def __init__(self, size: int) -> None:
        self.parent = list(range(size))
        self.rank = [0] * size

    def find(self, x: int) -> int:
        # Iterative path compression (avoids deep recursion)
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a: int, b: int) -> bool:
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        if self.rank[ra] < self.rank[rb]:
            self.parent[ra] = rb
        elif self.rank[ra] > self.rank[rb]:
            self.parent[rb] = ra
        else:
            self.parent[rb] = ra
            self.rank[ra] += 1
        return True


Edge = Tuple[int, int, int]  # (weight, u, v)

def kruskal(graph_edges: List[Edge], number_of_vertices: int) -> List[Edge]:
    """Return the edges of a Minimum Spanning Tree using Kruskal's algorithm.
    Assumes vertices are 0..number_of_vertices-1.
    """
    uf = UnionFind(number_of_vertices)
    mst: List[Edge] = []

    # Sort strictly by weight (clearer than relying on tuple order)
    for w, u, v in sorted(graph_edges, key=lambda e: e[0]):
        if 0 <= u < number_of_vertices and 0 <= v < number_of_vertices:
            if uf.union(u, v):
                mst.append((w, u, v))
                # Early exit when MST has V-1 edges
                if len(mst) == number_of_vertices - 1:
                    break
        else:
            # Skip invalid edges; could also raise ValueError
            continue
    return mst


if __name__ == "__main__":
    # Example
    edges: List[Edge] = [
        (4, 0, 1), (8, 1, 2), (7, 2, 3), (9, 3, 4), (10, 4, 5),
        (14, 3, 5), (2, 5, 6), (4, 5, 2), (6, 6, 8), (1, 6, 7),
        (7, 7, 8), (2, 8, 2), (8, 7, 0), (11, 7, 1)
    ]  # (weight, node1, node2)

    n = 9  # vertices 0..8
    mst = kruskal(edges, n)
    print("MST edges:", mst)

    total_weight = sum(w for w, _, _ in mst)
    print("MST total weight:", total_weight)