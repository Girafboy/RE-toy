#!/bin/bash

# This script automatically runs four experiments mentioned in the paper, namely parameter, tradeoff, real_graph, and dense_up.

# config

# path to the executable file
executable="./build/reachability"

# common parent directory of graph files
input_directory="./graphs/"

# path to the output directory of all the results of the four experiments
# results will be stored in parameter.csv, tradeoff.csv, real_graph.csv, and dense_up.csv, respectively
output_dir="./output"

# test number of query pairs in each experiment
query_num="100000"

# maximum execution time for each single test in seconds
max_time="10000"

# number of seeds for each random graph
seed_cnt=100


# create directories and clear old outputs
tasks=("parameter" "tradeoff" "real_graph" "dense_up")
mkdir -p "${output_dir}"
for task in "${tasks[@]}"
do
    echo "algorithm,graph,params,index(B),query_num,query_mean(ns)" > "${output_dir}/${task}.csv"
done

# convert relative paths to absolute paths
input_directory=$(readlink -f "${input_directory}")
output_dir=$(readlink -f "${output_dir}")


######################################## Experiment 1: parameter ########################################

# input graphs
graphs=(
  "--random 1000 10"
)

# algorithms to be tested
algorithms=(
# RE-toy
  "re_toy 8 1.0"
  "re_toy 16 1.0"
  "re_toy 32 1.0"
  "re_toy 64 1.0"
  "re_toy 128 1.0"
  "re_toy 256 1.0"
  "re_toy 512 1.0"
  "re_toy 1024 1.0"
  "re_toy 8 1.2"
  "re_toy 16 1.2"
  "re_toy 32 1.2"
  "re_toy 64 1.2"
  "re_toy 128 1.2"
  "re_toy 256 1.2"
  "re_toy 512 1.2"
  "re_toy 1024 1.2"
  "re_toy 8 2.0"
  "re_toy 16 2.0"
  "re_toy 32 2.0"
  "re_toy 64 2.0"
  "re_toy 128 2.0"
  "re_toy 256 2.0"
  "re_toy 512 2.0"
  "re_toy 1024 2.0"
  "re_toy 8 5.0"
  "re_toy 16 5.0"
  "re_toy 32 5.0"
  "re_toy 64 5.0"
  "re_toy 128 5.0"
  "re_toy 256 5.0"
  "re_toy 512 5.0"
  "re_toy 1024 5.0"
  "re_toy 8 20.0"
  "re_toy 16 20.0"
  "re_toy 32 20.0"
  "re_toy 64 20.0"
  "re_toy 128 20.0"
  "re_toy 256 20.0"
  "re_toy 512 20.0"
  "re_toy 1024 20.0"
  "re_toy 8 50.0"
  "re_toy 16 50.0"
  "re_toy 32 50.0"
  "re_toy 64 50.0"
  "re_toy 128 50.0"
  "re_toy 256 50.0"
  "re_toy 512 50.0"
  "re_toy 1024 50.0"
  "re_toy 8 500.0"
  "re_toy 16 500.0"
  "re_toy 32 500.0"
  "re_toy 64 500.0"
  "re_toy 128 500.0"
  "re_toy 256 500.0"
  "re_toy 512 500.0"
  "re_toy 1024 500.0"
  "re_toy 8 5000.0"
  "re_toy 16 5000.0"
  "re_toy 32 5000.0"
  "re_toy 64 5000.0"
  "re_toy 128 5000.0"
  "re_toy 256 5000.0"
  "re_toy 512 5000.0"
  "re_toy 1024 5000.0"
)

# main tests
echo "Experiment 1: parameter starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    for ((seed=1; seed<=seed_cnt; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/parameter.csv"
      eval "${test_command}"
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
          echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
      fi
    done
  done
done
echo -e "\033[0G\033[2KExperiment 1: parameter done."


######################################## Experiment 2: tradeoff ########################################

# input graphs
graphs=(
  "--random 1000 10"
)

# algorithms to be tested
algorithms=(
# RE-toy
  "re_toy 8 20.0"
  "re_toy 16 20.0"
  "re_toy 32 20.0"
  "re_toy 64 20.0"
  "re_toy 96 20.0"
  "re_toy 128 20.0"
  "re_toy 192 20.0"
  "re_toy 256 20.0"
  "re_toy 512 20.0"
  "re_toy 1024 20.0"
# BFL
  "bfl 1"
  "bfl 2"
  "bfl 3"
  "bfl 5"
  "bfl 8"
  "bfl 10"
  "bfl 15"
  "bfl 20"
  "bfl 25"
  "bfl 30"
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
  "ferrari 3 32 1"
  "ferrari 4 32 1"
  "ferrari 5 32 1"
  "ferrari 6 32 1"
  "ferrari 8 32 1"
  "ferrari 10 32 1"
  "ferrari 2 32 0"
  "ferrari 3 32 0"
  "ferrari 4 32 0"
  "ferrari 5 32 0"
  "ferrari 6 32 0"
  "ferrari 8 32 0"
  "ferrari 10 32 0"
# IP
  "ip 2 2 100"
  "ip 3 3 100"
  "ip 4 4 100"
  "ip 5 5 100"
  "ip 6 6 100"
  "ip 8 8 100"
  "ip 10 10 100"
# PLL
  "pll 1"
  "pll 0"
)

