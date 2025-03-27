import doctest as dt

def split(text):
    '''
    (str) -> str

    >>> split('Hello, World!\\nPython 3.9 is fantastic!')
    Hello,
    World!
    Python
    3.9
    is
    fantastic!
    '''

    parola = ''#creo la variabile dove metterò ogni parola
    for i in text:
        if(i.isspace()):
            print(parola)#se sei arrivato alla fine della parola stampamela e poi resettala a vuota
            parola = ''
        else:
            parola += i#se sei dentro ad una parola aggiungi le lettere alla variabile parola
    if(text[-1].isspace() == False):
        print(parola)#stampa l'ultima parola se non è uno spazio
    return None

dt.testmod(verbose=True)