def dsatur(V, E):
    U = V.copy() # vertex uncolored
    C = {v:0 for v in V} # each vertex color
    S = {v:set() for v in V} # colors used by adjacents of v
    deg = lambda v: len(E[v])
    sat = lambda v: len(S[v])
    cmap = set(range(1, len(U) + 1)) 
    while(U):
        # get the vertex with highest degree of saturation
        # (break ties by highest degree)
        h = U[0]
        for v in U:
            if sat(v) > sat(h) or (sat(v) == sat(h) and deg(v) > deg(h)):
                h = v 
        v = U.pop(U.index(h))
        # get the min color unused by adj to paint the vertex
        C[v] = min(cmap - S[v])
        # add vertex color to adjacents colors sets 
        for u in E[v]:
            S[u].add(C[v])
    return C