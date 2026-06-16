#Esame 18/09/2024
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#3*20 = 60 variabili binarie x_ij = 1 se il lavoratore i ha fatto lui il lavoro j

#!!! devo tenere una variabile y che rappresenta la differenza massima in valore assoluto tra tutte le differenze possibili
#sono io però che con i vincoli la devo far "agire" cosi

mod1 = gp.Model()

ndip = 3
ncommesse = 20
x = mod1.addMVar(shape=(ndip, ncommesse), vtype=GRB.BINARY)

y = mod1.addVar(lb=0)

#vincoli
carichiMax = np.array([80, 170, 50])
carichiCommesse = np.array([10, 20, 15, 10, 16, 9, 17, 13, 18, 14, 14, 16, 13, 18, 14, 16, 19, 13, 11, 17])
tempoCommesse = np.array([6, 8, 3, 12, 8, 5, 3, 10, 5, 2, 9, 7, 1, 6, 2, 2, 16, 7, 5, 4])

#ogni lavoro deve essere svolto da 1 solo dipendente
for j in range(ncommesse):
    mod1.addConstr(x[:, j].sum() == 1)#la somma delle colonne = 1 (un singolo lavoro è fatto da solo 1 dipendente)

#vincolo carichi
for i in range(ndip):
    mod1.addConstr((x[i, :]*carichiCommesse).sum() <= carichiMax[i])#la somma dei lavori che fa un dipendente deve essere minore del suo carico massimo

#voglio minimizzare la differenza tra le ore che lavorano
ore1 = (x[0, :] * tempoCommesse).sum()#somma dei tempi necessari alle commesse però solo quelle che ha fatto
ore2 = (x[1, :] * tempoCommesse).sum()
ore3 = (x[2, :] * tempoCommesse).sum()

#se voglio che y sia il max di tutte le differenze in valore assoluto
#basta che sia maggiore di tutte cosi sicuramente al minimo sara il max

mod1.addConstr(y >= ore1 - ore2)
mod1.addConstr(y >= ore2 - ore1)

mod1.addConstr(y >= ore1 - ore3)
mod1.addConstr(y >= ore3 - ore1)

mod1.addConstr(y >= ore2 - ore3)
mod1.addConstr(y >= ore3 - ore2)

mod1.setObjective(y, GRB.MINIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(ndip):
        for j in range(ncommesse):
            print(np.abs(x[i, j].X), end=' ')
        print()


#Esercizio 4
#4 variabili x_i = quantita di prodotto i portato

mod2 = gp.Model()

nris = 4
x = mod2.addMVar(nris, lb=0, ub=[300, 200, 150, GRB.INFINITY], vtype=GRB.INTEGER)

#vincolo capacità di trasporto
unitaSpazio = np.array([1, 3, 2, 1])
mod2.addConstr(unitaSpazio@x <= 800)

#vincolo disponibilita risorse
#numero massimo di risorse gia messo come upper bound tranne le coperte
mod2.addConstr(x[3] >= 0.5*(x[0]+x[1]+x[2]))

#vincolo distribuzione equa
mod2.addConstr(x[0] + x[1] >= 0.4*x.sum())

#vincolo limitazioni logistiche
mod2.addConstr(x[1]+x[2] <= x[0]+x[3])

#funzione obiettivo
importanza = np.array([4, 5, 9, 3])

mod2.setObjective(importanza@x, GRB.MAXIMIZE)
mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nris):
        print(x[i].X, end=' ')
    print()
