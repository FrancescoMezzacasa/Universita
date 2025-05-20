data("iris")
#sto vedendo sepal.length condizionata alle categorie della specie

devianzaGen <- var(iris$Sepal.Length) * (length(iris$Sepal.Length) - 1)

dev1 <- var(iris$Sepal.Length[iris$Species == "setosa"]) * (length(iris$Sepal.Length[iris$Species == "setosa"]) - 1)
dev2 <- var(iris$Sepal.Length[iris$Species == "virginica"]) * (length(iris$Sepal.Length[iris$Species == "virginica"]) - 1)
dev3 <- var(iris$Sepal.Length[iris$Species == "versicolor"]) * (length(iris$Sepal.Length[iris$Species == "versicolor"]) - 1)

boxplot(iris$Sepal.Length ~ iris$Species)

media1 <- mean(iris$Sepal.Length[iris$Species == "setosa"])
media2 <- mean(iris$Sepal.Length[iris$Species == "virginica"])
media3 <- mean(iris$Sepal.Length[iris$Species == "versicolor"])

medie_gruppi <- c(media1, media2, media3)

dev_medie <- var(medie_gruppi) * 2

dev_medie + mean(dev1, dev2, dev3)


library(MASS)
data("whiteside")

#voglio scoprire se quanto gas consumo dipende dalla temperatura esterna
plot(whiteside$Gas, whiteside$Temp)
plot(whiteside$Temp, whiteside$Gas)

plot(whiteside$Temp, whiteside$Gas)
#vedo che ha covarianza negativa

codevianza <- sum((whiteside$Temp-mean(whiteside$Temp))*(whiteside$Gas - mean(whiteside$Gas)))

n <- length(whiteside$Temp)
covarianzaN <- codevianza/n
#uso la funzione di R
cov(whiteside$Temp, whiteside$Gas)
#vedo che è diversa perche divide per n-1
covarianzaMenoUno <- codevianza/(n - 1)
covarianzaMenoUno == cov(whiteside$Temp, whiteside$Gas)

#calcolo il coeff. di correlazione lineare

coeff <- covarianzaMenoUno/sqrt(var(whiteside$Temp)*var(whiteside$Gas))
cor(whiteside$Gas, whiteside$Temp)


#trovo i coeff della regressione
beta1 <- covarianzaMenoUno/var(whiteside$Temp)
beta0 <- mean(whiteside$Gas)-beta1*mean(whiteside$Temp)
#ora traccio la retta con questi coeff
abline(beta0, beta1, col=2)

#voglio prevedere quanto sara il consumo di gas se ad esempio ci saranno 6 gradi
beta0 + beta1*6


#funzione lm
regresslin <- lm(whiteside$Gas ~ whiteside$Temp)

#qua dentro trovo sia i coefficienti che i residui (le distanze dalla retta)
regresslin$coefficients
regresslin$residuals

summary(regresslin)
