import doctest as dt

def sottostringa(text, pattern):
    '''
    (str, str) -> bool
    >>> sottostringa('campane', 'pane')
    True
    >>> sottostringa('animale', 'ciao')
    False
    >>> sottostringa('arcobaleno', 'coba')
    True
    '''
    j = 0
    tuttiDiversi = True
    for i in range(len(text) - len(pattern) + 1):
        if(text[i] == pattern[j]):
            tuttiDiversi = False
            if(j + 1 == len(pattern)):
                return True
            j += 1
        else:
            if(j > 0):
                return False
    if(tuttiDiversi):
        return False
    return True

    

dt.testmod(verbose=True)