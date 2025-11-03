dizionario = {'Alba': 17, 'Bruno': 20, 'Caterina': 21, 'Dario': 17, 'Eliseo': 19, 'Fulvia': 25, 'Giacomo': 24, 'Hellen': 27, 'Iris': 19, 'Lara': 21, 'Mara': 21, 'Nora': 26, 'Omar': 28, 'Paola': 23, 'Quarto': 21, 'Romeo': 18, 'Silvia': 19, 'Tobia': 20, 'Ugo': 23, 'Viola': 24, 'Zeno': 25}

risultato = dict()

for eta in dizionario.values():
    lista = list(dizionario.values())
    frequenza = lista.count(eta)
    risultato[eta] = frequenza

for chiave in risultato.keys():
    print(chiave, "-->", risultato[chiave])