import random
import time
random.seed(time.time()*1000)
def creaMatrice(r, c):
    '''
    (int, int) -> list<list<int>>
    '''
    lista = []
    for i in range(r*c):
        lista.append(random.randint(-10, 10))
    
    matrice = []
    for i in range(r):
        matrice.append(lista[c*i:c*i+c])
    return matrice

def stampaMatrice(matrice):
    '''
    (list<list<int>>) -> None
    '''
    for riga in matrice:
        for valore in riga:
            print(f"{valore:3}", end=" ")
        print()
    return None

r = int(input("Inserisci il numero di righe: "))
c = int(input("Inserisci il numero di colonne: "))

matrice = creaMatrice(r, c)
stampaMatrice(matrice)
print(matrice[1][2])


