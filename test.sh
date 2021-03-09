#!/bin/bash

cd inputs

for i in `ls`
do
	for j in `ls $i`
	do
		echo "Test $j"
		../BankFrontEnd.exe ../accounts.txt $i/$j/$j.atf < $i/$j/$j.inp > $i/$j/$j.out
	done
done

for i in `ls`
do
	for j in `ls $i`
	do
		echo "Checks for differences on: $j"
		diff $i/$j/$j.ato $i/$j/$j.bto > diffTerminal.txt
		diff $i/$j/$j.atf $i/$j/$j.etf > diffTransaction.txt
	done
done
