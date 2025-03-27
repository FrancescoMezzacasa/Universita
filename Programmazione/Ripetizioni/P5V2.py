import doctest as dt

def sost(text):
    '''
    (str) -> str

    >>> sost('E nst?!?in')
    'E1nst3in'
    >>> sost('K12345678901234J')
    'K14J'
    >>> sost('2 rimane 2')
    '2rimane2'
    '''

    count = 0 #conto quanti non alfabetici
    ris = ''
    for i in range(len(text)):
        if(not text[i].isalpha()): #se è un carattere strano contalo
            count += 1
        else: #se è una lettera
            if(count > 0): #se hai gia visto qualche simbolo
                ris +=  str(count) #aggiungi il conto dei simboli visti finora
                count = 0 #resetta il count
            ris += text[i] #e in ogni caso aggiungi la lettera
    if(count > 0): #se sei alla fine (fuori dal for) e hai un conto
                    #vuol dire che finiva con un simbolo quinfdi aggiungi il count
        ris += str(count)
    return ris

dt.testmod(verbose=True)

