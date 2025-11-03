library(MASS)
data("geyser")

boxplot(geyser$waiting)

hist(geyser$waiting)


#vediamo il metodo di lisciamento col nucleo

punti <- c(148, 155, 162, 168, 172, 174, 175, 178, 186)

plot(punti, rep(0, 9), ylim=c(0, 0.065), xlim=c(135, 200), pch=4, cex=1.5, main = "esempio")

abline(0, 0)

n <- length(punti)
for(i in 1:n){
  curve(dnorm(x, punti[i], 4)/n, col=(i+1), lwd=3, lty=2, add=T)
}

#funzione che calcola la somma delle altezze di tutte le gaussiane in ogni punto

nucleo <- function(x, punti, h=1){
  nucl <- 0
  nn <- length(x)
  n = length(punti)
  for(i in 1:nn){
    nucl[i] = sum(dnorm(x[i], punti, h)/n)
  }
  nucl
}
xr <- seq(130, 200, 0.5)
lines(xr, nucleo(xr, punti, 8), lwd=2.5)

den <- density(geyser$waiting)
plot(den)

nuovo <- ksmooth(den$x, den$y, bandwidth = 4)
plot(nuovo)


library(insuranceData)
data("AutoBi")

summary(AutoBi)

mean(AutoBi$LOSS)

mean(AutoBi$CLMAGE)
mean(AutoBi$CLMAGE, na.rm = T)

table(AutoBi$MARITAL)
table(AutoBi$MARITAL, useNA = "ifany")
