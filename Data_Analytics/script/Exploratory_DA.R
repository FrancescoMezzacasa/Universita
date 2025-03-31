#### Matrice dei dati e data frame in R  -----------------------------------------

# La struttura del "data frame" ha una rappresentazione coerente con la matrice 
# dei dati. 

library(insuranceData)
data("AutoBi")
str(AutoBi) # esplorare il tipo di variabili 

# Alcune variabili hanno codici numerici, ma sono variabili qualitative: 
# è opportuno ricodificarle
# Ad esempio, la variabile `AutoBi$MARITAL` è in realta una variabile 
# qualitativa (un fattore) con 4 modalità (=1 if married, =2 if single, 
# =3 if widowed, and =4 if divorced/separated)

AutoBi$MARITAL<- factor(AutoBi$MARITAL) # i missing rimangono tali
levels(AutoBi$MARITAL)<-c("married", "single", "widowed", "divorced")
str(AutoBi$MARITAL)

# per unire delle categorie

levels(AutoBi$MARITAL)<-c("married", "single", "previouslymarried", "previouslymarried")
levels(AutoBi$MARITAL)

# A volte è opportuno trasformare un variabile quantitativa in classi: cut()
?cut # restituisce un factor
range(AutoBi$CLMAGE, na.rm=TRUE)
CLMAGEclass<-cut(AutoBi$CLMAGE, breaks=6) #crea 6 classi
str(CLMAGEclass)
levels(CLMAGEclass)

# ma possiamo anche definire classi di ampiezza diversa
CLMAGEclass<-cut(AutoBi$CLMAGE, breaks=c(0,15,24,36,50,95),
                 include.lowest = T)
CLMAGEclass<-cut(AutoBi$CLMAGE, breaks=c(-1,15,24,36,50,95))
str(CLMAGEclass)
levels(CLMAGEclass)

# Tenere a mente che:
# se le classi non coprono tutti i valori numerici del vettore questi 
# saranno definiti come NA.
# si può anche usare come estremo superiore (inferiore) di una classe il
# valore speciale `Inf` o `-Inf`.

### Tabelle di frequenza -------------------------------------------------------

## Tabelle di frequenza semplici -----------------
?table()

AutoBi$ATTORNEY<- factor(AutoBi$ATTORNEY) 
levels(AutoBi$ATTORNEY) = c("yes", "no")
tabella<-table(AutoBi$ATTORNEY)
tabella #distribuzione di frequenza assoluta

str(tabella) #table: di fatto un array con una dimensione

tabella1<-table(AutoBi$MARITAL)
tabella1
tabella1[1] #gestibile come un vettore
tabella1["married"]
dim(tabella1)

# per fare confronti con numerosità diverse è indicato utilizzare le frequenze
# relative: frequenze assolute/numerosità totale

freqrel<-table(AutoBi$MARITAL)/length(AutoBi$MARITAL)
freqrel

# margin.table(): va applicata ad un array 
# (restuisce le distribuzioni marginali) o il totale 

totale<-margin.table(table(AutoBi$MARITAL))
totale
# usiamo questo come divisore 
freqrel<-table(AutoBi$MARITAL)/totale
freqrel


# prop.table() consente di ottenere direttamente la versione
# della tabella con le frequenze relative
prop.table(table(AutoBi$MARITAL))

## Note:
table(AutoBi$CLMAGE) #poco leggibile
table(CLMAGEclass)   #più utile

AutoBi$CLMSEX <- factor(AutoBi$CLMSEX)
str(AutoBi$CLMSEX)
a <- AutoBi$CLMSEX[AutoBi$CLMSEX==1]
table(a) # evidenzia livelli vuoti

str(a) # 
str(droplevels(a))

## Tabelle a doppia entrata -----------------

# tabelle a più entrate o di contingenza: rappresentano la
# distribuzione congiunta di due o più variabili categoriali (fattori).
# ovvero le frequenze (assolute) congiunte: quante unità mostrano 
# congiuntamente una determinata coppia di modalità

