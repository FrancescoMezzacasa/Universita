dizionario = {"Alessandro": "Juventus", "Giovanni": "Inter", "Luca": "Milan", "Marco": "Napoli",
"Matteo": "Roma", "Francesco": "Lazio", "Andrea": "Fiorentina", "Stefano": "Juventus",
"Roberto": "Inter", "Paolo": "Milan", "Davide": "Napoli", "Nicola": "Roma", "Giorgio":
"Lazio", "Simone": "Fiorentina", "Antonio": "Juventus", "Federico": "Inter", "Leonardo":
"Milan", "Salvatore": "Napoli", "Michele": "Roma", "Enrico": "Lazio"}

nuovo = dict()

squadre = list(dizionario.values())
squadre = list(set(squadre))

valori = list(dizionario.values())

for squadra in squadre:
    nuovo[squadra] = valori.count(squadra)

for k, v in nuovo.items():
    print(k, end = "")
    print(":", v)
      