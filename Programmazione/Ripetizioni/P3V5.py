import doctest as dt

def trattino(text):
    '''
    (str) -> str

    >>> trattino('allegramente')
    'a-lle-gra-me-nte'

    >>> trattino('aiuole')
    'aiuo-le'
    '''
    ris = ''
    vocali = 'aeiouAEIOU'
    for i in range(len(text) - 1):
        if(not text[i].isalpha()):
            print('Devi usare una parola singola senza simboli')
        else:
            ris += text[i]
            if text[i] in vocali and text[i+1] not in vocali:
                ris += '-'
    ris += text[-1]
    return ris
    
dt.testmod(verbose=True)

