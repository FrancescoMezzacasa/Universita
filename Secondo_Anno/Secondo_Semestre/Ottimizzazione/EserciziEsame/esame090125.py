#Esame 09/01/2025
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#4 variabili x_i = tonnellate passate per il percorso i-esimo

mod1 = gp.Model()

nperc = 4
x = mod1.addMVar(nperc, lb=0, ub=[1500, 2000, 1800, 2500])#metto gia il loro massimo come upper bound

#vincolo quantita minima
mod1.addConstr(x.sum() >= 4000)

#vincolo emissioni massime
coeffEmiss = np.array([0.5, 0.8, 0.3, 0.6])
mod1.addConstr(coeffEmiss@x <= 2200)

#altri vincoli
mod1.addConstr(x[2] + x[3] >= 0.6*x.sum())

mod1.addConstr(x[1] >= 2*x[0])

mod1.addConstr(x[2] <= (x[1] + x[3])/2)

mod1.addConstr(x[0] + x[2] >= 0.4*x.sum())

#funzione obiettivo
coeffCostoEc = np.array([3, 5, 2, 4])

costoAmb = coeffEmiss@x
costoEc = coeffCostoEc@x

mod1.setObjective(costoAmb + costoEc, GRB.MINIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nperc):
        print(x[i].X, end=' ')
    print()

#Esercizio 4
#3 variabili x_i = quante unita di prodotto i produco in una settimana

mod2 = gp.Model()

nprod = 3
x = mod2.addMVar(nprod, lb=[50, 30, 40], ub=GRB.INFINITY, vtype=GRB.INTEGER)#metto la domanda minima come lower bound

#vincoli sulle ore per ogni step della produzione

oreAssemblaggio = np.array([1.5, 3, 2])
oreTest = np.array([1, 2, 1.5])
oreImballaggio = np.array([0.5, 1, 0.5])

mod2.addConstr(oreAssemblaggio@x <= 400)
mod2.addConstr(oreTest@x <= 300)
mod2.addConstr(oreImballaggio@x <= 150)

#vincolo componenti premium
compPremiumNecessari = np.array([2, 5, 3])
mod2.addConstr(compPremiumNecessari@x <= 800)

#vincoli aggiuntivi
mod2.addConstr(x[1] + x[2] >= 0.5*x.sum())

mod2.addConstr(x.sum() <= 200)

mod2.addConstr(x[2] >= 0.6*x[1])

mod2.addConstr((oreAssemblaggio[0] + oreTest[0])*x[0] <= 0.6*(400+300))

mod2.addConstr(oreImballaggio@x >= 50)

mod2.addConstr(compPremiumNecessari[1]*x[1] >= 0.3*(compPremiumNecessari@x))#almeno il 30% di quelli usati non di tutti i possibili

mod2.addConstr(x[0] <= 2*(x[1] + x[2]))

mod2.addConstr(oreAssemblaggio@x + oreTest@x >= 0.9*(400+300))

#vincolo sui profitti
profittiUnitari = np.array([250, 700, 450])
mod2.addConstr(profittiUnitari[0]*x[0] <= 0.5*(profittiUnitari@x))

#funzione obiettivo
mod2.setObjective(profittiUnitari@x, GRB.MAXIMIZE)

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nprod):
        print(x[i].X, end=' ')
    print()
