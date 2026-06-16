#Esame 17/07/2024
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#4 variabili x_i = numero di libri della categoria i acquistati
#0 romanzi brevi
#1 romanzi lunghi
#2 manuali brevi
#3 manuali lunghi

mod1 = gp.Model()

ncategorie = 4

x = mod1.addMVar(ncategorie, lb=0, vtype=GRB.INTEGER)

#vincolo budget
prezzi = np.array([5, 8, 7, 3])
mod1.addConstr(prezzi@x <= 300)

#vincolo spessore
spess = np.array([2, 4, 3, 5])
mod1.addConstr(spess@x <= 180)

#vincolo numeri minimi
mod1.addConstr(x[0] + x[1] >= 12)
mod1.addConstr(x[2] + x[3] >= 10)

#altri vincoli
mod1.addConstr(x[0] == 2*x[2])

mod1.addConstr(x[0] + x[3] >= 2*(x[1] + x[2]))

#vincolo spesa
mod1.addConstr(x[1]*prezzi[1] >= 0.25*(prezzi@x))

#funzione obiettivo
gradimento = np.array([4, 3, 5, 6])
mod1.setObjective(gradimento@x, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(ncategorie):
        print(x[i].X, end=' ')
    print()

#Esercizio 4
#7 variabili x_i = numero di dipendenti che inizia il turno nel giorno i

mod2 = gp.Model()

ngiorni = 7
x = mod2.addMVar(ngiorni, lb=0, vtype=GRB.INTEGER)

#vincolo numero minimo
#noto che nel giorno i sono presenti tutti tranne chi inizia il giorno dopo e quello dopo ancora
#ad esempio di lunedi ci sono tutti tranne chi ha iniziato il turno martedi e mercoledi, 
#perche di riposo avranno rispettivamente domenica e lunedi, e lunedi e martedi
#quindi nel vincolo posso usare la somma totale meno chi non c'è, stando attento di quando
#un giorno sarebbe dopo il settimo farlo tornare al primo (uso il modulo)
personaleMinimo = np.array([28, 18, 20, 26, 22, 13, 13])

for i in range(ngiorni):
    mod2.addConstr(x.sum() - (x[(i+1)%7] + x[(i+2)%7]) >= personaleMinimo[i])
    #x.sum() sono tutti i dipendenti di tutti i giorni, ci tolgo la somma di quelli che iniziano
    #il giorno dopo e quello dopo ancora, faccio %7 cosi se verrebbe che uno ha iniziato nel giorno 8 torna il giorno 1

#funzione obiettivo
#prendono tutti di base almeno 250€, chi lavora il sabato ne aggiungo 25 (prende 75 invece che 50)
#e chi lavora domenica ne aggiungo 35

#lunedi non fa weekend quindi 250
#martedi fa solo sabato quindi 275
#mercoledi entrambi quindi 310
#giovedi entrambi quindi 310
#venerdi entrambi quindi 310
#sabato entrambi quindi 310
#domenica non fa sabato quindi 285

stip = np.array([250, 275, 310, 310, 310, 310, 285])

mod2.setObjective(stip@x, GRB.MINIMIZE)

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(ngiorni):
        print(x[i].X, end=' ')
    print()

'''
il punto b chiede che non ci sia differenza di stipendio maggiore a 40€
l'unico caso che non va bene è la differenza tra chi fa tutto il weekend e chi inizia il lunedi 
e fa solo giorni della settimana, perché la differenza tra gli stipendi è 310 - 250 = 60,
mentre in tutti gli altri casi va bene.
Bisogna considerare, però che gli stipendi sono fissi, quindi se ci sarà qualcuno che inizia il lunedi
e qualcuno che inizia mercoledi, giovedi, venerdi o sabato questa differenza si presenterà sempre.
Dovrà quindi essere che o nessuno inizia il lunedi, o nessuno inizia il mercoledi, giovedi, venerdi e sabato
'''

#Caso 1, tolgo il lunedi
toltoLunedi = mod2.addConstr(x[0] == 0)
mod2.update()

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    turni1 = []
    for i in range(ngiorni):
        turni1.append(x[i].X)
val1 = mod2.ObjVal

#Caso 2, tolgo gli altri
mod2.addConstr(x[2] + x[3] + x[4] + x[5] == 0)
mod2.remove(toltoLunedi)
mod2.update()

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    turni2 = []
    for i in range(ngiorni):
        turni2.append(x[i].X)
val2 = mod2.ObjVal

if(val1 < val2):
    print("Conviene non far iniziare il lunedi e organizzare i turni cosi:")
    for i in range(ngiorni):
        print(turni1[i], end=' ')
    print()
elif(val1 > val2):
    print("Conviene togliere chi inizia il mercoledi, giovedi, venerdi e sabato, organizzando cosi:")
    for i in range(ngiorni):
        print(turni2[i], end=' ')
    print()
else:
    print("Le due soluzioni sono uguali, quindi i turni si possono organizzare in entrambi i modi")
    for i in range(ngiorni):
        print(turni1[i], end=' ')
    print()
    for i in range(ngiorni):
        print(turni2[i], end=' ')
    print()