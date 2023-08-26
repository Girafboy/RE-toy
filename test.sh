#!/bin/bash

cd ./cmake-build-release || exit
out_put_file="../output/result.csv"
max_time="3"  # second

echo "algorithm,graph,params,construction(ns),index(B),query_num,query_mean(ns),query_stdev" > ${out_put_file}

graphs=(
# random graphs
"--random 5 1"
#"--random 500 10"
"--random 4125 10"
"--random 10000 10"
# graphs from files
"--file ../graphs/simple.txt"
)

algorithms=(
# XJump
"xjump 32"
# BFL
"bfl 5"
# Grail
#"grail 1 0 2"
# PathTree
#"pathtree 1"
# TOL
#"tol 0 2"
# Gripp
#"gripp"
# Ferrari
#"ferrari 2 32 1"
# IP
#"ip 2 2 100"
# PLL
#"pll 1"
# PReaCH
#"preach"
# DBL
#"dbl"
)

# todo 超时print信息
for algorithm in "${algorithms[@]}"
do
  echo "Start testing ${algorithm}"
  for graph in "${graphs[@]}"
  do
    tmp_command="timeout ${max_time} ./tc --time ${max_time} ${graph} --algorithm ${algorithm}"
    eval "${tmp_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo "Timeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done


#timeout ${max_time} ./tc --time ${max_time} --file ./graphs/simple.txt --algorithm xjump 32
#timeout ${max_time} ./tc --time ${max_time} --random 5 1 --algorithm xjump 32
#timeout ${max_time} ./tc --time ${max_time} --random 500 10 --algorithm xjump 32
#timeout ${max_time} ./tc --time ${max_time} --random 4125 10 --algorithm xjump 32
#timeout ${max_time} ./tc --time ${max_time} --random 10000 10 --algorithm xjump 32