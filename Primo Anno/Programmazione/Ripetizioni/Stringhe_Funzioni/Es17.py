import doctest as dt

def rep(text, old, new):
    '''
    (str, str) -> str
    >>> rep('ooooo', 'oo', 'ca')
    'cacao'
    >>> rep('cicciogamer', 'ci', 'dio')
    'diocdioogamer'
    '''
    if(old not in text):
        return text
        #se non c'è mai ritorno il testo così com'è
    else:
        for i in range(len(text)-len(old)):
            if(text[i:i+len(old)] == old):#quando l'hai trovato
                text = text[:i] + new + text[i + len(old):]
                #aggiungi il testo prima dell'occorrenza, poi la nuova stringa, poi il testo dopo l'occorrenza
        return text
        
dt.testmod(verbose=True)