import doctest as dt

def sostituisci(stringa):
    '''
    (str) -> str
    >>> sostituisci('AAA Abbaco ccccdd')
    '3A A2baco 4c2d'

    >>> sostituisci('Aabbb gghiokkkkk')
    'Aa3b 2ghio5k'
    '''

    count = 1
    ris = ''
    for i in range(len(stringa) - 1):
        if stringa[i].isalpha(): #se è una lettera
            if(stringa[i] == stringa[i + 1]): #se è uguale a quella dopo
                if(i + 2 == len(stringa)): #se la prossima è l'ultima
                    count += 1
                    ris += str(count)
                    ris += stringa[i]
                    return ris
                else: #se invece sono in un altra a caso non l ultima
                    count += 1
            else: #se quella dopo è diversa
                if(count == 1): #se ne ho vista solo una non mettere nessun numero
                    ris += stringa[i]
                else:# altrimenti mettici il numero
                    ris += str(count)
                    ris += stringa[i]
                    count = 1
                if(i + 2 == len(stringa)):# se quella dopo è diversa ma è l ultima aggiungila così
                    ris += stringa[i + 1]
        else:
            ris += stringa[i]

    return ris

if(__name__ == "__main__"):
    frase = input('Inserire la frase da testare (solo lettere): ')
    print(sostituisci(frase))


