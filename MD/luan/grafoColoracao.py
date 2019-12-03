import csv
import time
import statistics
from fractions import Fraction as F

start = time.time() 
grafo = []
saturacoes = []
cores = []
grafoEntrada = open('../grafo11.csv', 'r')
reader = csv.reader(grafoEntrada)
for linha in reader:
    del linha[0] ##tira o primeiro elemento que indica o indice
    lisNumber = []
    for i in linha:
        lisNumber.append(int(i))
    grafo.append(lisNumber) ##gera o grafico em forma de lista
    saturacoes.append(0)
    cores.append(0)
##A posição indica o elemento, e a lista indica os nós adjacentes

graus = []
for i in grafo:
    graus.append(len(i))

maiorGrau = max(graus)
menorGrau = min(graus)
media = sum(graus)/len(graus)
numeroArestas = 0
for i in grafo:
    numeroArestas += len(i)
numeroArestas = numeroArestas/2

    
cores[maiorGrau] = 1

qtd = 0
for i in grafo[maiorGrau]:
    saturacoes[qtd] += 1
    qtd += 1

while 0 in cores:
    maiorSat = max(saturacoes)
    grauNo = 0
    n = -1
    no = 0
    for i in saturacoes:
        n += 1
        if i == maiorSat and graus[n] > grauNo:
            grauNo = graus[n]
            no = n
    
    menorCor = False
    coresVizinhos = []
    cor = 1
    for i in grafo[no]:
        coresVizinhos.append(cores[i-1])
        
        
    while not menorCor:
        if cor not in coresVizinhos:
            menorCor = True
        else:
            cor += 1  
    cores[no] = cor 

    len(grafo[no])
    len(cores)
    len(saturacoes)
    for i in grafo[no]:
        if cores[i-1] == 0: 
            saturacoes[i-1] += 1
 
    saturacoes[no] = 0

    ##print(maiorSat)
end = time.time()

print ("No. nós | No aretas |               Grau dos nós           | No. Cores | Run time (s)")
print ("        |           | minimo  máximo  médio  desvio padrão |           |             ")
print ("  ", len(grafo), "  |  ", numeroArestas, "  |  ", menorGrau, "    ", maiorGrau, "    ", media, "    ",  round(statistics.pstdev(graus),4), "   |     ", max(cores), "   |  ", end-start)
            
            
