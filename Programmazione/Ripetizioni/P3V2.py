import doctest as dt

def lunghezza(stringa):
    '''
    (str) -> int
    >>> lunghezza('I do not want words')
    5

    >>> lunghezza('Anche questa potrebbe andare bene')
    8
    '''
    count = 0
    count_max = 0
    for i in stringa:
        if(i != ' '):
            count += 1
        else:
            if(count > count_max):
                count_max = count
            count = 0
    if(count > count_max):
        count_max = count
    return count_max



if(__name__ == "__main__"):
    dt.testmod(verbose=True)


