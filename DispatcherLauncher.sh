#!/bin/bash

gcc -o Dispatcher Dispatcher.c

gcc -o counter counter.c
./Dispatcher a textFile.txt
