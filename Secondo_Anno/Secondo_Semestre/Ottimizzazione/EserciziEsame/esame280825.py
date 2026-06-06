#Esame 28/08/2025
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#Esercizio 3

#4 variabili p_i = quanti prodotti i ho fatto
#3 variabili m_j = quanti kg di materia j ho comprato
#1 variabile m2ill = quanti kg di materia M2 ho preso dal serbatoio illimitato

mod1 = gp.Model()

nprod = 4
nmat = 3

p = mod1.addMVar(nprod, lb=0)#lascio i prodotti una variabile continua perche sembra che si vendano al kg
m = mod1.addMVar(nmat, lb=0, ub=[3000, 2000, 5000])
m2ill = mod1.addVar(lb=0, ub=GRB.INFINITY)

#vincolo sull'uso dei prodotti acquistati
usom1 = np.array([2, 6, 7, 9])#quanto m1 richiedono i 4 prodotti
usom2 = np.array([10, 20, 2, 7])
usom3 = np.array([4, 3, 20, 10])

mod1.addConstr(p@usom1 <= m[0])#quanti m1 mi servono * quanti ho fatto di ogni prodotto <= quanti m1 ho comprato
mod1.addConstr(p@usom2 <= m[1] + m2ill)#nel caso di m2 posso comprare anche dal serbatoio illimitato
mod1.addConstr(p@usom3 <= m[2])

#vincolo sulle ore
ore = np.array([10, 12, 20, 18])
mod1.addConstr(ore[1]*p[1] <= 0.3*(ore@p))

#funzione obiettivo
#guadagni - spese
#i guadagni sono formati dal prezzo di vendita per quanti prodotti ho fatto
prezziVendita = np.array([20000, 25000, 40000, 35000])
guadagno = prezziVendita@p

#le spese invece sono formate da: quanto prodotto ho comprato (sia standard che da quello illimitato)
# + quante ore ho lavorato

prezziAcquisto = np.array([100, 150, 200])
prezzoIll = 300#quanto costa quello illimitato
spesaAcquisto = prezziAcquisto@m + m2ill*300

costoOrario = 1000
oreRichieste = np.array([10, 12, 20, 18])
spesaOre = (oreRichieste@p)*1000

mod1.setObjective(guadagno - (spesaAcquisto + spesaOre), GRB.MAXIMIZE)
mod1.optimize()
if(mod1.Status == GRB.OPTIMAL):
    for i in range(nprod):
        print(p[i].X, end=' ')
    print()
    for j in range(nmat):
        print(m[j].X, end=' ')
    print()
    print(m2ill.X)

#Esercizio 4

#3*3 = 9 variabili x_ij = tonnellate portate dal quartiere i alla destinazione j

mod2 = gp.Model()

nquart = 3
ndest = 3
x = mod2.addMVar(shape=(nquart, ndest), lb=0)

#vincolo sulla quantità iniziale di rifiuti
#vanno trattati tutti i rifiuti, quindi per ogni quartiere la quantita portata in tutte le dest. deve fare quanta ne avevo all inizio
quantitaIniziale = np.array([40, 30, 50])
for i in range(nquart):
    mod2.addConstr(gp.quicksum(x[i, j] for j in range(ndest)) == quantitaIniziale[i])

#vincoli capacità massima
capacita = np.array([60, 70, 100])
for j in range(ndest):
    mod2.addConstr(gp.quicksum(x[i, j] for i in range(nquart)) <= capacita[j])#qua invece sommo per tutti i quartieri

#vincolo sulle emissioni
emiss = np.array([[0.2, 0.3, 0.4], [0.25, 0.2, 0.35], [0.3, 0.25, 0.3]])

mod2.addConstr((emiss*x).sum() <= 60)

#vincolo logico-strategico
mod2.addConstr(x[2, 1] >= 0.2*quantitaIniziale[2])

#funzione obiettivo
matrCostiTrasporto = np.array([[4, 6, 8], [5, 3, 6], [7, 4, 5]])
matrCostiTrattamento = np.array([1, 1.5, 2])#per ogni destinazione

costiTrasp = (x*matrCostiTrasporto).sum()
costiTratt = (x*matrCostiTrattamento).sum()

mod2.setObjective(costiTrasp + costiTratt, GRB.MINIMIZE)
mod2.optimize()
if(mod2.Status == GRB.OPTIMAL):
    for i in range(nquart):
        for j in range(ndest):
            print(x[i, j].X, end=' ')
        print()


