#!/bin/bash

if [ "$1" = "DEBUG" ]
then
	gcc -o dht22 dht22.c -L/usr/local/lib -lwiringPi -std=c99 -DDEBUG
else
	gcc -o dht22 dht22.c -L/usr/local/lib -lwiringPi -std=c99
fi
