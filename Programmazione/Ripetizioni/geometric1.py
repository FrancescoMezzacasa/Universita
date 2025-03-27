import doctest as dt

def serie_geometrica(n, q):
    '''
    somma i primi n termini 
    della serie geometrica di ragione 1/q

    (int, float) -> float

    >>> serie_geometrica(5, 2)
    0.96875

    >>> serie_geometrica(4, 1)
    4.0
    '''
    if(q == 0):
        print('q non può essere 0')
        return None
    else:
        somma = 0
        #sbagliato il libro, il primo termine
        #della serie non è 1 ma (1/q)
        for i in range(1, n + 1):
            somma += (1/q)**i
        return somma

if(__name__ == '__main__'):
    dt.testmod(verbose=True)
    