AutoBi$CLMSEX<-factor(AutoBi$CLMSEX)
levels(AutoBi$CLMSEX) <- c("M", "F") 
str(AutoBi$ATTORNEY)
str(AutoBi$CLMSEX)


tab1 <- table(AutoBi$CLMSEX,AutoBi$ATTORNEY) 
tab1

# la funzione margin.table applicata ad una tabella a doppia entrata
# permette di ottenere le distribuzioni marginali (le distribuzioni
# di frequenza delle variabili prese singolarmente)

margin.table(tab1,1) # marginale per sesso

margin.table(tab1,2) # marginale per avvocato

# Nel caso delle tabelle a doppia entrata possiamo calcolare: 
# - la distribuzione delle frequenze relative congiunte
prop.table(tab1)                 
# - la distribuzione delle frequenze relative condizionate di riga
prop.table(tab1, 1)      # le righe sommano a 1      
# - la distribuzione delle frequenze relative condizionate di colonna
prop.table(tab1, 2)     # le colonne sommano a 1


## Associazione

# La tabella a doppia entrata è esposta per evidenziare che la conoscenza di una
# variabile aumenta le nostre informazioni sulla seconda.

# Nella maggior parte dei casi si ha in mente quale sia il ruolo delle due
# variabili:
#  - variabile dipendente o variabile risposta: variabile oggetto di interesse
#  - variabile indipendente o fattore esplicativo: variabile che ci permetterebbe
# di aumentare le nostre informazioni sulla variabile risposta.

# La scelta di quale sia appropriata dipende dal contesto dell'analisi.

# cerchiamo le frequenze relative condizionate in relazione alla variabile
# indipendente.

# Ad esempio, ci aspettiamo che ci sia una relazione tra le variabili 
# `ATTORNEY` (risposta)
# `LOSS` (esplicativa)

AutoBi$LOSSclass<-cut(AutoBi$LOSS,breaks=c(0,0.5,2,4,8,1100))

table(AutoBi$ATTORNEY)  # frequenze assolute della variabile marginale ATTORNEY
tot<-prop.table(table(AutoBi$ATTORNEY))
tot                     # frequenze relative della variabile marginale ATTORNEY

tabella1<-table(AutoBi$ATTORNEY, AutoBi$LOSSclass)
tabella1                # tabella di frequenze congiunte

mio<-prop.table(tabella1, 2)  # tabella di frequenze condizionate di colonna (|LOSS)

cbind(mio, tot)        


# se le due variabili non sono associate, sono indipendenti
tabella2<-table(AutoBi$CLMSEX, CLMAGEclass)
tabella2                # tabella di frequenze congiunte

mio2<-prop.table(tabella2, 2)  # tabella di frequenze condizionate di colonna (|Age)
mio2

# per valutare se le differenze tra le distribuzioni condizionate (e marginale)
# possono ritenersi trascurabili abbiamo bisogno di considerazioni di tipo inferenziale 

# Nota: in caso di indipendenza, si riscontrerebbe anche invertendo il ruolo 
# delle variabili

### Tabelle di frequenze a più entrate

tabella3<-table(AutoBi$ATTORNEY, AutoBi$LOSSclass, AutoBi$CLMSEX)
tabella3
str(tabella3) #array con. 3 dimensioni

### Esercizio ----
# Donne al volante pericolo costante?

#1. Ottenere la distribuzione delle frequenze assolute e relative per sesso

#2. Suddividere la variabile perdita utilizzando i quartili della distribuzione

#3. Ottenere la distribuzione congiunta delle due variabili

#4  Calcolare la distribuzione della variabile sesso condizionatamente alla perdita

#5. Calcolare la distribuzione della variabile perdita condizionatamente al sesso

#6. Confrontare le distribuzioni condizionate con le rispettive marginali
