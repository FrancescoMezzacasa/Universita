import random
import time
random.seed(time.time()*1000)

def creaMatrice(r, c, minimo, massimo):
    '''
    (int, int) -> list<list<int>>
    '''
    matrice = []
    for i in range(r):
        riga = []
        for j in range(c):
            riga.append(random.randint(minimo, massimo))
        matrice.append(riga)
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

def stampaColonna(indice, matrice):
    '''
    (int, list<list<int>>) -> None
    '''
    colonna = []
    for riga in matrice:
        colonna.append(riga[indice])
    print(colonna)
    return None


r = int(input("Inserire il numero di righe: "))
c = int(input("Inserire il numero di colonne: "))

minimo = int(input("Inserire il numero minimo: "))
massimo = int(input("Inserire il numero massimo: "))

matrice = creaMatrice(r, c, minimo, massimo)
stampaMatrice(matrice)

indice = int(input(f"Inserire un indice di colonna [0-{c-1}]: "))

stampaColonna(indice, matrice)
