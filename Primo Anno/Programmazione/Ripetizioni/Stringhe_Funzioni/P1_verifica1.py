import doctest

def isSorted(a, b, c):
    '''
    (int, int, int) -> bool

    >>> isSorted(1, 3, 5)
    True

    >>> isSorted(1, 3, 3)
    True

    >>> isSorted(6, 3, 5)
    False
    '''
    if(a <= b and b <= c):
        return True
    return False

if(__name__ == '__main__'):
    doctest.testmod(verbose=True)