import doctest as dt

def finiscecon(text, pattern):
    '''
    (str, str) -> bool
    >>> finiscecon('campane', 'pane')
    True
    >>> finiscecon('animale', 'ciao')
    False
    '''
    j = 0#lo uso per scorrere su pattern
    for i in range(len(text) - len(pattern), len(text)):#inizio da dove inizierebbe il suffisso nella parola text
        #cioè se la parola è campane e so quanto è lungo il suffisso pane inizio da camPane
        if(text[i] != pattern[j]):
            return False
        else:
            j += 1

    return True

dt.testmod(verbose=True)