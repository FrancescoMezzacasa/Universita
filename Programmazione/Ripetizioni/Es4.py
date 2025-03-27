import doctest as dt

def percent(text):
    '''
    (str) -> float
    >>> percent('Hello, world!')
    30
    >>> percent('Aiuole')
    83
    '''

    text = text.lower()#tutto in minuscolo

    conto_vocali = 0#conta le vocali man mano che le trovo
    conto_lettere = 0#conta tutti i caratteri isalpha (mi serve per la percentuale)
    for i in text:
        if(i.isalpha()):
            conto_lettere += 1
            if(i in 'aeiou'):
                conto_vocali += 1

    ris = (conto_vocali / conto_lettere) * 100
    return int(ris)

dt.testmod(verbose=True)