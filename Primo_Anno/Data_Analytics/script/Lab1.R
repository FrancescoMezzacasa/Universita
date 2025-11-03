#Laboratorio 1
#Punto 1
rapp_fibo <- function(vettore){
  ris <- c()
  for (i in 3:length(vettore)){
    print(vettore[i])
    ris <- c(ris, (vettore[i]/vettore[i-1]))
  }
  return(ris)
}

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
  print(rapp_fibo(ris))
  return(ris)
}

#Punto 2

x <- runif(20, min = 1, max = 20)
media = mean(x)

x_tilde <- c()
for (i in x){
  if(x[i] <= media){
    x_tilde <- c(x_tilde, media)
  } else{
    x_tilde <- c(x_tilde, x[i])
  }
}

#Punto 3
funzione <- function(x){
  if(x < 0){
    return((x*x) - 1)
  }else{
    return((x*x*x) - 1)
  }
}


#Punto 5
data(iris)

iris2 <- iris[sample(1:nrow(iris), 15), ]

iris2$Sepal.Length <- cut(iris2$Sepal.Length, breaks = 5)

table(iris2$Species)
table(iris2$Sepal.Length)

#rifaccio con il dataframe originiale
iris$Sepal.Length <- cut(iris$Sepal.Length, breaks = 5)

table(iris$Species)
table(iris$Sepal.Length)

iris3 <- iris[iris$Species == "setosa", ]
