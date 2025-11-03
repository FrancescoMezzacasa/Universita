def trovaChiave(valore, dizionario):
    '''
    (int, dict) -> list
    Dato un valore e un dizionario la funzione
    ritorna una lista con le chiavi che hanno quel valore
    '''
    lista = []
    for chiave in list(dizionario.keys()):
        if(dizionario[chiave] == valore):
            lista.append(chiave)
    return lista

dizionario = {'Alba': 17, 'Bruno': 20, 'Caterina': 21, 'Dario': 17, 'Eliseo': 19, 'Fulvia': 25, 'Giacomo': 24, 'Hellen': 27, 'Iris': 19, 'Lara': 21, 'Mara': 21, 'Nora': 26, 'Omar': 28, 'Paola': 23, 'Quarto': 21, 'Romeo': 18, 'Silvia': 19, 'Tobia': 20, 'Ugo': 23, 'Viola': 24, 'Zeno': 25}

nuovo = dict()

eta = list(dizionario.values())

eta.sort()
eta = list(set(eta))

for e in eta:
    nuovo[e] = trovaChiave(e, dizionario)

for e in eta:
    print(e, nuovo[e])