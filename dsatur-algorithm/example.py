from dsatur import dsatur

# vertices
V = [1, 2, 3, 4, 5, 6, 7, 8]

# edges
E = { 1: [2, 3, 4, 7],
      2: [1, 5, 6],
      3: [1, 7],
      4: [1, 7],
      5: [2, 6, 8],
      6: [2, 5, 8],
      7: [1, 3, 4, 8],
      8: [5, 6, 7] }

print(dsatur(V, E))