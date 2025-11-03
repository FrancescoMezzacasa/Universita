import random
import time
import math
random.seed(time.time() * 1000)

def distanza(punto):
    '''
    (tuple) -> float
    '''
    dist = math.sqrt(punto[0]**2 + punto[1]**2)
    return dist

n = int(input("Inserisci un numero: "))

l = []

for i in range(n):
    x = random.randint(-9, 9)
    y = random.randint(-9, 9)
    l.append((x, y))

distanza_minima = distanza(l[0])
punto_minimo = l[0]
for i in l:
    if(distanza(i) < distanza_minima):
        distanza_minima = distanza(i)
        punto_minimo = i

print("Il punto più vicino è ", punto_minimo, "e la sua distanza è ", distanza_minima)

distanza_massima = distanza(l[0])
punto_massimo = l[0]
for i in l:
    if(distanza(i) > distanza_massima):
        distanza_massima = distanza(i)
        punto_massimo = i

print("Il punto più lontano è ", punto_massimo, "e la sua distanza è ", distanza_massima)
