import random

w = int(input("Inserire la larghezza dell'immagine: "))
h = int(input("Inserire l'altezza dell'immagine: "))

matrice = []

for i in range(h):
    riga = []
    for j in range(w):
        a =  random.randint(0, 500)
        b = random.randint(501, 999)
        riga.append((a, b))
    matrice.append(riga)

for riga in matrice:
    for i in riga:
        print(i, end=" ")
    print()
