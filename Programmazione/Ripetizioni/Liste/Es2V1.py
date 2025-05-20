import random
import time
random.seed(time.time() * 1000)

n = int(input("Inserisci un numero: "))
lista = []

for i in range(n):
    x = random.randint(-10, 10)
    y = random.randint(-10, 10)
    lista.append((x, y))

print(lista)

conto = 0
for coppia in lista:
    if(coppia[0] >= -5 and coppia[0] <= 5 and coppia[1] >= -5 and coppia[1] <= 5):
        conto += 1

print(conto)

rapporto = 4* conto / n
print(rapporto)