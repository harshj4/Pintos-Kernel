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
	{ echo -ne $i "\t" & head -1 ./build/tests/threads/$i  ; }
done

