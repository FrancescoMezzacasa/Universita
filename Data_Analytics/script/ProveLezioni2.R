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
