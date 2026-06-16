#Esame 25/01/2024
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 1 gia fatto con il tutor

#Esercizio 2
#punto a
#2 variabili numero di voli di ogni tipo di aereo
#tipo 0 10 e 30 utile 10k
#tipo 1 20 e 100 utile 25k
mod1 = gp.Model()

naerei = 2
x = mod1.addMVar(naerei, lb=0, vtype=GRB.INTEGER)

#vincolo persone totali
mod1.addConstr(x[0]*(10+30) + x[1]*(20+100) <= 800)

#vincolo persone in top class
mod1.addConstr(x[0]*10 + x[1]*20 <= 150)

#funzione obiettivo
utili = np.array([10, 25])
mod1.setObjective(utili@x, GRB.MAXIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(naerei):
        print(x[i].X, end=' ')
    print()

#punto b
#trovare l utile minimo del primo tipo in modo che non serva usarne del secondo
#andava fatto graficamente, ha sbagliato il prof
'''
i vincoli rimangono uguali, quindi la regione ammissibile rimane la stessa
cambiare l'utile vuol dire cambiare un coeff. della funzione obiettivo,
bisogna notare che la soluzione in cui non si usano neanche uno del secondo tipo è gia un 
vertice della regione, ed è (15, 0), quindi la domanda diventa trovare il minimo coeff.
tale che l ottimo diventi quello
e fa 12500

anche il punto c si fa graficamente
'''

#punto d
mod2 = gp.Model()

naerei2 = 4
x = mod2.addMVar(naerei2, lb=0, vtype=GRB.INTEGER)

#vincolo persone totali
mod2.addConstr(x[0]*(10+30) + x[1]*(20+100) + x[2]*(17+55) + x[3]*(5+25) <= 800)

#vincolo persone in top class
mod2.addConstr(x[0]*10 + x[1]*20 + x[2]*17 + x[3]*5 <= 150)

mod2.addConstr(x[0] <= 2)

#funzione obiettivo
utili2 = np.array([10, 25, 15, 7])
mod2.setObjective(utili2@x - 15, GRB.MAXIMIZE)
mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(naerei2):
        print(x[i].X, end=' ')
    print()



