from dsatur import dsatur
from io_csv import read_in, save_out
from statistics import pstdev
import time
import sys

f_in = "grafo11.csv"

if(f_in.endswith(".csv")):
    (V, E) = read_in(f_in)

    start_t = time.time() 
    C = dsatur(V, E)
    end_t = time.time()    

    save_out(C, f_in.split(".")[0] + "_out.csv")
    edges = [len(E[e]) for e in E]

    print("No. nós:", len(V))
    print("No. arestas:", sum(edges)/2)
    print("Grau dos nós:")
    print("\tmínimo:", min(edges))
    print("\tmáximo:", max(edges))
    print("\tmédio:", sum(edges)/len(edges))
    print("\tdesvio padrão", pstdev(edges))
    print("No. cores:", max([C[c] for c in C]))
    print("Run time (s):", end_t-start_t)
else:
   print("Argument file invalid.", f_in)