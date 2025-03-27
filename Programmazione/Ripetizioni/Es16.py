import doctest as dt

#funzione ausiliaria
def occorrenze(text, x):
    conto = 0
    for i in text:
        if(i == x):
            conto += 1
    return conto

def singole(text):
    '''
    (str) -> str
    
    >>> singole('doppio')
    'di'
    >>> singole('zuzzurellone')
    'ron'
    >>> singole('aiuole')
    'aiuole'
    '''

    nuova = ''
    for i in text:
        if(occorrenze(text, i) == 1):#se compare solo una volta nella parola aggiungila alla nuova
            nuova += i

    return nuova

dt.testmod(verbose=True)