#Esame 20/02/2026

#Esercizio 3 chiedeva solo di formularlo quindi fatto su carta

#Esercizio 4
import gurobipy as gp
from gurobipy import GRB
import numpy as np

#8 variabili ognuna dice quanti soldi andati al progetto i-esimo
mod1 = gp.Model()

nprog = 8
x = mod1.addMVar(nprog, lb=0, ub=[3, 2.5, 2, 2, 1.5, 2, 1.5, 1.5])#metto gia come upper bound il finanziamento massimo

#vincoli
mod1.addConstr(x.sum() <= 10)

for i in range(nprog):
    mod1.addConstr(x[i] <= 0.3*x.sum())

xstem = x[0]+x[1]+x[2]
xsoc = x[3]+x[4]
xgreen = x[5]+x[6]+x[7]

mod1.addConstr(xgreen >= 0.5*x.sum())

mod1.addConstr(xstem >= 0.8*xsoc)

#se il green deve prendere almeno 2 milioni gli altri devono prenderne al massimo 8
mod1.addConstr(xstem + xsoc <= 8)

mod1.addConstr(xstem <= 0.4*x.sum())

aree = [xstem, xsoc, xgreen]
#se voglio dire che tra l'area piu e quella meno finanziata deve essere meno di 2 mln se lo faccio per tutti va bene
for i in aree:
    for j in aree:
        mod1.addConstr(i - j <= 2)

rendimento = np.array([9, 8, 7, 6, 5, 8, 7, 6])
mod1.setObjective(x@rendimento, GRB.MAXIMIZE)
mod1.optimize()
if(mod1.status == GRB.OPTIMAL):
    for i in range(nprog):
        print(x[i].X)



#in realta il modo giusto per farlo era dividendo i budget
mod1 = gp.Model()

nprog = 8
# --- LA VIA ACCADEMICA ---
# Sdoppio le variabili (tolgo l'ub da qui perché lo rimetto come vincolo dopo)
xL = mod1.addMVar(nprog, lb=0, name="Fondi_Liberi")
xV = mod1.addMVar(nprog, lb=0, name="Fondi_Vincolati_Green")

# Ricreo la tua x come somma delle due (Gurobi la leggerà come un'espressione lineare)
x = xL + xV

# Rimetto l'upper bound che avevi impostato
ub_max = np.array([3, 2.5, 2, 2, 1.5, 2, 1.5, 1.5])
mod1.addConstr(x <= ub_max, name="Finanziamento_Massimo")

# Imposto i limiti dei due "portafogli" separati
mod1.addConstr(xL.sum() <= 8, name="Max_Fondo_Libero")
mod1.addConstr(xV.sum() <= 2, name="Max_Fondo_Vincolato")

# I progetti da 0 a 4 (STEM e SOC) NON possono toccare i fondi vincolati
mod1.addConstr(xV[:5] == 0, name="No_Fondi_Vincolati_Per_Stem_Soc")
# -------------------------

# Da qui in poi, il TUO codice originale non cambia di una virgola!
mod1.addConstr(x.sum() <= 10)

for i in range(nprog):
    mod1.addConstr(x[i] <= 0.3*x.sum())

xstem = x[0]+x[1]+x[2]
xsoc = x[3]+x[4]
xgreen = x[5]+x[6]+x[7]

mod1.addConstr(xgreen >= 0.5*x.sum())
mod1.addConstr(xstem >= 0.8*xsoc)
mod1.addConstr(xstem <= 0.4*x.sum())

aree = [xstem, xsoc, xgreen]
for i in aree:
    for j in aree:
        mod1.addConstr(i - j <= 2)

rendimento = np.array([9, 8, 7, 6, 5, 8, 7, 6])
mod1.setObjective(x@rendimento, GRB.MAXIMIZE)

mod1.optimize()
if(mod1.status == GRB.OPTIMAL):
    for i in range(nprog):
        print(f"Progetto {i+1}: {xL[i].X + xV[i].X} mln (di cui {xV[i].X} dal fondo GREEN)")