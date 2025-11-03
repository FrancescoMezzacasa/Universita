#!/bin/bash

if (( $# < 2 ))
then
    echo "Usage: ./container-run.sh conf-file.txt command <options>"
else
    temporanea="/tmp/container"
    mkdir -p $temporanea
    IFS=$'\n'

    #esempio io devo copiare /usr/bin/ls in /temp/usr/bin/ <-- togliere ls
    for riga in $(cat "$1")
    do
        fuori=$(echo "$riga" | cut -d " " -f 1)
        dentro=$(echo "$riga" | cut -d " " -f 2)

        if [ -d "$fuori" ]
        #se sono cartelle 
        then
            mkdir -p $temporanea$dentro
            bindfs --no-allow-other $fuori $temporanea$dentro
        elif [ -f "$fuori" ]
        then
            folder=$(dirname "$dentro")
            mkdir -p $temporanea$folder
            cp $fuori $temporanea$folder
        fi
    done
    echo "Eseguo $temporanea ${@:2}"
    fakechroot chroot $temporanea ${@:2} 
fi