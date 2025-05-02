#titolo: "Primo Homework"
#autore: "Francesco Mezzacasa"


#Primo Homework

#sequenza numeri da 20 a 50
seq(20, 50)

#numeri razionali da 20 a 50
razionali <- seq(20, 50, 0.01)

sum <- sum(razionali)
sum
mean <- mean(razionali)
mean



#Funzione per i primi n Fibonacci
fibo <- function(n){
  if(n == 0)
    return(-1)
  if(n == 1)
    return(0)
  
  ris <- c(0, 1)
  primo <- 0
  secondo <- 1
  count <- 2
  while(count < n){
    nuovo <- primo+secondo
    ris <- append(ris, nuovo)
    primo <- secondo
    secondo <- nuovo
    count <- count + 1
  }
  return(ris)
}

#Funzione che include anche somma e media
fibo2 <- function(n){
  if(n == 0)
    return(-1)
  if(n == 1)
    return(0)

  
  ris <- c(0, 1)
  primo <- 0
  secondo <- 1
  count <- 2
  while(count < n){
    nuovo <- primo+secondo
    ris <- append(ris, nuovo)
    primo <- secondo
    secondo <- nuovo
    count <- count + 1
  }
  media <- mean(ris)
  somma <- sum(ris)
  ris <- append(ris, c(somma, media))
  return(ris)
}

#Lista di elementi misti
lista <- list(
  runif(5, min = 1, max = 10),
  matrix(runif(6, min = 0, max = 1), nrow = 2, ncol = 3),
  c("Ligabue", "Eminem", "Elvis")
)
#Ora con nomi
names(lista) <- c("Numeri", "Matrice", "Cantanti")


#carico life.csv
life <- read.csv("Life.csv", header = T)

#noto che nella seconda riga ha un avviso
#quindi elimino la prima

life <- life[-1, ]
#vedo i tipi delle variabili
str(life)
#country_code = char
#country_name = char
#year = int
#value = float


#trasformo i char in stringhe
life$country_code <- factor(life$country_code)
life$country_name <- factor(life$country_name)

#rimuovo i valori non presenti
life <- life[life$value != 9999, ]

table(life$country_name)
table(life$year)

#calcolo la media dell'aspettativa di vita per l'Australia
media_australia <- mean(life$value[life$country_name == "Australia"])

#confronto con media di tutti gli altri stati
life$value[life$country_code != "Australia"]
media_altri <- mean(life$value[life$country_code != "Australia"])

media_altri
media_australia


matrice <- matrix(seq(2, 12, 2), nrow = 2)
is.matrix(matrice) #TRUE
is.array(matrice) #TRUE

b <- matrice[, 3]
is.vector(b)#b è un vector

#trasformo in matrice
b <- as.matrix(b)


#importo il nuovo dataset
nazioni <- read.csv("nazioni.csv", header = T)

dim(nazioni)[1] #105 righe

colnames(nazioni)

#contare il numero di NA per colonna

numero_na <- c()
for (colonna in colnames(nazioni)){
  numero_na <- c(numero_na, sum(is.na(nazioni[colonna])))
}
numero_na

#nome dei paesi che hanno valori mancanti
nazioni[is.na(nazioni$infmort) | is.na(nazioni$oil), "nome"]

#tolgo le righe che erano na
nazioni <- na.omit(nazioni)

#ordino le frequenze percentuali
sort(prop.table(table(nazioni$areaGeo)) *100, decreasing = T)

#creo la nuova colonna factor
nazioni$areaGeofact <- factor(nazioni$areaGeo, levels = c("Africa", "Asia", "Americas", "Europe"), ordered = T)
nazioni$areaGeo <- NULL #ed alimino la scorsa

#stessa cosa con oil
nazioni$oilFactor <- factor(nazioni$oil, labels = c("No", "Yes"))
nazioni$oil <- NULL

#vedo i paesi che esportano petrolio
paesi_esportatori <- cbind(nazioni[nazioni$oilFactor == "Yes", "nome"], nazioni[nazioni$oilFactor == "Yes", "areaGeofact"])
paesi_esportatori

#calcolo medie mortalità infantile per zona geografica
medie_mortalita <- aggregate(nazioni$infmort, by = list(nazioni$areaGeofact), FUN = mean)
medie_mortalita

#trovo gli stati con mortalità maggiore di 300
maggiori <- nazioni$nome[nazioni$infmort > 300]
maggiori

#dico se esportano petrolio o no
for (nome in maggiori){
  print(nome)
  print(nazioni$oilFactor[nazioni$nome == nome])
}

#boxplot mortalità per ogni area geografica
par(mfrow = c(2, 2))
boxplot(nazioni$infmort[nazioni$areaGeofact == "Africa"], ylim = c(0, 650), main = "Africa")
boxplot(nazioni$infmort[nazioni$areaGeofact == "Asia"], ylim = c(0, 650), main = "Asia")
boxplot(nazioni$infmort[nazioni$areaGeofact == "Americas"], ylim = c(0, 650), main = "Americas")
boxplot(nazioni$infmort[nazioni$areaGeofact == "Europe"], ylim = c(0, 650), main = "Europe")

#istogramma con distribuzione del reddito
par(mfrow = c(1, 1))
hist(nazioni$reddito, xlab = "Reddito", main = NULL)

#aggiungo la mediana
abline(v = median(nazioni$reddito[nazioni$areaGeofact == "Africa"]), col = 'red')

abline(v = median(nazioni$reddito[nazioni$areaGeofact == "Asia"]), col = 'blue')

abline(v = median(nazioni$reddito[nazioni$areaGeofact == "Europe"]), col = 'green')

abline(v = median(nazioni$reddito[nazioni$areaGeofact == "Americas"]), col = 'orange')

#divido il reddito in categorie
redditoCat <- cut(nazioni$reddito, breaks = c(0, 500, 1500, 4000, Inf), include.lowest = T)
redditoCat

#vedo quanti paesi ci sono nella categoria 4000+
table(redditoCat)[4]
#vedo in quali aree geografiche sono
barplot(table(nazioni$areaGeofact[nazioni$reddito >= 4000]))

#distribuzioni condizionate
condizionata <- table(redditoCat, nazioni$oilFactor)
condizionata
frequenze <- round(prop.table(condizionata, 2), 2)
frequenze
