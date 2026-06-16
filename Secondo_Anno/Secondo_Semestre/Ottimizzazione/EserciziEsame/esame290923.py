#Esame 29/09/2023
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 1
#3*2 = 6 variabili x_ij = numero scatole di batterie di tipo i prodotte nell'impianto j

mod1 = gp.Model()

ntipi = 3
nimpianti = 2

x = mod1.addMVar(shape=(ntipi, nimpianti), lb=0, ub=([30, 40], [30, 20], [10, 20]), vtype=GRB.INTEGER)

rame = np.array([[0, 0], [1, 2], [2, 5]])
nickel = np.array([[2, 4], [3, 2], [0, 0]])

#vincolo sulla massima quantita di materiali
mod1.addConstr((rame*x).sum() <= 200)
mod1.addConstr((nickel*x).sum() <= 200)

#vincolo finale
mod1.addConstr(x[0, :].sum() >= 2*x[1, :].sum())
mod1.addConstr(x[0, :].sum() <= x[2, :].sum())

#funzione obiettivo
#profitto = guadagno da vendita - costo acquisto materie - costo manodopera

coeffManodopera = np.array([[12, 8], [6, 10], [4, 3]])
costoManodopera = (coeffManodopera*x).sum()

costoRame = 5*((rame*x).sum())
costoNickel = 7*((nickel*x).sum())
costoMaterie = costoRame + costoNickel

prezzi = np.array([45, 40, 30])
guadagni = gp.quicksum(x[i, :].sum()*prezzi[i] for i in range(ntipi))

mod1.setObjective(guadagni - costoMaterie - costoManodopera, GRB.MAXIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(ntipi):
        for j in range(nimpianti):
            print(x[i, j].X, end=' ')
        print()
    
#Esercizio 2
#3 variabili x_i = quante macchine di tipo i faccio al giorno
#0 = economica
#1 = normale
#2 = lusso

mod2 = gp.Model()

ncategorie = 3
x = mod2.addMVar(ncategorie, lb=0)

#vincoli sui tempi dei robot
tempiA = np.array([20, 30, 62])
tempiB = np.array([31, 42, 51])
tempiC = np.array([16, 81, 10])

mod2.addConstr(tempiA@x <= 8*60)#converto ore a minuti
mod2.addConstr(tempiB@x <= 8*60)
mod2.addConstr(tempiC@x <= 5*60)

#altri vincoli
mod2.addConstr(x[2] <= 0.2*x.sum())

mod2.addConstr(x[0] >= 0.4*x.sum())

#funzione obiettivo
prezzi = np.array([1000, 1500, 2200])
mod2.setObjective(prezzi@x, GRB.MAXIMIZE)
mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(ncategorie):
        print(x[i].X, end=' ')
    print()

#punto 2
#se ora ogni robot può farne una da solo devo mettere 3*3 = 9 variabili
#x_ij = macchine prodotte dal robot i di tipo j
mod3 = gp.Model()

ncategorie = 3
nrobot = 3

x = mod3.addMVar(shape=(nrobot, ncategorie), lb=0)

#vincoli tempi robot
tempiA = np.array([20, 30, 62])
tempiB = np.array([31, 42, 51])
tempiC = np.array([16, 81, 10])

mod3.addConstr(tempiA@(x[0, :]) <= 8*60)#converto ore a minuti
mod3.addConstr(tempiB@(x[1, :]) <= 8*60)
mod3.addConstr(tempiC@(x[2, :]) <= 5*60)

#altri vincoli
mod3.addConstr(x[:, 2].sum() <= 0.2*x.sum())

mod3.addConstr(x[:, 0].sum() >= 0.4*x.sum())

#funzione obiettivo
prezzi = np.array([1000, 1500, 2200])
mod3.setObjective((prezzi*x).sum(), GRB.MAXIMIZE)
mod3.optimize()
if(mod3.Status == GRB.OPTIMAL):
    for i in range(nrobot):
        for j in range(ncategorie):
            print(x[i, j].X, end=' ')
        print()
