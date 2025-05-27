import random
import time
random.seed(time.time()*1000)

n = int(input("Inserisci un numero: "))

lista = []
for i in range(n):
    numeratore = random.randint(1, 9)
    denominatore = random.randint(1, 9)
    lista.append((numeratore, denominatore))

for tupla in lista:
    print(str(tupla[0]) + "/" + str(tupla[1]), end=" ")
print()

for tupla1 in lista:
    for tupla2 in lista:
        if(tupla1 != tupla2):
            if(tupla1[0]/tupla1[1] == tupla2[0]/tupla2[1]):
                print(str(tupla1[0])+"/"+str(tupla1[1])+"="+str(tupla2[0])+"/"+str(tupla2[1]))