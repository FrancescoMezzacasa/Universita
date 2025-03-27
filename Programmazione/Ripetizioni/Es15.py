import doctest as dt

def sort_case(text):
    '''
    (str) -> str
    
    >>> sort_case('PlYoTHOveN')
    'lovePYTHON'
    >>> sort_case('PYTHONlove')
    'lovePYTHON'
    '''
    nuova = ''

    for i in text:
        if(i.islower()):#se è minuscolo lo aggiungi alla nuova
            nuova += i

    for i in text:
        if(i.isupper()):#se è maiuscolo lo aggiungi alla nuova
            nuova += i
    
    return nuova

dt.testmod(verbose=True)
