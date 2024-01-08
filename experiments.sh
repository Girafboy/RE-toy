#!/bin/bash

# This script automatically runs four experiments mentioned in the paper, namely tradeoff, real_graph, scale_up, and dense_up.

# config

# path to the executable file
executable="./build/reachability"

# common parent directory of graph files
input_directory="./graphs/"

# path to the output directory of all the results of the four experiments
# results will be stored in subdirectories called tradeoff, real_graph, scale_up, and dense_up, respectively
output_dir="./output"

# test number of query pairs in each experiment
query_num="100000"

# maximum execution time for each single test in seconds
max_time="1000"


# convert relative paths to absolute paths
input_directory=$(readlink -f "${input_directory}")
output_dir=$(readlink -f "${output_dir}")

# create directories and clear old outputs
subdirs=("tradeoff" "real_graph" "scale_up" "dense_up")
mkdir -p "${output_dir}"
for subdir in "${subdirs[@]}"
do
    subdir_path="${output_dir}/${subdir}"
    mkdir -p "${subdir_path}"
    echo "algorithm,graph,params,construction(ns),index(B),query_num,query_mean(ns)" > "${subdir_path}/result.csv"
    mkdir -p "${subdir_path}/query_time"
    rm "${subdir_path}/query_time"/*.txt
done


######################################## Experiment 1: tradeoff ########################################

# input graphs
graphs=(
  "--random 1000 10"
)

# algorithms to be tested
algorithms=(
# RE-toy
  "re_toy 8 2.0"
  "re_toy 16 2.0"
  "re_toy 32 2.0"
  "re_toy 64 2.0"
  "re_toy 96 2.0"
  "re_toy 128 2.0"
  "re_toy 192 2.0"
  "re_toy 256 2.0"
  "re_toy 512 2.0"
  "re_toy 1024 2.0"
# BFL
  "bfl 1"
  "bfl 2"
  "bfl 5"
  "bfl 10"
  "bfl 20"
  "bfl 50"
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
  "tol 0 2"
  "tol 1 2"
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
echo "Experiment 1: tradeoff starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/tradeoff/result.csv --result_dir ${output_dir}/tradeoff/query_time/"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
echo -e "\033[0G\033[2KExperiment 1: tradeoff done."


######################################## Experiment 2: real_graph ########################################

# input graphs
graphs=(
  "--file ${input_directory}/econ-poli.txt"
  "--file ${input_directory}/ash958.txt"
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
# RE-toy
  "re_toy 8 2.0"
  "re_toy 16 2.0"
  "re_toy 32 2.0"
  "re_toy 64 2.0"
  "re_toy 128 2.0"
  "re_toy 256 2.0"
  "re_toy 512 2.0"
  "re_toy 1024 2.0"
  "re_toy 2048 2.0"
  "re_toy 4096 2.0"
  "re_toy 8192 2.0"
  "re_toy 16384 2.0"
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
  "tol 0 2"
  "tol 1 2"
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
echo "Experiment 2: real_graph starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/real_graph/result.csv --result_dir ${output_dir}/real_graph/query_time/"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
echo -e "\033[0G\033[2KExperiment 2: real_graph done."


######################################## Experiment 3: scale_up ########################################

# input graphs
graphs=(
  "--random 32 6"
  "--random 64 8"
  "--random 128 12"
  "--random 256 16"
  "--random 512 23"
  "--random 1024 32"
  "--random 2048 46"
  "--random 4096 64"
  "--random 8192 91"
)

# RE-toy algorithm with recommended parameters for each graph
re_toys=(
  "re_toy 5 2.0"
  "re_toy 8 2.0"
  "re_toy 11 2.0"
  "re_toy 16 2.0"
  "re_toy 22 2.0"
  "re_toy 32 2.0"
  "re_toy 45 2.0"
  "re_toy 64 2.0"
  "re_toy 90 2.0"
)

# other baseline algorithms with their recommended parameters for all the graphs above
baselines=(
  "bfl 5"
  "grail -2 1 5"
  "pathtree 2"
  "tol 0 2"
  "ferrari 5 32 1"
  "ip 5 5 100"
  "pll 1"
  "preach"
  "dbl"
)

# main tests
echo "Experiment 3: scale_up starts."
# RE-toy
echo -ne "\033[2K\rTesting re_toy..."
for ((i=0; i<${#graphs[@]}; i++))
do
  graph="${graphs[$i]}"
  algorithm="${re_toys[$i]}"
  test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/scale_up/result.csv --result_dir ${output_dir}/scale_up/query_time/"
  eval "${test_command}"
  exit_status=$?
  if [[ $exit_status -eq 124 ]]; then
      echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
  fi
done
# baseline algorithms
for algorithm in "${baselines[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/scale_up/result.csv --result_dir ${output_dir}/scale_up/query_time/"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
echo -e "\033[0G\033[2KExperiment 3: scale_up done."


######################################## Experiment 4: dense_up ########################################

# input graphs
graphs=(
  "--random 1024 1"
  "--random 1024 2"
  "--random 1024 4"
  "--random 1024 8"
  "--random 1024 16"
  "--random 1024 32"
  "--random 1024 64"
  "--random 1024 128"
  "--random 1024 256"
  "--random 1024 511"
)

# algorithms to be tested
algorithms=(
  "re_toy 32 2.0"
  "bfl 5"
  "grail -2 1 5"
  "pathtree 2"
  "tol 0 2"
  "ferrari 5 32 1"
  "ip 5 5 100"
  "pll 1"
  "preach"
  "dbl"
)

# main tests
echo "Experiment 4: dense_up starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/dense_up/result.csv --result_dir ${output_dir}/dense_up/query_time/"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
echo -e "\033[0G\033[2KExperiment 4: dense_up done."
