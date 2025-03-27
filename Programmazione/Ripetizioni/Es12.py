import doctest

def fattoriale(x):
    if(x < 0):
        return -1
    if(x == 0 or x == 1):
        return 1
    prod = 1
    for i in range(1, x + 1):
        prod *= i
    return prod

def isFactorial_bool(n):
    '''
    (int) -> bool

    >>> isFactorial_bool(120)
    True

    >>> isFactorial_bool(100)
    False
    '''
    for i in range(1, n + 1):
        if(fattoriale(i) == n):
            return True
    return False

if(__name__ == '__main__'):
    doctest.testmod(verbose=True)