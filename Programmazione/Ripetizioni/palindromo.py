import doctest as dt

def pal(text):
    '''
    (str) -> bool

    >>> pal('anna')
    True
    >>> pal('stepny')
    False
    >>> pal('arira')
    True
    '''

    #prima disegnare
    for i in range(len(text)):
        if(text[i] != text[len(text) - 1 - i]):#se il primo non è uguale all ultimo il secondo al penultimo...
            return False
        
    return True

dt.testmod(verbose=True)