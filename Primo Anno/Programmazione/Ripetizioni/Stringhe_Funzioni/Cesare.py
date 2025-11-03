import doctest as dt


def codifica(text, offset):
    '''
    (str, int) -> str
    >>> codifica('Alea iacta est!', 3)
    'DOHD LDFWD HVW!'
    >>> codifica('HAL', 1)
    'IBM'
    >>> codifica('IBM', -1)
    'HAL'
    >>> codifica('abjurer', 13)
    'NOWHERE'
    >>> codifica(codifica('abjurer', 13), 13)
    'ABJURER'
    '''

    text = text.upper()#tutto maiuscolo
    ris = ''
    for i in text:
        if(i.isalpha() == False):#se non è una lettera lascio così
            ris += i
        else:
            delta = ord(i) + offset
            if(delta > 90):#se vado oltre alla Z torno alla A
                delta -= 26
            ris += chr(delta)
        
    return ris

dt.testmod(verbose=True)