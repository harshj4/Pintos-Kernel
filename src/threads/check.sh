#!/bin/bash

fl=(
	alarm-multiple.result
	alarm-negative.result
	alarm-priority.result
	alarm-simultaneous.result
	alarm-single.result
	alarm-zero.result
	mlfqs-block.result
	mlfqs-fair-20.result
	mlfqs-fair-2.result
	mlfqs-load-1.result
	mlfqs-load-60.result
	mlfqs-load-avg.result
	mlfqs-nice-10.result
	mlfqs-nice-2.result
	mlfqs-recent-1.result
	priority-change.result
	priority-condvar.result
	priority-donate-chain.result
	priority-donate-lower.result
	priority-donate-multiple2.result
	priority-donate-multiple.result
	priority-donate-nest.result
	priority-donate-one.result
	priority-donate-sema.result
	priority-fifo.result
	priority-preempt.result
	priority-sema.result
)

for i in ${fl[@]}
do
    
    len=$(echo -n $i | wc -c)
    len=$(expr $len / 8)
    len=$(expr 6 - $len)
    echo -n $i
    printf "%0.s\t" $(seq 1 $len)
    
    if [ -f ./build/tests/threads/$i ]; then
        state=$(head -1 ./build/tests/threads/$i)
        if [ $state = "PASS" ]; then
            echo -e "\033[0;42m $state\033[0m"
        else
            echo -e "\033[0;41m $state\033[0m"
        fi
    else
        echo -e "\033[0;44m PENDING\033[0m"
    fi
done

