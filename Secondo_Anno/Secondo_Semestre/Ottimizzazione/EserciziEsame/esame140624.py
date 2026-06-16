#Esame 14/06/2024
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#8 variabili p_i = quantita di prismil assunta nella fascia oraria i
#8 variabili c_i = quantita di cilindren assunta nella fascia oraria i

mod1 = gp.Model()

nfasce = 8

p = mod1.addMVar(nfasce, lb=0)
c = mod1.addMVar(nfasce, lb=0)

#vincolo proteina minima
minimi = np.array([2.5, 0, 11, 4, 27, 8, 20, 15])
#in ogni fascia oraria si somma anche l'effetto di quelle prese prima
prism = np.array([2.8, 1.9, 0.7, 0.2])
cilind = np.array([3, 2.7, 0.3, 0])

#noto che hanno effetto solo quelli presi 5, 6 e 7 fasce dopo
#tipo se prendo a mezzanotte (prima fascia)
#le uniche altre fasce che hanno ancora effetto sono quella delle 15, delle 18 e delle 21 (del giorno prima)

#devo ricordarmi però che l'effetto dei farmaci cambia col tempo, però uso il fatto che 
#la fascia oraria i è quella in cui prendo il farmaco quindi nella lista degli effetti sara sempre il primo elemento
#la fascia i+7 sarebbe quella prima quindi sarà sempre il secondo elemento (come se lo avessi preso una fascia prima)
#la fascia i+6 il terzo
#la fascia i+5 il quarto
for i in range(nfasce):
    effettoPrism = p[i]*prism[0] + p[(i+5)%8]*prism[3] + p[(i+6)%8]*prism[2] + p[(i+7)%8]*prism[1]
    effettoCilind = c[i]*cilind[0] + c[(i+5)%8]*cilind[3] + c[(i+6)%8]*cilind[2] + c[(i+7)%8]*cilind[1]
    mod1.addConstr(effettoPrism + effettoCilind >= minimi[i])
    #vincolo anche sul massimo consentito
    mod1.addConstr(effettoPrism + effettoCilind <= 45)

mod1.setObjective(p.sum() + c.sum(), GRB.MINIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nfasce):
        print(p[i].X, end=' ')
    print()
    for i in range(nfasce):
        print(c[i].X, end=' ')
    print()

#punto b, minimizzare il costo
mod1.setObjective(0.7*p.sum() + 0.95*c.sum(), GRB.MINIMIZE)
mod1.update()
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nfasce):
        print(p[i].X, end=' ')
    print()
    for i in range(nfasce):
        print(c[i].X, end=' ')
    print()
#Esercizio 4
#3 variabili x_i = ore di allenamento di tipo i
#con i che può essere resistenza, forza, abilità

mod2 = gp.Model()

nattivita = 3
x = mod2.addMVar(nattivita, lb=0)

#vincoli
mod2.addConstr(x.sum() <= 20)

mod2.addConstr(x[0] >= 4)

mod2.addConstr(x[1] >= 0.5*x[2])

mod2.addConstr(x[1] <= 8)

mod2.addConstr(x[0] + x[2] <= 18)

#funzione obiettivo
formaFisica = np.array([5, 4, 6])
mod2.setObjective(formaFisica@x, GRB.MAXIMIZE)

mod2.optimize()
somma = 0
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nattivita):
        print(x[i].X, end=' ')
        somma += x[i].X
    print()
vecchioValFormaFisica = mod2.ObjVal

#secondo punto dell'es 4

#voglio minimizzare x[1] ore di allenamento di forza mantenendo lo stesso numero di ore settimanali ottime
mod2.addConstr(x.sum() == somma)
mod2.setObjective(x[1], GRB.MINIMIZE)
mod2.update()
mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nattivita):
        print(x[i].X, end=' ')
    print()

# #voglio la perdita in forma fisica
nuovoValFormaFisica = formaFisica@x.X
print(vecchioValFormaFisica - nuovoValFormaFisica)