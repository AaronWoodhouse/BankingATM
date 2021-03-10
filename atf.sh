#!/bin/bash

cd inputs

for i in `ls`
do
	for j in `ls $i`
	do
		echo "Removing $j"
		rm $i/$j/$j.atf
	done
done
