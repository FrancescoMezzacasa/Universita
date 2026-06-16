#Esame 29/08/2024
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#5 variabili x_i = percentuale del capitale investita nella classe i
#0 azioni 
#1 obbligazioni
#2 immobili
#3 materie prime
#4 titoli del mercato monetario

mod1 = gp.Model()

nclassi = 5
x = mod1.addMVar(nclassi, lb=0, ub=100)#sono percentuali quindi massimo 100

#vincolo partizionamento del capitale totale
mod1.addConstr(x.sum() == 100)

#vincoli vari
mod1.addConstr(x[4] >= 0.25*x.sum())

for i in range(nclassi):
    mod1.addConstr(x[i] <= 0.35*x.sum())#nessuno degli investimenti può superare il 35% del totale
    #quindi metto il vincolo per tutti


mod1.addConstr(x[1] + x[3] >= 0.4*x.sum())

stDevRendimenti = np.array([12, 23, 31, 9, 18])

mod1.addConstr((stDevRendimenti@x)/100 <= 21)
#dato che ho messo le percentuali moltiplicate per 100 poi devo dividere cosi tornano numeri decimali

#funzione obiettivo
rendimenti = np.array([1.2, 1.5, 1.8, 1.05, 1.13])
mod1.setObjective(rendimenti@x, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nclassi):
        print(x[i].X, end=' ')
    print()

#Esercizio 4
#totale 15 variabili
#5 variabili x_i = quante ho prodotto nel mese i
#5 variabili y_i = quante ho prodotto in più nel mese i (con la produzione "straordinaria")
#5 variaibli z_i = quante sono rimaste invendute nel mese i

mod2 = gp.Model()

nmesi = 5

x = mod2.addMVar(nmesi, lb=0, ub=110, vtype=GRB.INTEGER)#capacita produttiva 110 pezzi/mese
y = mod2.addMVar(nmesi, lb=0, ub=60, vtype=GRB.INTEGER)#massimo ulteriori 60 pezzi/mese
z = mod2.addMVar(nmesi, lb=0, vtype=GRB.INTEGER)


#vincolo domanda
domanda = np.array([100, 130, 150, 140, 180])

#quelli invenduti di questo mese sono quelli prodotto meno la domanda (vendo solo la domanda)
for i in range(nmesi):
    if(i == 0):#se sono al primo mese non ho roba invenduta dai mesi prima da vendere
        mod2.addConstr(x[i] + y[i] - domanda[i] == z[i])
    else:
        mod2.addConstr(x[i] + y[i] + z[i-1] - domanda[i] == z[i])
        #negli altri mesi posso vendere anche quello che avevo nel magazzino prima oltre a quello prodotto

#io avevo fatto con maggiore uguale e senza questo vincolo
for i in range(nmesi):
    mod2.addConstr(x[i] + y[i] + z[i-1] >= domanda[i])#in realta questo è ridondante ma il prof l ha messo
#dice che per ogni mese quanto produco e quanto ho rimasto dal mese prima deve essere abbastanza per la domadna

#funzione obiettivo
costox = 300
costoy = 330
costoz = 10

mod2.setObjective(costox*x.sum() + costoy*y.sum() + costoz*z.sum(), GRB.MINIMIZE)

mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nmesi):
        print(x[i].X, end=' ')
    print()
    for i in range(nmesi):
        print(y[i].X, end=' ')
    print()
    for i in range(nmesi):
        print(z[i].X, end=' ')
    print()