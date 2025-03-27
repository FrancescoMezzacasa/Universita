import doctest as dt

def finiscecon(text, pattern):
    '''
    (str, str) -> bool
    >>> finiscecon('campane', 'pane')
    True
    >>> finiscecon('animale', 'ciao')
    False
    '''
    for i in range(len(pattern)):
        if(pattern[-i-1] != text[-i-1]):
            return False
    return True

dt.testmod(verbose=True)