from dsatur import dsatur
from io_csv import read_in, save_out
from statistics import stdev

(V, E) = read_in("in.csv")  
C = dsatur(V, E)
save_out(C, "out.csv")

edges = [len(E[e]) for e in E]

print("No. nós:", len(V))
print("No. arestas:", sum(edges)/2)
print("Grau dos nós:")
print("\tmínimo:", min(edges))
print("\tmáximo:", max(edges))
print("\tmédio:", sum(edges)/len(edges)/2)
print("\tdesvio padrão", stdev(edges))
print("No. cores:", max([C[c] for c in C]))