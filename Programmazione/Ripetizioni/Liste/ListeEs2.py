import doctest

def differenza(listaIniziale):
    '''
    (list) -> list

    >>> differenza([3, 5, 8, 2])
    [2, 3, -6]
    '''
    listaFinale = []
    for i in range(1, len(listaIniziale)):
        listaFinale.append(listaIniziale[i] - listaIniziale[i-1])
    return listaFinale

doctest.testmod(verbose=True)