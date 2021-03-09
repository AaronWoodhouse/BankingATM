#!/bin/bash

#THIS IS THE INPUT TXT FILE
input = "changeplan1.inp"

#THIS IS TO READ ALL OF THE STDOUT
exec > login1.ato

#THIS IS WHERE YOU EXECUTE THE .EXE
..\..\..\BankFrontEnd.exe

#READ ALL THE LINE IN INPUT FILE
while read line;
do 
    $line;
done < "$input"
$line
kill

#COMPARE THE TERMINAL OUTPUT DIFFERENCE
diff login1.ato login1.bto > diffTerminal.txt

#COMPARE THE TRANSACTION FILE DIFFERENCE
diff login1.atf login1.etf > diffTransaction.txt