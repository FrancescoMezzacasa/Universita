dizionario = {'Alba': 17, 'Bruno': 20, 'Caterina': 21, 'Dario': 17, 'Eliseo': 19, 'Fulvia': 25, 'Giacomo': 24, 'Hellen': 27, 'Iris': 19, 'Lara': 21, 'Mara': 21, 'Nora': 26, 'Omar': 28, 'Paola': 23, 'Quarto': 21, 'Romeo': 18, 'Silvia': 19, 'Tobia': 20, 'Ugo': 23, 'Viola': 24, 'Zeno': 25}

nomi_minimi = []
nomi_massimi = []

lista_eta = list(dizionario.values())

eta_minima = min(lista_eta)
eta_massima = max(lista_eta)

for nome in dizionario.keys():
    if(dizionario[nome] == eta_minima):
        nomi_minimi.append(nome)
    if(dizionario[nome] == eta_massima):
        nomi_massimi.append(nome)

print("Età minima", eta_minima, ">", nomi_minimi)
print("Età massima", eta_massima, ">", nomi_massimi)
