#!/bin/bash

RATES=(0.84 0.89 0.94 0.99)
AMPLIFIED=(84 89 94 99)
ACCEPTED=(100 200 500 1000 2000 5000 10000 20000 50000)

TOTAL=324
DONE=0

for rate_i in 0 1 2 3
do
    COOLING_RATE=${RATES[$rate_i]}
    AMPLIFIED_RATE=${AMPLIFIED[$rate_i]}

    for NEIGHBORS_ACCEPTED in ${ACCEPTED[@]}
    do
        for INSTANCE in `ls input`
        do
            echo "=========================================="
            echo "Testing ${INSTANCE}..."
            echo "Cooling rate: ${COOLING_RATE}"
            echo "Neighbors accepted: ${NEIGHBORS_ACCEPTED}"
            echo "=========================================="

            for BATCH in 1 2
            do
                echo "Doing batch $BATCH..."
                for i in 1 2 3 4 5
                do
                    NAME=`basename $INSTANCE .json`
                    TEST=$(((${BATCH} - 1) * 5 + ${i}))

                    DIR=output/SA/${AMPLIFIED_RATE}_${NEIGHBORS_ACCEPTED}/${NAME}

                    mkdir -p ${DIR}
                    
                    ./main --main::instance input/$INSTANCE \
                            --main::method SA \
                            --ExchangeSA::min_temperature 0.1 \
                            --ExchangeSA::compute_start_temperature-enable \
                            --ExchangeSA::neighbors_sampled 100000 \
                            --ExchangeSA::neighbors_accepted $NEIGHBORS_ACCEPTED \
                            --ExchangeSA::cooling_rate $COOLING_RATE \
                            --MySolver::timeout 300.0 \
                            > ${DIR}/${TEST}.txt &
                done

                echo "Waiting..."
                wait
            done

            DONE=$(($DONE+1))
        done

        echo "============================="
        echo "[Done/Total] = [$DONE/$TOTAL]"
        echo "============================="
        echo ""
    done
done
