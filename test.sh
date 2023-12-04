#!/bin/bash

# config

# path to the executable file
executable="./cmake-build-debug/reachability"

# common parent directory of graph files
# ignore if only use random DAGs
input_directory="./graphs/"

# flag to indicate whether to perform accuracy tests
# true: test accuracy
# false: test index size, construction time, and query time
test_accuracy=false

# path to the output result.csv
# ignore if test_accuracy
output_file="./output/result.csv"

# path to the directory of output query time files
# ignore if test_accuracy
# !!!IMPORTANT!!!: the script would DELETE all .txt files under this directory before writing new query time files
# please save all important files under this directory before running this script
output_query_time_dir="./output/query_time/"

# test number of query pairs
# ignore if test_accuracy
query_num="100000"

# maximum execution time in seconds
max_time="1000"


# convert relative paths to absolute paths
input_directory=$(readlink -f "${input_directory}")
output_file=$(readlink -f "${output_file}")
output_query_time_dir=$(readlink -f "${output_query_time_dir}")

if [ "${test_accuracy}" = false ]; then
  # clean old outputs
  echo "algorithm,graph,params,construction(ns),index(B),query_num,query_mean(ns)" > ${output_file}
  rm "${output_query_time_dir}"/*.txt
fi

# input graphs
graphs=(
# random graphs
 "--random 1000 10"
# graphs from files
 "--file ${input_directory}/econ-poli.txt"
 "--file ${input_directory}/mouse_visual.cortex_1.txt"
 "--file ${input_directory}/yago_sub_6642.txt"
 "--file ${input_directory}/econ-beaflw.txt"
 "--file ${input_directory}/bcsstm27.txt"
 "--file ${input_directory}/bcsstk04.txt"
 "--file ${input_directory}/BA-1_10_60-L5.txt"
 "--file ${input_directory}/c-fat500-10.txt"
 "--file ${input_directory}/scc_infect-hyper.txt"
 "--file ${input_directory}/complete-1000.txt"
)

# algorithms to be tested
algorithms=(
 # ORSE-Toy
   "orse_toy 8 2.0"
   "orse_toy 16 2.0"
   "orse_toy 32 2.0"
   "orse_toy 64 2.0"
   "orse_toy 128 2.0"
   "orse_toy 256 2.0"
   "orse_toy 512 2.0"
   "orse_toy 1024 2.0"
   "orse_toy 2048 2.0"
   "orse_toy 4096 2.0"
 # BFL
   "bfl 1"
   "bfl 2"
   "bfl 5"
   "bfl 10"
   "bfl 20"
   "bfl 50"
   "bfl 100"
   "bfl 200"
   "bfl 500"
 # Grail
   "grail 1 0 2"
   "grail 1 0 3"
   "grail 1 0 4"
   "grail 1 0 5"
   "grail 1 1 2"
   "grail 1 1 3"
   "grail 1 1 4"
   "grail 1 1 5"
   "grail -2 0 2"
   "grail -2 0 3"
   "grail -2 0 4"
   "grail -2 0 5"
   "grail -2 1 2"
   "grail -2 1 3"
   "grail -2 1 4"
   "grail -2 1 5"
 # PathTree
   "pathtree 1"
   "pathtree 2"
 # TOL
   "tol 0 1"
   "tol 0 2"
   "tol 1 2"
 # Gripp
   "gripp"
 # Ferrari
   "ferrari 2 32 1"
   "ferrari 5 32 1"
   "ferrari 2 32 0"
   "ferrari 5 32 0"
 # IP
   "ip 2 2 100"
   "ip 5 5 100"
 # PLL
   "pll 1"
   "pll 0"
 # PReaCH
   "preach"
 # DBL
   "dbl"
)

# main tests
for algorithm in "${algorithms[@]}"
do
  echo "Start testing ${algorithm}"
  for graph in "${graphs[@]}"
  do
    if [ "$test_accuracy" = true ]; then
      test_command="timeout ${max_time} ${executable} --accuracy --graph ${graph} --algorithm ${algorithm}"
    else
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_file} --result_dir ${output_query_time_dir}"
    fi
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo "Timeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
