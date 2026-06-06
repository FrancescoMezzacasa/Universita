#Esame 19/06/2025
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
'''
5 variabili ognuna che dice quante di quelle attivita sono fatte in una settimana

'''
mod1 = gp.Model()

nattivita = 5

x = mod1.addMVar(nattivita, lb=[0]*nattivita)

costi = np.array([100, 150, 200, 300, 50])

#vincolo sui costi
#potevo anche fare con addVars e fare gp.quicksum(costi[i]*x[i] for i in range(nattivita))
mod1.addConstr(costi@x <= 10000)

#vincolo sul carico ambientale
caricoAmb = np.array([2, 3, 1, 0.5, -1])#metto il carico ambientale del volontariato -1 cosi quando sommo quello verrà tolto
mod1.addConstr(caricoAmb@x <= 150)

#vincoli su altre garanzie
mod1.addConstr(x[2]+x[3] >= 0.3*gp.quicksum(x))#le cose culturali almeno il 30% del totale
mod1.addConstr(x[0]+x[1] >= 0.4*gp.quicksum(x))#le cose outdoor almeno il 40% del totale

#vincoli sulle guide
guide = np.array([1, 1, 2, 0, 1])
mod1.addConstr(guide@x <= 60)

#vincoli generali
mod1.addConstr(x[0]+x[1]+x[2]+x[3] <= 80)
mod1.addConstr(x[4] <= 20)

#funzione obiettivo
sostenibilita = np.array([5, 6, 8, 7, 4])
mod1.setObjective(sostenibilita@x, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nattivita):
        print(x[i].X)
    print(mod1.ObjVal)

#Esercizio 4
'''
18 variabili per magazzini ospedali prodotti, ovvero x_ijp dice quante unita di prodotto p vanno dal magazzino i all ospedale j
6 variabili per i camion, una per ogni magazzino per ogni ospedale
9 variabili per quanto prodotto non portato agli ospedali, 9 perche per ogni ospedale per ogni prodotto, 
quindi u_ip dice quante unita di prodotto p sono rimaste invendute per l ospedale i
'''

mod2 = gp.Model()

nmag = 2
nosp = 3
nprod = 3

x = mod2.addMVar(shape=(nmag, nosp, nprod), lb=0, vtype=GRB.INTEGER)
y = mod2.addMVar(shape=(nmag, nosp), lb=0, ub=3, vtype=GRB.INTEGER)
u = mod2.addMVar(shape=(nosp, nprod), lb=0, vtype=GRB.INTEGER)

#vincolo sulla capacita dei magazzini
capacita = np.array([[100, 500, 300], [80, 400, 250]])
for i in range(nmag):
    for p in range(nprod):
        mod2.addConstr(gp.quicksum(x[i, j, p] for j in range(nosp)) <= capacita[i, p])

#vincolo sulla domanda
domande = np.array([[50, 200, 100], [30, 150, 80], [40, 100, 120]])
for j in range(nosp):
    for p in range(nprod):
        mod2.addConstr(gp.quicksum(x[i, j, p] for i in range(nmag)) + u[j, p] == domande[j, p])

#vincolo sul trasporto
for i in range(nmag):
    for j in range(nosp):
        mod2.addConstr(gp.quicksum(x[i, j, p] for p in range(nprod)) <= 120*y[i, j])
    
costiTrasp = np.array([[50, 60, 70], [40, 50, 55]])

penalita = np.array([10, 3, 2])
# costiTrasp * y moltiplica ogni tratta per il suo costo e poi sum() somma tutto
# u * penalita moltiplica automaticamente ogni colonna per 10, 3 e 2 e poi sum() somma tutto
mod2.setObjective((costiTrasp * y).sum() + (u * penalita).sum(), GRB.MINIMIZE)
mod2.optimize()