#FRANCESCO MEZZACASA SM3201587
import json
from exceptions import SceneParsingError

class SceneParser():
    def __init__(self, scenepath):
        try:
            with open(scenepath, "r") as scenefile:
                self.scena = json.load(scenefile)
        except FileNotFoundError:
            raise SceneParsingError("File scena non trovato")
        except json.JSONDecodeError:#JSONDecodeError è l'errore che si crea quando il file dato non è in formato json valido
            raise SceneParsingError("File JSON scena non valido")
        
        if(type(self.scena) != dict):
            #se non contiene un singolo oggetto JSON non riesco a vederlo come dizionario
            raise SceneParsingError("Il file JSON deve contenere un oggetto JSON")
        
        #se non ho errori il file json è stato interpretato come un dizionario python
        #faccio i controlli che i campi (chiavi del dizionario) ci siano tutti e siano conformi
        campi = ["transparent_index", "tile_map", "sprites"]#questi sono i campi che ci devono essere (almeno)
        for c in campi:
            if c not in self.scena.keys():
                raise SceneParsingError("Il file scena non contiene tutti i campi necessari")
            
        if(type(self.scena["transparent_index"]) != int):
            raise SceneParsingError("L'indice del colore di trasparenza deve essere un numero intero")
        if(self.scena["transparent_index"] < 0 or self.scena["transparent_index"] > 15):
            raise SceneParsingError("L'indice del colore di trasparenza deve essere un numero compreso tra 0 e 15 (ci sono solo 16 colori)")
        
        if(type(self.scena["tile_map"]) != list):
            raise SceneParsingError("La tile map deve essere una matrice")
        if(len(self.scena["tile_map"]) != 15):
            raise SceneParsingError("La tile map deve avere le dimensioni di 15x20")
        for i in self.scena["tile_map"]:
            if(type(i) != list):
                raise SceneParsingError("La tile map deve essere una matrice")
            if(len(i) != 20):
                raise SceneParsingError("La tile map deve avere le dimensioni di 15x20")
            for val in i:
                if(type(val) != int):
                    raise SceneParsingError("I valori della tile map devono essere interi")
                if(val < 0 or val > 63):
                    raise SceneParsingError("I tile devono essere identificati con valori da 0 a 63")

        if(type(self.scena["sprites"]) != list):
            raise SceneParsingError("Il campo sprites deve contenere una lista di sprite")
        campi_sprite = ["id", "x", "y", "flip_h", "flip_v", "rotation"]#campi che devono esserci per ogni sprite
        for sprite in self.scena["sprites"]:
            if(type(sprite) != dict):
                raise SceneParsingError("Ogni sprite deve essere un oggetto JSON")
            
            for c in campi_sprite:
                if c not in sprite.keys():
                    raise SceneParsingError(f"Gli sprite devono contenere almeno i campi {campi_sprite}")
            
            if(type(sprite["id"]) != int):
                raise SceneParsingError("L'id dello sprite deve essere un numero intero")
            if(sprite["id"] < 0 or sprite["id"] > 15):
                raise SceneParsingError("Gli sprite devono essere indicizzati da 0 a 15")
            if(type(sprite["x"]) != int or type(sprite["y"]) != int):
                raise SceneParsingError("Le coordinate degli sprite devono essere intere")
            if(type(sprite["flip_h"]) != bool or type(sprite["flip_v"]) != bool):
                raise SceneParsingError("I valori di flip orizzontale e verticale devono essere booleani")
            valori_rotation = [0, 90, 180, 270]
            if(sprite["rotation"] not in valori_rotation):
                raise SceneParsingError("Uno sprite può essere ruotato solo di 0, 90, 180 o 270 gradi")
            
        #ora che so che sono giusti i vari campi li metto come attributi
        self.transparent_index = self.scena["transparent_index"]
        self.tile_map = self.scena["tile_map"]
        self.sprites = self.scena["sprites"]
