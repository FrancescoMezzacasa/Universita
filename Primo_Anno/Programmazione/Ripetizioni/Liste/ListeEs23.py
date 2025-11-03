import doctest

def seleziona(lista, minimo, massimo):
    '''
    (list, float, float) -> list

    >>> seleziona([2, 4, 5, 10, 13, 17], 3, 10)
    [4, 5, 10]
    '''
    risultato = []
    for valore in lista:
        if(valore >= minimo and valore <= massimo):
            risultato.append(valore)
    return risultato


doctest.testmod(verbose=True)