import doctest as dt

def haDoppie(text):
    '''
    (str) -> bool

    >>> haDoppie('doppia')
    True
    >>> haDoppie('arcobaleno')
    False
    '''
    text = text.lower()

    for i in range(len(text) - 1):#testo fino alla penultima
        if(text[i] == text[i + 1]):#vedo se quella su cui sono è uguale a quella dopo
            return True
        
    return False

dt.testmod(verbose=True)