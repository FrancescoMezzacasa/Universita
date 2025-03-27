n = int(input('Inserisci il numero: '))

for j in range(1, n + 1):
    for i in range(1, n + 1):#stampa i numeri da 1 a n
        print(i * j, '\t', end='')
    print('')