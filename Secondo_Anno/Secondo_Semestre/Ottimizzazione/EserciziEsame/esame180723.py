#Esame 18/07/2023
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 2
#2*5*3 = 30 variabili x_ijk = numero di trapani prodotti nello stabilimento i, dati al negozio j, nel mese k

mod1 = gp.Model()

nstab = 2
nneg = 5
nmesi = 3

x = mod1.addMVar(shape=(nstab, nneg, nmesi), lb=0, vtype=GRB.INTEGER)

#vincoli capacita produttiva mensile
for k in range(nmesi):
    mod1.addConstr(1.5*x[0, :, k].sum() <= 420)
    mod1.addConstr(2*x[1, :, k].sum() <= 330)

#vincoli richieste negozi
#primo mese
richieste1mese = np.array([150, 90, 50, 32, 100])
for j in range(nneg):
    mod1.addConstr(x[:, j, 0].sum() == richieste1mese[j])

#secondo mese
richieste2mese = np.array([30, 50, 180, 120, 0])
for j in range(nneg):
    mod1.addConstr(x[:, j, 1].sum() == richieste2mese[j])

#terzo mese
richieste3mese = np.array([0, 100, 75, 0, 200])
for j in range(nneg):
    mod1.addConstr(x[:, j, 2].sum() == richieste3mese[j])

#funzione obiettivo
#profitto = guadagni - costi di distribuzione - costi di produzione

guadagni = 100*x.sum()#li vendo tutti a 100€

coeffCostiDistr = np.array([[5, 4.7, 3.8, 3, 3.9], [4.6, 4.5, 2.4, 2.5, 3]])
costiDistr = gp.quicksum((x[:, :, k]*coeffCostiDistr).sum() for k in range(nmesi))

costiProd = 40*x[0, :, :].sum() + 35*x[1, :, :].sum()

mod1.setObjective(guadagni - costiDistr - costiProd, GRB.MAXIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for k in range(nmesi):
        for i in range(nstab):
            for j in range(nneg):
                print(x[i, j, k].X, end=' ')
            print()
        print()