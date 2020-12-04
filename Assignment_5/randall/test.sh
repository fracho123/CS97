#!/bin/bash
N=1000
if [ "$(./randall "$N" | wc -c)" = "$N" ]
then
    echo "Pass 1" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i mrand48_r "$N" | wc -c)" = "$N" ]
then
    echo "Pass 2" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i /dev/urandom "$N" | wc -c)" = "$N" ]
then
    echo "Pass 3" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -o stdio "$N" | wc -c)" = "$N" ]
then
    echo "Pass 4" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -o 100 "$N" | wc -c)" = "$N" ]
then
    echo "Pass 5" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i rdrand -o stdio "$N" | wc -c)" = "$N" ]
then
    echo "Pass 6" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i rdrand -o 100 "$N" | wc -c)" = "$N" ]
then
    echo "Pass 7" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i mrand48_r -o stdio "$N" | wc -c)" = "$N" ]
then
    echo "Pass 8" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i mrand48_r -o 100 "$N" | wc -c)" = "$N" ]
then
    echo "Pass 9" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i /dev/urandom -o stdio "$N" | wc -c)" = "$N" ]
then
    echo "Pass 10" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

if [ "$(./randall -i /dev/urandom -o 100 "$N" | wc -c)" = "$N" ]
then
    echo "Pass 11" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

N=100
if [ "$(./randall "$N" | wc -c)" = "$N" ]
then
    echo "Pass 12" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

N=10
if [ "$(./randall "$N" | wc -c)" = "$N" ]
then
    echo "Pass 13" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

N=10000
if [ "$(./randall "$N" | wc -c)" = "$N" ]
then
    echo "Pass 14" # Success!
else
    echo "Failure"
    exit 1 # Failure
fi

echo "Success"

time dd if=/dev/urandom ibs=8192 obs=8192 count=16384 >/dev/null
time ./randall 133562368 >/dev/null
time ./randall 133562368 | cat >/dev/null
time ./randall 133562368 >rand.data