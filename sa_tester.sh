#!/bin/bash

RATES=(0.84 0.89 0.94 0.99)
ACCEPTED=(100 200 500 1000 2000 5000 10000 20000 50000)

TOTAL=324
DONE=0

for COOLING_RATE in ${RATES[@]}
do
    for NEIGHBORS_ACCEPTED in ${ACCEPTED[@]}
    do
        for INSTANCE in `ls input`
        do
            for BATCH in 1 2
            do
                echo "Doing batch $BATCH..."
                for i in 1 2 3 4 5
                do
                    
                done

                echo "Waiting..."
                wait
            done

            DONE=$(($DONE+1))
        done

        echo "============================="
        echo "[Done/Total] = [$DONE/$TOTAL]"
        echo "============================="
    done
done