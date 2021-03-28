#!/bin/bash

RATES=(0.84 0.89 0.94 0.99)
ACCEPTED=(100 200 500 1000 2000 5000 10000 20000 50000)

for COOLING_RATE in ${RATES[@]}
do
    for NEIGHBORS_ACCEPTED in ${ACCEPTED[@]}
    do
        for INSTANCE in `ls input`
        do
            for test_number in {1,2,3,4,5,6,7,8,9,10}
            do
                NAME=`basename $INSTANCE .json`
                echo "Considering file $INSTANCE"
                echo "Instance name is $NAME"
                echo "Cooling rate is $COOLING_RATE"
                echo "Accepted is $NEIGHBORS_ACCEPTED"
                echo "I will define directory output/SA/${COOLING_RATE}_${NEIGHBORS_ACCEPTED}/$NAME/$test_number.txt"
                echo ""
            done
        done
    done
done