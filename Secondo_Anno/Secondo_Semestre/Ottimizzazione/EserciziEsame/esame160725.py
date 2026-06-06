#Esame 16/07/2025
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
'''
xA = numero totale giornaliero di consegne fatte dal drone A
xB = numero totale giornaliero di consegne fatte dal drone B
uA = numero di consegne fatte con urgenza dal drone A
uB = numero di consegne fatte con urgenza dal drone B
zA = numero di consegne fatte in ZTL dal drone A
(il drone B per ora non può fare consegne in ZTL)
'''
mod1 = gp.Model()

xA = mod1.addVar(lb=0, ub=6, vtype=GRB.INTEGER)
xB = mod1.addVar(lb=0, ub=5, vtype=GRB.INTEGER)#massimo 5 perche ogni consegna sta 2 km e ne puo fare al massimo 10
uA = mod1.addVar(lb=0, vtype=GRB.INTEGER)
uB = mod1.addVar(lb=0, vtype=GRB.INTEGER)
zA = mod1.addVar(lb=0, vtype=GRB.INTEGER)

#vincoli sulle variabili di urgenza e ZTL
mod1.addConstr(uA <= xA)
mod1.addConstr(uB <= xB)
mod1.addConstr(zA <= xA)

mod1.addConstr(uA + zA <= xA)

#vincolo sulle consegne urgenti
mod1.addConstr(uA + uB >= 0.3 * (xA + xB))

#vincolo sulle consegne in ZTL
mod1.addConstr(zA <= 4)

#funzione obiettivo: massimo numero di consegne
mod1.setObjective(xA + xB, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    print(xA.X, xB.X, uA.X, uB.X, zA.X)


#Esercizio 4
'''
Ho 5 distretti, i possibili incroci sarebbero 5*5 = 25 ma non si spostano bici da un distretto
a se stesso, quindi diventano 20.
Uso quindi 20 variabili x_ij = numero di bici spostate dal distreto i al distretto j
(Le variabili possono anche essere 0, indicando che da quel distretto non sono uscite bici verso il distretto
j, ma questo cmq non implica che non possa averle ricevute dall'altro distretto)
Per comodità le matrici le creo cmq in una matrice 5*5 però forzerò a 0 le variabili della diagonale
'''

mod2 = gp.Model()

ndistr = 5
x = mod2.addMVar(shape=(ndistr, ndistr), lb=0, ub=10, vtype=GRB.INTEGER)#il testo mi dice che posso spostare al max 10 bici

for i in range(ndistr):
    mod2.addConstr(x[i, i] == 0)
    #le variabili delle diagonali (scambio bici con se stesso) devono essere nulle

#vincolo sulle ore totali
mod2.addConstr(x.sum() * 0.1 <= 80)

#vincolo sulle emissioni
distanze = np.array([[0, 5, 8, 6, 10], [5, 0, 4, 5, 9], [8, 4, 0, 6, 7], [6, 5, 6, 0, 5], [10, 9, 7, 5, 0]])
mod2.addConstr((x*distanze).sum() * 0.2 <= 100)

#vincolo sulla distanza massima percorribile
mod2.addConstr((x*distanze).sum() <= 200)

#vincolo sulla richiesta minima
fineGiornata = np.array([20, 50, 20, 40, 25])
richMin = np.array([35, 30, 25, 30, 35])
#deve essere che il valore a fine giornata + le bici messe li - le bici tolte da li sia maggiore del minimo
for i in range(ndistr):
    biciMesse = gp.quicksum(x[j, i] for j in range(ndistr))#le bici messe sono quelle arrivate da tutti gli altri distretti
    biciTolte = gp.quicksum(x[i, j] for j in range(ndistr))#le bici tolte sono quelle date a tutti gli altri
    mod2.addConstr(fineGiornata[i] + biciMesse - biciTolte >= richMin[i])

#funzione obiettivo
costi = np.array([[0, 4, 6, 5, 7], [4, 0, 3, 4, 6], [6, 3, 0, 5, 5], [5, 4, 5, 0, 4], [7, 6, 5, 4, 0]])
mod2.setObjective((costi*x).sum(), GRB.MINIMIZE)

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(ndistr):
        for j in range(ndistr):
            print(x[i, j].X, end=' ')
        print()