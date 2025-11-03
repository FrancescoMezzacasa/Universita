import doctest

def unisci(lista1, lista2):
    '''
    (list, list) -> list

    >>> unisci([1, 2, 3], [4, 5, 6])
    [1, 4, 2, 5, 3, 6]

    >>> unisci([1, 2, 3], [4, 5, 6, 7, 8])
    [1, 4, 2, 5, 3, 6, 7, 8]

    >>> unisci([1, 2, 3, 7, 8], [4, 5, 6])
    [1, 4, 2, 5, 3, 6, 7, 8]
    
    '''
    risultato = []
    #prima prendo i pezzi in comune
    i = 0 #indice per lista1
    j = 0 #indice per lista2
    while(i < len(lista1) and j < len(lista2)):
        risultato.append(lista1[i])
        risultato.append(lista2[j])
        i += 1
        j += 1
    if(i < len(lista1)): #se la prima non è ancora finita
        while(i < len(lista1)):
            risultato.append(lista1[i]) #aggiungi i mancanti
            i += 1
    elif(j < len(lista2)): #stessa cosa se la seconda non è finita
        while(j < len(lista2)):
            risultato.append(lista2[j])
            j += 1
    
    return risultato

doctest.testmod(verbose=True)