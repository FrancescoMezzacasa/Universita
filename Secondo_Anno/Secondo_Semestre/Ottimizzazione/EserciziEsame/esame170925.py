#Esame 17/09/2025
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3
#Le variabili saranno i metri quadri messi per ogni pianta, quindi 5 variabili
mod1 = gp.Model()

npiante = 5
x = mod1.addMVar(shape=npiante, lb=0, ub=[30, 40, 60, 80, 20])#gli upper bound sono le quantità massime acquistabili

#vincolo di superficie
mod1.addConstr(x.sum() <= 100)

#vincolo idrico
consumi = np.array([2.5, 1.8, 1, 0.6, 1.2])
mod1.addConstr(consumi@x <= 200)

#vincolo disponibilità vivaio gia messo come upper bound

#vincolo copertura impollinatori
mod1.addConstr(x[1] + x[2] + x[3] >= 0.2*100)

#vincolo bassa manutenzione
mod1.addConstr(x[4] >= 0.05*100)

#dovrei avere due funzioni obiettivo, minimizzare i costi e massimizzare la soddisfazione
#mi viene fornita però una formula per trasformare il gradimento in soldi guadagnati
#quindi uso questo per minimizzare i costi ma togliendoci i soldi guadagnati 

costi = np.array([5, 4, 3, 2, 6])
sodd = np.array([9, 7, 6, 4, 5])

spesa = costi@x
guadagno = (sodd@x)*1.5
mod1.setObjective(spesa - guadagno, GRB.MINIMIZE)

mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(npiante):
        print(x[i].X)

#Esercizio 4
#20 variabili x_ij, il treno i parte nella finestra j (con finestra 0 :00, finestra 1 :15, ecc.)

mod2 = gp.Model()

ntreni = 4
nfinestre = 5

x = mod2.addMVar(shape=(ntreni, nfinestre), lb=0, ub=1, vtype=GRB.INTEGER)#variabili binarie

#vincolo unicità di partenza
for i in range(ntreni):
    mod2.addConstr(gp.quicksum(x[i, j] for j in range(nfinestre)) == 1)#sommo per i bloccato e per tutti i j e deve fare 1
    #e lo faccio per ogni treno

#vincolo capacità della stazione
for j in range(nfinestre):#stavolta scorro sulle finestre temporali
    mod2.addConstr(gp.quicksum(x[i, j] for i in range(ntreni)) <= 2)#massimo 2 treni per finestra ma anche meno

#vincolo di sicurezza
#controllo che se sono in indici "vicini" la somma delle variabili deve essere al massimo 1
for j in range(nfinestre):
    for k in range(nfinestre):
        if(np.abs(j - k) <= 1):
            mod2.addConstr(x[0, j] + x[1, k] <= 1)
            #se stai guardando indici che mi rappresenterebbero una differenza minore di 30
            #allora solo una può essere attiva al massimo



#vincolo di connessione
#moltiplico le variabili binarie per il ritardo che rappresentano, cosi mi rimane l'effettivo ritardo
coeffRitardo = np.array([0, 15, 30, 45, 60])
tabellaRitardi = coeffRitardo*x #moltiplicazione elemento per elemento (viene fatto in automatico il "broadcast" della lista corta)

mod2.addConstr(gp.quicksum(tabellaRitardi[3, j] for j in range(nfinestre)) - gp.quicksum(tabellaRitardi[2, j] for j in range(nfinestre)) >= 0)
#la differenza di ritardo tra il treno 4 e il treno 3 deve essere >= 0, cosi vuol dire che il 4 parte o uguale o dopo al 3
#NB potevo anche fare tabellaRitardi[3, :].sum() cosi mi prende tutta la riga e la somma

#vincolo di equilibrio
#chiede che la differenza sia <= 1 ma devo fare il valore assoluto quindi divido in <= 1 e >= -1
mod2.addConstr(x[:, :3].sum() - x[:, 3:].sum() <= 1)
mod2.addConstr(x[:, 3:].sum() - x[:, :3].sum() <= 1)

#funzione obiettivo

mod2.setObjective(tabellaRitardi.sum(), GRB.MINIMIZE)


mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(ntreni):
        for j in range(nfinestre):
            print(x[i, j].X, end=' ')
        print()


