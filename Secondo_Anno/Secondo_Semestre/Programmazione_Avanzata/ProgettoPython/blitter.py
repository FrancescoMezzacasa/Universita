#FRANCESCO MEZZACASA SM3201587

import numpy as np
from exceptions import BlitterError
#in questa classe io metterò solo un singolo tile o sprite al suo posto, 
#sarà la classe renderingPipeline a chiamare questa classe tante volte quanti tile e sprite 
#dovrà posizionare
class Blitter():
    def __init__(self, framebuffer, vram):
        #ricevo come parametri il frame buffer e la vram gia inizializzati da renderingpipeline
        #vram mi serve perche li ho il tile sheet e lo sprite sheet gia condificati
        self.framebuffer = framebuffer
        self.vram = vram
    
    #creo due metodi separati per tiles e sprites
    def posizionaTile(self, tileid, tilei, tilej):
        #tilei e tilej sono gli indici del tile nella matrice 15*20 che è tutto il frame buffer (li vedo come blocchi)
        #devo disegnarlo nel frame buffer, partendo dall indice i, j e andando avanti di 32 in orizzontale e verticale

        riga_sheet = tileid//8
        colonna_sheet = tileid%8
        self.framebuffer[tilei*32:tilei*32 + 32, tilej*32:tilej*32 + 32] = self.vram.tiles[riga_sheet*32:riga_sheet*32 + 32, colonna_sheet*32:colonna_sheet*32 + 32]
    
        #spiegazione:
        #i tile sono identificati con numeri da 0 a 15, ma sono in una matrice 8*8, quindi voglio
        #avere riga e colonna avendo solo l'id, facendo i calcoli si vede che la riga si ottiene facendo la
        #divisione intera per 8 (ogni volta che arrivi ad un nuovo multiplo di 4 sei alla riga successiva)
        #mentre la colonna si ottiene con il resto della divisione per 4 (infatti solo valori da 0 a 7 che si resettano ogni riga)
        #dovrò poi però riferirmi ai pixel, e quindi moltiplico per 32 per essere all'inizio del tile (pixel in alto a sinistra)
        #e scorrero per 32 pixel sia in orizzontale che in verticale

        #per le x e y del frame buffer invece uso la i e la j che identificano il tile all interno della tile map
        #nella scena (file sceneparser) e quindi la i del tile la moltiplico per 32 che è l'altezza dei tile
        #per ottenere da che pixel parte nel tile sheet, e stessa cosa per la j
        #prenderò poi i successivi 32 pixel

    def posizionaSprite(self, spid, inputx, inputy, fliph, flipv, rot, transparent_index):
        #NB si veda alla fine della funzione per chiarimento sul significato delle variabili

        #ora dovrò fare diverse cose quindi non posso fare in una riga ma separo

        #inizialmente estraggo la sottomatrice 64*64 che rappresenta lo sprite giusto dallo sprite sheet
        #si applica la logica di prima per ottenere le coordinate avendo solo un identificatore

        colonna_sheet = spid%4
        riga_sheet = spid//4

        inizioysheet = riga_sheet * 64 #ogni sprite è alto 64
        inizioxsheet = colonna_sheet * 64 #ogni sprite è largo 64

        sprite = self.vram.sprites[inizioysheet:inizioysheet + 64, inizioxsheet:inizioxsheet + 64]#stavolta i blocchi sono lunghi 64
        #sprite è la sottomatrice presa dallo sprite sheet

        #ora applico le trasformazioni
        #uso le funzioni di numpy per flippare e ruotare matrici (di fatto lo sprite è una matrice di pixel)
        if(fliph == True):
            sprite = np.fliplr(sprite)
        if(flipv == True):
            sprite = np.flipud(sprite)
        #assumo che le rotazioni vogliano essere fatte in senso orario
        sprite = np.rot90(sprite, k=-rot//90)
        #questa funzione vuole sapere quante volte ruotare di 90 gradi quindi faccio //90
        #inoltre metto il segno meno perche la funzione di default gira in senso antiorario

        #ora controllo se uno sprite finirebbe fuori dallo schermo e in caso taglio la sottomatrice
        #prima controllo se sono completamente fuori
        inizioframebufferx = inputx
        inizioframebuffery = inputy
        fineframebufferx = inputx + 64 #di base lo sprite finisce 64 pixel dopo
        fineframebuffery = inputy + 64
        #queste variabili rappresentano le posizioni x e y finali e iniziali NEL FRAME BUFFER a cui andra disegnato lo sprite

        if(inputx > 639 or inputy > 479 or fineframebufferx <= 0 or fineframebuffery <= 0):
            #controllo se dovrebbe essere disegnato completamente fuori
            #se è cosi semplicemente non lo disegno
            return
        #se sono qua so che di sicuro non saranno tutti fuori

        if(fineframebufferx > 640):#se finirebbe fuori (ma di sicuro inizia dentro) allora dico che finisca alla fine
            fineframebufferx = 640
        if(fineframebuffery > 480):
            fineframebuffery = 480
        
        if(inputx < 0):#se inizia fuori l'inizio vero è a 0
            inizioframebufferx = 0
        if(inputy < 0):
            inizioframebuffery = 0
        
        #adesso ho che lo sprite andra disegnato nel frame buffer a partire da inizioframebufferx e fino a fineframebufferx, idem per y

        #adesso devo calcolare quali pixel dello sprite verranno effettivamente disegnati
        #l'inizio dello sprite sarebbe dove dovrebbe partire meno da dove effettivamente parte, cosi ottengo quanti pixel sono tagliati
        #e cosi so di partire da li cosi non scrivo quelli tagliati
        iniziospritex = inizioframebufferx - inputx
        iniziospritey = inizioframebuffery - inputy
        #per calcolare dove finisce invece sommo la coordinata dell inizio piu quanto viene EFFETTIVAMENTE scritto, cioè la differenza tra inizio e fine nel frame buffer
        finespritex = iniziospritex + (fineframebufferx - inizioframebufferx)
        finespritey = iniziospritey + (fineframebuffery - inizioframebuffery)

        #ora devo effettivamente "tagliare" lo sprite
        spritetagliato = sprite[iniziospritey:finespritey, iniziospritex:finespritex]

        #ora mi taglio la parte di frame buffer in cui devo disegnare
        sezioneframebuffer = self.framebuffer[inizioframebuffery:fineframebuffery, inizioframebufferx:fineframebufferx]

        #ora lo disegno però tenendo conto di non disegnare i pixel che hanno il colore di trasparenza
        #creo una maschera con i pixel che sono diversi dal colore di trasparenza, e quelli sono gli unici che voglio colorare
        maschera = (spritetagliato != transparent_index)
        #ora posso applicare questa maschera al frame buffer
        sezioneframebuffer[maschera] = spritetagliato[maschera]

        #RIASSUNTO SIGNIFICATO VARIABILI:
        #ho usato molte variabili con nomi simili o ambigui, quindi spiegherò qui cosa rappresentano (spiego solo per le x perche per le y è tutto uguale)
        #inizioxsheet è la coordinata del pixel iniziale nello sprite sheet dello sprite giusto
        #inputx è la x a cui l'utente intende mettere lo sprite, che però potrebbe essere fuori dal frame buffer
        #inizioframebufferx è la coordinata a cui lo sprite verrà effettivamente disegnato NEL FRAME BUFFER, quindi questa è quella che viene corretta
        #se lo sprite va fuori
        #fineframebufferx è come inizioframebufferx ma rappresenta la fine
        #iniziospritex rappresenta la coordinata DEL SINGOLO QUADRATO PRESO DALLO SPRITE SHEET (sprite) a cui iniziero a disegnarlo
        #questa mi serve perche se calcolo nel frame buffer che una parte sarebbe fuori non dovrò disegnare tutto il quadrato ma solo una parte
        #finespritex è come iniziospritex ma è la fine
