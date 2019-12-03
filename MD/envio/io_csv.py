import csv

def read_in(path):
    with open(path, 'r') as file:
        table = list(csv.reader(file, delimiter=','))
        V = []
        E = {}
        for row in table:
            V.append(int(row[0]))
            E[V[-1]] = [int(e) for e in row[1:]]
    return (V, E)

def save_out(C, path):
    with open(path, 'w') as file:
        table = csv.writer(file, delimiter=',')
        for v in C:
            table.writerow([v, C[v]])