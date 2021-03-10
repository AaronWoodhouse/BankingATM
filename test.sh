#!/bin/bash

cd inputs

for i in `ls`
do
	for j in `ls $i`
	do
		echo "Test $j"
		../BankFrontEnd.exe ../accounts.txt $i/$j/$j.atf < $i/$j/$j.inp > $i/$j/$j.ato
	done
done

for i in `ls`
do
	for j in `ls $i`
	do
		echo "Checks for differences on: $j"
		diff $i/$j/$j.ato $i/$j/$j.bto > $i/$j/diffTerminal.txt
		diff $i/$j/$j.atf $i/$j/$j.etf > $i/$j/diffTransaction.txt
	done
done
