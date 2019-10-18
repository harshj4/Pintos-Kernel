#!/bin/bash

fl=(alarm-multiple.result
alarm-negative.result
alarm-priority.result
alarm-simultaneous.result
alarm-single.result
alarm-zero.result
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
priority-sema.result)

for i in ${fl[@]}
do
	{ echo -ne $i "\t" & head -1 /home/ved/Pintos-Kernel/src/threads/build/tests/threads/$i  ; }
done

