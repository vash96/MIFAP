#!/bin/bash

ALPHA=(0.1 0.2 0.4 0.5)
AMPLIFIED=(1 2 4 5)
N_SIZE=(150 650 1150 1650)
F_SIZE=(40 70)

METHOD=TS

TOTAL=288
DONE=0

for alpha_i in 0 1 2 3
do
    ALPHA_CHOICE=${ALPHA[$alpha_i]}
    AMPLIFIED_CHOICE=${AMPLIFIED[$alpha_i]}

    for N in ${N_SIZE[@]}
    do
        for F in ${F_SIZE[@]}
        do
            # MAX_TENURE = alpha*N*F
            MAX_TENURE=$((${AMPLIFIED_CHOICE} * ${N} * ${F} / 10))
            MIN_TENURE=$((${MAX_TENURE} / 100))

            for INSTANCE in `ls input`
            do
                echo "=========================================="
                echo "Testing ${INSTANCE}..."
                echo "Alpha factor: ${ALPHA_CHOICE}"
                echo "Number of cells: ${N}"
                echo "Number of freqs: ${F}"
                echo "=========================================="

                for BATCH in 1 2
                do
                    echo "Doing batch $BATCH..."
                    for i in 1 2 3 4 5
                    do
                        NAME=`basename $INSTANCE .json`
                        TEST=$(((${BATCH} - 1) * 5 + ${i}))

                        DIR=output/${METHOD}/${AMPLIFIED_CHOICE}_${N}_${F}/${NAME}

                        mkdir -p ${DIR}
                        
                        ./main --main::instance input/$INSTANCE \
                                --main::method ${METHOD} \
                                --ExchangeTS::max_idle_iterations -1 \
                                --ExchangeTS::max_tenure ${MAX_TENURE} \
                                --ExchangeSA::min_tenure ${MIN_TENURE} \
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
done
