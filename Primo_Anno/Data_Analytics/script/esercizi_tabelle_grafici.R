##A1. Carica il file hotdog.RData ----------------------------------------------
load("hotdog.RData")

##A2. Descrivi in dettaglio l'oggetto hotdog. 
## Che tipo di oggetto è? Quante unità contiene?
## Quante variabili? Che tipo di variabili? ------------------------------------
str(hotdog) #è un data frame e contiene 54 unità/osservazioni
#contiene 4 variabili di tipo int factor int int

##A3. Calcola la distribuzione di frequenza assoluta, 
## relativa e percentuale della variabile Tipo. 
## Stampa i risultati in tre colonne. ------------------------------------------
table(hotdog$Type)#per le assolute
round(prop.table(table(hotdog$Type)), 2)#relative
round(prop.table(table(hotdog$Type)), 3)*100

a <- cbind(table(hotdog$Type),
      round(prop.table(table(hotdog$Type)), 2),
      round(prop.table(table(hotdog$Type)), 3)*100)
colnames(a) <- c("Assolute", "Relative", "Percentuali")
##A4. Qual è la rappresentazione grafica più appropriata per la variabile
## Tipo? ----------------------------------------------------------

#è qualitativa
pie(table(hotdog$Type))
barplot(table(hotdog$Type))

##A5. Rappresenta graficamente la ditribuzione di frequenza per la variabile  
## calorie in classi. ----------------------------------------------------------

#è quantitativa
hist(hotdog$Calorie)
boxplot(hotdog$Calorie)

##A6. Calcola la distribuzione di frequenza per la variabile Calorie 
## considerando le seguenti classi:  [80,120] (120,140] (140,160] (160,200] ----
hotdog$CalorieClass <- cut(hotdog$Calorie, breaks = c(80, 120, 140, 160, 200), include.lowest = T)
table(hotdog$CalorieClass)
##A7. Disegna un nuovo istogramma considerando le classi precedenti. -----------
hist(hotdog$Calorie, breaks = c(80, 120, 140, 160, 200))

##A8. Calcola le densità plottate nel punto precedente. ------------------------
wid <- diff(c(80, 120, 140, 160, 200))

prop.table(table(hotdog$CalorieClass))/wid

abline(h = prop.table(table(hotdog$CalorieClass))/wid)
##A9. Disegna e confronta gli istogrammi della variabile Calorie secondo le 
## modalità della variabile Tipo e commenta. -----------------------------------

par(mfrow = c(3, 1))
hist(hotdog$Calorie[hotdog$Type == "Beef"], main = "Beef", xlim = c(80, 200))
hist(hotdog$Calorie[hotdog$Type == "Meat"], main = "Meat", xlim = c(80, 200))
hist(hotdog$Calorie[hotdog$Type == "Poultry"], main = "Poultry", xlim = c(80, 200))

##A10. Calcolare le medie della variabile Calorie per ogni modalità della 
## variabile Tipo ed aggiungerle ai grafici ottenuti al punto precedente. ------

b <- tapply(hotdog$Calorie, hotdog$Type, mean)
par(mfrow = c(3, 1))

hist(hotdog$Calorie[hotdog$Type == "Beef"], main = "Beef", xlim = c(80, 200))
abline(v = )
