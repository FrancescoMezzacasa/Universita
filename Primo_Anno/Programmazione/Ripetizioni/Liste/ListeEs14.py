def binary_increment(numero):
    '''
    (list) -> list
    Incrementa il numero di 1
    '''
    ris = []
    riporto = 0
    #ora trattiamo l'ultima cifra
    if(numero[-1] == 0):
        ris.append(1)
    elif(numero[-1] == 1):
        ris.append(0)
        riporto = 1
    
    #ora vediamo le altre cifre
    for i in range(2, len(numero) + 1):
        if(numero[-i] == 0 and riporto == 0):
            ris.insert(0, 0)
        elif(numero[-i] == 0 and riporto == 1):
            ris.insert(0, 1)
            riporto = 0
        elif(numero[-i] == 1 and riporto == 0):
            ris.insert(0, 1)
            riporto = 0
        elif(numero[-i] == 1 and riporto == 1):
            ris.insert(0, 0)
            riporto = 1
    if(riporto == 1):
        ris.insert(0, 1)
    return ris

numero = [0]
while(sum(numero) < 4):
    print(numero)
    numero = binary_increment(numero)
print(numero)