# main tests
echo "Experiment 2: tradeoff starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    for ((seed=1; seed<=seed_cnt; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/tradeoff.csv"
      eval "${test_command}"
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
          echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
      fi
    done
  done
done
echo -e "\033[0G\033[2KExperiment 2: tradeoff done."


######################################## Experiment 3: real_graph ########################################

# input graphs
graphs=(
  "--file ${input_directory}/watt__1.txt"
  "--file ${input_directory}/ash219.txt"
  "--file ${input_directory}/wm3.txt"
  "--file ${input_directory}/bcsstk09.txt"
  "--file ${input_directory}/dwt__193.txt"
  "--file ${input_directory}/bcsstk04.txt"
  "--file ${input_directory}/BA-1_10_60-L5.txt"
  "--file ${input_directory}/c-fat500-10.txt"
  "--file ${input_directory}/scc_infect-hyper.txt"
  "--complete 10000"
)

# algorithms to be tested
algorithms=(
# RE-toy
  "re_toy 8 20.0"
  "re_toy 12 20.0"
  "re_toy 16 20.0"
  "re_toy 24 20.0"
  "re_toy 32 20.0"
  "re_toy 48 20.0"
  "re_toy 64 20.0"
  "re_toy 96 20.0"
  "re_toy 128 20.0"
  "re_toy 192 20.0"
  "re_toy 256 20.0"
  "re_toy 384 20.0"
  "re_toy 512 20.0"
  "re_toy 768 20.0"
  "re_toy 1024 20.0"
  "re_toy 1536 20.0"
  "re_toy 2048 20.0"
  "re_toy 3072 20.0"
  "re_toy 4096 20.0"
  "re_toy 6144 20.0"
  "re_toy 8192 20.0"
# BFL
  "bfl 1"
  "bfl 2"
  "bfl 3"
  "bfl 5"
  "bfl 8"
  "bfl 10"
  "bfl 12"
  "bfl 15"
  "bfl 18"
  "bfl 20"
  "bfl 22"
  "bfl 25"
  "bfl 28"
  "bfl 30"
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
  "ferrari 3 32 1"
  "ferrari 4 32 1"
  "ferrari 5 32 1"
  "ferrari 6 32 1"
  "ferrari 8 32 1"
  "ferrari 10 32 1"
  "ferrari 2 32 0"
  "ferrari 3 32 0"
  "ferrari 4 32 0"
  "ferrari 5 32 0"
  "ferrari 6 32 0"
  "ferrari 8 32 0"
  "ferrari 10 32 0"
# IP
  "ip 2 2 100"
  "ip 3 3 100"
  "ip 4 4 100"
  "ip 5 5 100"
  "ip 6 6 100"
  "ip 8 8 100"
  "ip 10 10 100"
# PLL
  "pll 1"
  "pll 0"
)

# main tests
echo "Experiment 3: real_graph starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/real_graph.csv"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
echo -e "\033[0G\033[2KExperiment 3: real_graph done."


######################################## Experiment 4: dense_up ########################################

# input graphs
graphs1=(
  "--random 1024 1"
)

graphs2=(
  "--random 1024 2"
)

graphs3=(
  "--random 1024 4"
  "--random 1024 8"
  "--random 1024 16"
  "--random 1024 32"
  "--random 1024 64"
  "--random 1024 128"
  "--random 1024 256"
)

complete_graphs=(
  "--complete 1024"
)

# algorithms to be tested
algorithms1=(
  "re_toy 32 20.0"
  "bfl 2"
  "grail -2 1 2"
  "pathtree 2"
  "tol 0 2"
  "ferrari 2 32 1"
  "ip 2 2 100"
  "pll 1"
)

algorithms2=(
  "re_toy 32 20.0"
  "bfl 5"
  "grail -2 1 2"
  "pathtree 2"
  "tol 0 2"
  "ferrari 2 32 1"
  "ip 5 5 100"
  "pll 1"
)

algorithms3=(
  "re_toy 32 20.0"
  "bfl 5"
  "grail -2 1 5"
  "pathtree 2"
  "tol 0 2"
  "ferrari 5 32 1"
  "ip 5 5 100"
  "pll 1"
)

# main tests
echo "Experiment 4: dense_up starts."

for algorithm in "${algorithms1[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs1[@]}"
  do
    for ((seed=1; seed<=seed_cnt; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/dense_up.csv"
      eval "${test_command}"
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
          echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
      fi
    done
  done
done

for algorithm in "${algorithms2[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs2[@]}"
  do
    for ((seed=1; seed<=seed_cnt; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/dense_up.csv"
      eval "${test_command}"
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
          echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
      fi
    done
  done
done

for algorithm in "${algorithms3[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs3[@]}"
  do
    for ((seed=1; seed<=seed_cnt; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/dense_up.csv"
      eval "${test_command}"
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
          echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
      fi
    done
  done
done

for algorithm in "${algorithms3[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${complete_graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/dense_up.csv"
    eval "${test_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo -e "\033[2K\rTimeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done

echo -e "\033[0G\033[2KExperiment 4: dense_up done."
