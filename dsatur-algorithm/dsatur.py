def dsatur(V, E):
    U = V.copy() # vertex uncolored
    C = {v:0 for v in V} # each vertex color
    S = {v:set() for v in V} # colors used by adjacents of v
    deg = lambda v: len(E[v])
    sat = lambda v: len(S[v])
    cmap = set(range(1, len(U) + 1)) 
    while(U): # while some vertex uncolored
        h = U[0] # get the vertex with highest degree of saturation
        for v in U: # break ties by highest degree
            if sat(v) > sat(h) or (sat(v) == sat(h) and deg(v) > deg(h)):
                h = v 
        v = U.pop(U.index(h))
        C[v] = min(cmap - S[v]) # paint the vertex with min color unused by adj
        for u in E[v]: # add vertex color to adjacents colors sets 
            S[u].add(C[v])
    return C