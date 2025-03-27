#!/bin/bash

if (( $# < 1 ))
then
    echo "Serve almeno un argomento tra view, search, insert, delete"
else
    if [ "$1" == "view" ]
    then
        column address-book-database.csv -t -s ',' | head -n 1
        tail +2 address-book-database.csv | sort -k 4 -t "," | column -t -s ',' 
    elif [ "$1" == "search" ]
    then
        if (( $# != 2 ))
        then 
            echo "Uso corretto: $./address-book.sh search <stringa>"
        else
            #voglio ciclare su ogni linea che ritorna il grep, singolarmente poi separarla
            #e poi fare cut
            IFS=$'\n'
            ris=$( grep -i "$2" address-book-database.csv )
            if (( $? == 1 ))
            then
                echo "Not found"
            else
                for i in $ris #ris è una linea che contiene il pattern
                do
                    echo
                    nome=$( echo $i | cut -d "," -f 1 )
                    echo "Name: $nome"

                    cognome=$( echo $i | cut -d "," -f 2 )
                    echo "Surname: $cognome"

                    telefono=$( echo $i | cut -d "," -f 3 )
                    echo "Phone: $telefono"

                    mail=$( echo $i | cut -d "," -f 4 )
                    echo "Mail: $mail"

                    citta=$( echo $i | cut -d "," -f 5 )
                    echo "City: $citta"

                    indirizzo=$( echo $i | cut -d "," -f 6 )
                    echo "Address: $indirizzo"
                done
                echo
            fi
        fi
    elif [ "$1" == "insert" ]
    then
        echo -n "Name: "
        read name
        echo -n "Surname: "
        read surname
        echo -n "Phone: "
        read phone
        echo -n "Mail: "
        read mail
        echo -n "City: "
        read city
        echo -n "Address: "
        read address

        #ora faccio la ricerca, prima taglio il file per avere solo le mail

        tail +2 address-book-database.csv | cut -d "," -f 4 | grep -x $mail
        if (( $? == 0 ))
        then
            echo "Indirizzo mail già presente"
        else
            echo >> address-book-database.csv
            echo "$name,$surname,$phone,$mail,$city,$address" >> address-book-database.csv
            echo "Added"
            echo
        fi
    elif [ "$1" == "delete" ]
    then
        if (( $# != 2 ))
        then
            echo "Uso corretto: ./address-book.sh delete <mail>"
        else
            righe_file_originale=$( wc -l < address-book-database.csv )
            #uso -v perche cosi mi stampa solo le linee che non contengono il pattern
            cancellato=$( cat address-book-database.csv | grep -v $2 )
            echo "$cancellato" > address-book-database.csv
            righe_file_nuovo=$( wc -l < address-book-database.csv )
            echo "vecchio=$righe_file_originale, nuovo=$righe_file_nuovo"
            
            if (( $righe_file_nuovo < $righe_file_originale ))
            then
                echo "Deleted"
            elif (( $righe_file_nuovo == $righe_file_originale ))
            then
                echo Cannot find any record
            fi
        fi
    fi
fi