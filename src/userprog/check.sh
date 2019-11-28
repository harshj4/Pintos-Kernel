#!/bin/bash

fl1=(
    args-dbl-space.result
    args-many.result
    args-multiple.result
    args-none.result
    args-single.result
    bad-jump2.result
    bad-jump.result
    bad-read2.result
    bad-read.result
    bad-write2.result
    bad-write.result
    close-bad-fd.result
    close-normal.result
    close-stdin.result
    close-stdout.result
    close-twice.result
    create-bad-ptr.result
    create-bound.result
    create-empty.result
    create-exists.result
    create-long.result
    create-normal.result
    create-null.result
    exec-arg.result
    exec-bad-ptr.result
    exec-bound-2.result
    exec-bound-3.result
    exec-bound.result
    exec-missing.result
    exec-multiple.result
    exec-once.result
    exit.result
    halt.result
    multi-child-fd.result
    multi-recurse.result
    open-bad-ptr.result
    open-boundary.result
    open-empty.result
    open-missing.result
    open-normal.result
    open-null.result
    open-twice.result
    read-bad-fd.result
    read-bad-ptr.result
    read-boundary.result
    read-normal.result
    read-stdout.result
    read-zero.result
    rox-child.result
    rox-multichild.result
    rox-simple.result
    sc-bad-arg.result
    sc-bad-sp.result
    sc-boundary-2.result
    sc-boundary-3.result
    sc-boundary.result
    wait-bad-pid.result
    wait-killed.result
    wait-simple.result
    wait-twice.result
    write-bad-fd.result
    write-bad-ptr.result
    write-boundary.result
    write-normal.result
    write-stdin.result
    write-zero.result
    # no-vm/multi-oom.result
)

fl2=(
    lg-create.result
    lg-full.result
    lg-random.result
    lg-seq-block.result
    lg-seq-random.result
    sm-create.result
    sm-full.result
    sm-random.result
    sm-seq-block.result
    sm-seq-random.result
    syn-read.result
    syn-remove.result
    syn-write.result

)

pass=0
pend=0
fail=0

echo
echo
echo -e "\033[4;1;45;33m                  Tests for userprog                 \033[0m"
echo
for i in ${fl1[@]}
do
    
    len=$(echo -n $i | wc -c)
    len=$(expr $len / 8)
    len=$(expr 6 - $len)
    echo -n $i
    printf "%0.s\t" $(seq 1 $len)
    
    if [ -f ./build/tests/userprog/$i ]; then
        state=$(head -1 ./build/tests/userprog/$i)
        if [ $state = "PASS" ]; then
            echo -e "\033[0;42m $state\033[0m"
            pass=$((pass + 1))
        else
            echo -e "\033[0;41m $state\033[0m"
            fail=$((fail + 1))
        fi
    else
        echo -e "\033[0;44m PENDING\033[0m"
    fi
done


echo
echo -e "\033[4;1;45;33m                  Tests for no-vm                    \033[0m"
echo
echo -n "no-vm/multi-oom.result"
printf "%0.s\t" $(seq 1 $len)

if [ -f ./build/tests/userprog/no-vm/multi-oom.result ]; then
    state=$(head -1 ./build/tests/userprog/no-vm/multi-oom.result)
    if [ $state = "PASS" ]; then
        echo -e "\033[0;42m $state\033[0m"
    else
        echo -e "\033[0;41m $state\033[0m"
    fi
else
    echo -e "\033[0;44m PENDING\033[0m"
fi

echo
echo -e "\033[4;1;45;33m                  Tests for filesys                  \033[0m"
echo
for i in ${fl2[@]}
do
    
    len=$(echo -n $i | wc -c)
    len=$(expr $len / 8)
    len=$(expr 6 - $len)
    echo -n $i
    printf "%0.s\t" $(seq 1 $len)
    
    if [ -f ./build/tests/filesys/base/$i ]; then
        state=$(head -1 ./build/tests/filesys/base/$i)
        if [ $state = "PASS" ]; then
            echo -e "\033[0;42m $state\033[0m"
        else
            echo -e "\033[0;41m $state\033[0m"
        fi
    else
        echo -e "\033[0;44m PENDING\033[0m"
    fi
done

echo
echo -e "Tests failed:    \033[5;31m $fail\033[0m"
echo -e "Tests passed:    \033[5;32m $pass\033[0m"
echo -e "Tests pending:   \033[5;34m $pend\033[0m"

# This is a record of commands:
#
# Command to put in any file, e.g. args-none
# pintos -p tests/userprog/args-none -a args-none -- -f -q
