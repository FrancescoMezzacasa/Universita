import random

def stampaMatrice(matrice):
    '''
    (list<list<int>>) -> None
    '''
    for riga in matrice:
        for valore in riga:
            print(valore, end=" ")
        print()
    return None

w = int(input("Inserire la larghezza dell'immagine: "))
h = int(input("Inserire l'altezza dell'immagine: "))

matrice = []

for i in range(h):
    riga = []
    for j in range(w):
        a = random.randint(0, 255)
        b = random.randint(0, 255)
        c = random.randint(0, 255)
        riga.append((a, b, c))
    matrice.append(riga)

stampaMatrice(matrice)


