def pitagorica(A, B):
    '''
    (int, int) -> list<list>>
    '''
    matrice = []
    l = list(range(A, B+1))
    n = (B - A) + 1
    for i in range(n):
        riga = []
        for j in range(n):
            riga.append(l[j] * l[i])
        matrice.append(riga)
    return matrice

A = int(input("Inserire il primo numero: "))
B = int(input("Inserire il secondo numero: "))

matrice = pitagorica(A, B)
for riga in matrice:
    for i in riga:
        print(f"{i:3}", end=" ")
    print()