#FRANCESCO MEZZACASA SM3201587

class RendererError(Exception):
    #classe per le eccezioni generali però specifiche per il progetto
    pass

class PaletteError(RendererError):
    #classe per gli errori relativi alla palette
    pass

class VRAMError(RendererError):
    #classe per gli errori relativi alla VirtualVRAM
    pass

class SceneParsingError(RendererError):
    #classe per gli errori relativi al parsing della scena
    pass

class BlitterError(RendererError):
    #classe per gli errori relativi al disegno di tile e sprite nella scena
    pass