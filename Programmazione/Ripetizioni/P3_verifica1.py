import math

def calcola_e(N):
    somma = 0
    for i in range(1, N + 1):
        somma += 1/(2*i)
    return somma

M = int(input('Quante righe vuoi vedere? '))

for j in range(1, M + 1):
    somma = calcola_e(j)
    diff = somma - math.e
    print('{}\t{}\t{}'.format(j, somma, diff))