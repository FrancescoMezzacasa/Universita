library(MASS)
data("Cars93")
#voglio fare l'istogramma come avevamo fatto per le altezze negli appunti

hist(Cars93$Length)
#in automatico fa tutti gli intervalli della stessa dimensione
#quindi sulla y basta che metta quelle assolute

hist(Cars93$Length, probability = T)
#così sulla y mette la densità

#confronto ora
par(mfrow=c(2, 2))
hist(Cars93$Length, main = "Istogramma con f. assolute")
hist(Cars93$Length, probability = T, main = "Istogramma con densità")
hist(Cars93$Length, probability = T, breaks = 12, main = "Istogramma con 12 intervalli")
hist(Cars93$Length, probability = T, breaks = c(140, 160, 170, 180, 190, 200, 220), main = "Istogramma con intervalli di diversa ampiezza")

mio <- hist(Cars93$Length, probability = T, breaks = c(140, 160, 170, 180, 190, 200, 220), main = "Istogramma con intervalli di diversa ampiezza")
str(mio)

sum(c(0.00215*20, 0.01075*10, 0.03011*10, 0.02688*10, 0.016129*10, 0.0059*20))

len <- c(20, 10, 10, 10, 10, 20)
sum(len*mio$density)#fa 1

data("geyser")
str(geyser)
hist(geyser$waiting, prob = T)
