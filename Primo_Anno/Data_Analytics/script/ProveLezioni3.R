america <- read.csv("cancerdata.csv")
daticanc <- america[, c(3, 4, 5, 7, 8)]
cor(daticanc)
pairs(daticanc)

#voglio analizzare arate e meat ma senza la dipendenza da GDP
#guardo i resitui

mod1 <- lm(arate ~ GDP, data = daticanc)
resarate <- mod1$residuals
mod2 <- lm(meat ~ GDP, data = daticanc)
resmeat <- mod2$residuals

#ora che ho tolto la dipendenza calcolo la correlaizone
cor(resarate, resmeat)
#pochissima correlazione quindi il problema era il GDP

#lo posso vedere anche con il p-value
modinsieme <- lm(arate ~ meat + GDP, data = daticanc)
summary(modinsieme)

#il p-value (scritto come Pr(>|t|)) per meat è altissimo quindi non va bene (non c'è correlazione tra arate e mean)
