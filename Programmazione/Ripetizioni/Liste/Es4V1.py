dizionario = {'Alba': (78, 182), 'Bruno': (79, 172), 'Caterina': (68, 165), 'Dario': (64, 162), 'Eliseo': (75, 188), 'Fulvia': (76, 184), 'Giacomo': (69, 172), 'Hellen': (87, 192), 'Iris': (68, 177), 'Lara': (72, 169), 'Mara': (67, 173), 'Nora': (69, 165), 'Omar': (77, 173), 'Paola': (70, 180), 'Quarto': (72, 172), 'Romeo': (69, 172), 'Silvia': (73, 178), 'Tobia': (65, 174), 'Ugo': (81, 176), 'Viola': (71, 172), 'Zeno': (70, 166)}


risultato = dict()

for nome in dizionario.keys():
    tupla = dizionario[nome]
    peso = tupla[0]
    altezza = tupla[1] / 100
    bmi = peso/(altezza ** 2)

    #aggiungo una nuova coppia chiave-valore
    risultato[nome] = bmi

print(risultato)

valore_minimo = min(risultato.values())
valore_massimo = max(risultato.values())
for nome in risultato.keys():
    if(risultato[nome] == valore_minimo):
        nome_minimo = nome
    if(risultato[nome] == valore_massimo):
        nome_massimo = nome

print("Il BMI minimo corrisponde a:")
print(nome_minimo, valore_minimo)

print("Il BMI massimo corrisponde a:")
print(nome_massimo, valore_massimo)
