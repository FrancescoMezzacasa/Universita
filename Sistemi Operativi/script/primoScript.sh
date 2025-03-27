#!/bin/bash
#primo esempio di script
echo $RANDOM
echo "Scrivi un numero da uno a dieci: "
read numero
echo "Scrivi un numero da uno a dieci: "
read numero2
y="I numeri scelti sono $numero e $numero2"
somma=$(( numero + numero2 ))
echo $y
echo $somma
