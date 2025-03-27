import doctest as dt

def unione(a, b):
    '''
    (str, str) -> str
    >>> unione('12345', 'qwerty')
    '1q2w3e4r5ty'
    >>> unione('ciaone', 'CIAONE')
    'cCiIaAoOnNeE'
    >>> unione('acbln', 'roaeo')
    'arcobaleno'
    >>> unione('ciao', 'ARCOBALENO')
    'cAiRaCoOBALENO'
    >>> unione('arcobaleno', 'CIAO')
    'aCrIcAoObaleno'
    '''
    c = ''
    for i in range(0, min(len(a), len(b))):
        c += a[i]#aggiungo una per parola a quella nuova
        c += b[i]
    
    #quando ho finito devo vedere quale parola è più lunga e aggiungere le lettere mancanti
    if(len(a) < len(b)):
        c += b[len(a):]
    if(len(b) < len(a)):
        c += a[len(b):]
    return c

dt.testmod(verbose=True)