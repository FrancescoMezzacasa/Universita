#Esame 30/01/2026
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#uso 3*4 = 12 variabili x_ij = numero di studenti del dipartimento i che vanno nella citta j
#inoltre però devo tenere conto dei posti aggiuntivi
#potrei usare una matrice di posti aggiuntivi ma dovrei avere altre 12 variabili quando in realta so che ne ho solo 3
#quindi creo 3 singole variabili u_k = numero di studenti che hanno utilizzato il posto aggiuntivo k-esimo
#con posto aggiuntivo 0 il primo citato nel testo, 1 il secondo e 2 il terzo

mod1 = gp.Model()

ndip = 3
nsedi = 4
x = mod1.addMVar(shape=(ndip, nsedi), lb=0, vtype=GRB.INTEGER)

npostiagg = 3
u = mod1.addMVar(npostiagg, lb=0, ub=[5, 4, 3], vtype=GRB.INTEGER)#metto gia il numero massimo di posti aggiuntivi

#vincolo sul numero massimo totale
mod1.addConstr(x.sum() + u.sum() <= 130)#anche minore perche non tutti devono per forza andare

#vincolo sul numero di studenti del dipartimento
dimensioneDip = np.array([50, 45, 35])
for i in range(ndip):
    mod1.addConstr(x[i, :].sum() + u[i] <= dimensioneDip[i])#questo perche u[0] si riferisce proprio al primo dipartimento

#vincolo sulla capacita massima delle sedi
capSedi = np.array([40, 30, 25, 35])
for j in range(nsedi):
    mod1.addConstr(x[:, j].sum() <= capSedi[j])#non sommo i posti aggiuntivi perche non contano nella capacita massima

#vincolo equità
for j in range(nsedi):
    #la sede 1 non ha posti aggiuntivi, quindi controllo solo la somma di quelli normali
    if(j == 0):
        mod1.addConstr(x[:, 0].sum() <= 0.3*(x.sum() + u.sum()))
    else:
        #tutte le altre sedi ne hanno quindi i vincoli sono uguali
        #NB u[0] si riferisce al dipartimento 0 ma alla sede 1!!!
        #quindi metto j - 1 cosi si riferisce al dipartimento giusto, tipo quando j = 1 prendo u[0] che si riferisce appunto alla sede 1
        mod1.addConstr(x[:, j].sum() + u[j - 1] <= 0.3*(x.sum() + u.sum()))

#vincolo nessuna sede senza studenti
for j in range(nsedi):
    #stessa logica usata sopra
    if(j == 0):
        mod1.addConstr(x[:, 0].sum() >= 1)
    else:
        mod1.addConstr(x[:, j].sum() + u[j - 1] >= 1)

#vincolo mete prestigiose
#anche qua vale il fatto che i posti aggiuntivi sono shiftati rispetto alle sedi
mod1.addConstr((x[:, 1].sum() + u[0]) + (x[:, 2].sum() + u[1]) >= 0.55*(x.sum() + u.sum()))

#vincoli dipartimentali
#mentre invece i posti aggiuntivi sono allineati con i dipartimenti
minDipartimenti = np.array([15, 10, 8])
for i in range(ndip):
    mod1.addConstr(x[i, :].sum() + u[i] >= minDipartimenti[i])

#funzione obiettivo
coeffSoddisfazione = np.array([8, 7, 9, 6])

#calcolo separatamente la soddisfazione dei posti normali (non aggiuntivi)
soddPostiNormali = (coeffSoddisfazione * x).sum()#cosi moltiplica ogni valore di x per la sodd del suo dipartimento

soddPostiAgg = gp.quicksum(u[k]*coeffSoddisfazione[k + 1] for k in range(npostiagg))

mod1.setObjective(soddPostiNormali + soddPostiAgg, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(ndip):
        for j in range(nsedi):
            print(x[i, j].X, end=' ')
        print()
    print()
    for k in range(npostiagg):
        print(u[k].X, end=' ')
    print()

#Esercizio 4
#5 variabili una per ogni farmaco, mi dice quanto ne produco

mod2 = gp.Model()

nfarm = 5
nPrAtt = 3

x = mod2.addMVar(nfarm, lb=[20, 30, 10, 5, 0])#lower bound sono il minimo per l'ordine esterno

#vincoli disponibilità principi attivi
richiestaPrAtt = np.array([[2, 1, 3, 2, 1], [1, 2, 1, 3, 0], [0, 1, 1, 2, 2]])
disp = np.array([150, 130, 90])

for i in range(nPrAtt):
    mod2.addConstr(x@richiestaPrAtt[i] <= disp[i])

#vincolo sullo spazio

richOrdineEsterno = np.array([20, 30, 10, 5, 0])
spazio = np.array([3, 2, 4, 5, 2])
mod2.addConstr((x-richOrdineEsterno)@spazio <= 220)

#vincolo limiti produttivi
mod2.addConstr(x[3] <= 35)

#vincoli sbilanciamento produzione
mod2.addConstr(x[0] <= 2*x[1])
mod2.addConstr(x[3]+x[4] <= x[2])

#vincolo ordine esterno
mod2.addConstr(20 + 30 + 10 + 5 >= 0.35*(x.sum()))

#funzione obiettio
profitto = np.array([8, 6, 10, 14, 5])
mod2.setObjective(x@profitto, GRB.MAXIMIZE)
mod2.optimize()
if(mod2.status == GRB.OPTIMAL):
    for i in range(nfarm):
        print(x[i].X)