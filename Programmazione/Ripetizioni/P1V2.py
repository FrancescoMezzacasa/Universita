import doctest as dt

def codfisc(codice):
    '''
    (str) -> int

    >>> codfisc('RSSRRT80A01D229D')
    1
    >>> codfisc('RCCLCU05C57H537K')
    17
    '''

    #noto che il giorno è agli indici 9 e 10
    giorni = int(codice[9:11])
    if(giorni > 31): #se è femmina
        giorni -= 40
    return giorni

dt.testmod(verbose=True)