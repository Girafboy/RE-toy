#!/bin/bash

# This script automatically runs five experiments mentioned in the paper, namely parameter, tradeoff, real_graph, dense_up, and index_size.

# config

# path to the executable file
executable="./build/reachability"

# common parent directory of graph files
input_directory="./graphs/"

# path to the output directory of all the results of the five experiments
# results will be stored in parameter.csv, tradeoff.csv, real_graph.csv, dense_up.csv, and index_size, respectively
output_dir="./output"

# test number of query pairs in each experiment
query_num="100000"

# maximum execution time for each single test in seconds
max_time="86400"

# number of seeds for each random graph
seed_cnt=100
seed_cnt_index_size=5


# create directories and clear old outputs
tasks=("parameter" "tradeoff" "real_graph" "dense_up" "index_size")
mkdir -p "${output_dir}"
for task in "${tasks[@]}"
do
    echo "algorithm,graph,params,index(B),query_num,construction(ns),query_mean(ns)" > "${output_dir}/${task}.csv"
done

# convert relative paths to absolute paths
input_directory=$(readlink -f "${input_directory}")
output_dir=$(readlink -f "${output_dir}")

# parallel
rv_command_array=()
rv_i=0
error_log_file="./output/error.txt"
rv_max_num_of_concurrent_tasks=3


######################################## Experiment 1: parameter ########################################

# input graphs
graphs=(
  "--random 1000 20"
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
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
    done
  done
done
echo -e "\033[0G\033[2KExperiment 1: parameter done."


######################################## Experiment 2: tradeoff ########################################

# input graphs
graphs=(
  "--random 1000 20"
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
# O'Reach
  "oreach"
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
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
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
#  "--file ${input_directory}/twitter_combined.txt"
#  "--file ${input_directory}/amazon0505.txt"
#  "--file ${input_directory}/wiki-topcats_20-core.txt"
#  "--file ${input_directory}/soc-pokec-relationships_40-core.txt"
#  "--file ${input_directory}/soc-LiveJournal1_50-core.txt"
#  "--file ${input_directory}/mag_70-core.txt"
)

# algorithms to be tested
algorithms=(
# RE-toy
  "re_toy 32 20.0"
  "re_toy 64 20.0"
  "re_toy 128 2.0"
  "re_toy 256 2.0"
  "re_toy 512 2.0"
  "re_toy 1024 2.0"
  "re_toy 2048 2.0"
  "re_toy 4096 2.0"
  "re_toy 8192 2.0"
  "re_toy 16384 2.0"
  "re_toy 32768 2.0"
  "re_toy 65536 2.0"
  "re_toy 131072 2.0"
  "re_toy 262144 2.0"
  "re_toy 524288 2.0"
# BFL
  "bfl 1"
  "bfl 2"
  "bfl 3"
  "bfl 5"
  "bfl 10"
  "bfl 15"
  "bfl 20"
  "bfl 30"
# Grail
  "grail 1 0 2"
  "grail 1 0 3"
  "grail 1 0 4"
  "grail 1 0 5"
  "grail 1 1 5"
  "grail -2 0 5"
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
# O'Reach
  "oreach"
)

# main tests
echo "Experiment 3: real_graph starts."
for algorithm in "${algorithms[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs[@]}"
  do
    test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/real_graph.csv"
    rv_command_array[${rv_i}]="${test_command}"
    rv_i=$((rv_i + 1))
  done
done
echo -e "\033[0G\033[2KExperiment 3: real_graph done."


######################################## Experiment 4: dense_up ########################################

# input graphs
graphs1=(
  "--random 1024 2"
)

graphs2=(
  "--random 1024 4"
  "--random 1024 8"
  "--random 1024 16"
  "--random 1024 32"
  "--random 1024 64"
  "--random 1024 128"
  "--random 1024 256"
  "--random 1024 512"
)

# algorithms to be tested
algorithms1=(
  "re_toy 32 20.0"
  "bfl 5"
  "grail -2 1 2"
  "pathtree 2"
  "tol 0 2"
  "ferrari 2 32 1"
  "ip 5 5 100"
  "pll 1"
  "oreach"
)

algorithms2=(
  "re_toy 32 20.0"
  "bfl 5"
  "grail -2 1 5"
  "pathtree 2"
  "tol 0 2"
  "ferrari 5 32 1"
  "ip 5 5 100"
  "pll 1"
  "oreach"
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
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
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
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
    done
  done
done

echo -e "\033[0G\033[2KExperiment 4: dense_up done."


######################################## Experiment 5: index_size ########################################

# input graphs
graphs1=(
  "--random 8 1"
  "--random 32 1"
  "--random 128 1"
  "--random 512 1"
  "--random 2048 1"
  "--random 8192 1"
  "--random 32768 1"
  "--random 131072 1"
)

graphs2=(
  "--random 8 2"
)

graphs3=(
  "--random 8 4"
  "--random 32 4"
  "--random 32 16"
  "--random 128 8"
  "--random 128 64"
  "--random 512 16"
  "--random 512 256"
  "--random 2048 32"
  "--random 2048 1024"
  "--random 8192 64"
  "--random 8192 4096"
  "--random 32768 128"
  "--random 32768 16384"
  "--random 131072 256"
  "--random 131072 65536"
)

# algorithms to be tested
algorithms1=(
  "re_toy 1000000 20.0"
  "bfl 2"
  "grail -2 1 2"
  "pathtree 2"
  "tol 0 2"
  "ferrari 2 32 1"
  "ip 2 2 100"
  "pll 1"
  "oreach"
)

algorithms2=(
  "re_toy 1000000 20.0"
  "bfl 5"
  "grail -2 1 2"
  "pathtree 2"
  "tol 0 2"
  "ferrari 2 32 1"
  "ip 5 5 100"
  "pll 1"
  "oreach"
)

algorithms3=(
  "re_toy 1000000 20.0"
  "bfl 5"
  "grail -2 1 5"
  "pathtree 2"
  "tol 0 2"
  "ferrari 5 32 1"
  "ip 5 5 100"
  "pll 1"
  "oreach"
)


# main tests
echo "Experiment 5: index_size starts."

for algorithm in "${algorithms1[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs1[@]}"
  do
    for ((seed=1; seed<=seed_cnt_index_size; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/index_size.csv"
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
    done
  done
done

for algorithm in "${algorithms2[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs2[@]}"
  do
    for ((seed=1; seed<=seed_cnt_index_size; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/index_size.csv"
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
    done
  done
done

for algorithm in "${algorithms3[@]}"
do
  echo -ne "\033[2K\rTesting ${algorithm}..."
  for graph in "${graphs3[@]}"
  do
    for ((seed=1; seed<=seed_cnt_index_size; seed++))
    do
      test_command="timeout ${max_time} ${executable} --time ${max_time} --graph ${graph} --seed ${seed} --algorithm ${algorithm} --query_num ${query_num} --result_file ${output_dir}/index_size.csv"
      rv_command_array[${rv_i}]="${test_command}"
      rv_i=$((rv_i + 1))
    done
  done
done

echo -e "\033[0G\033[2KExperiment 5: index_size done."


######################################## Parallel ########################################

rv_temp_fifo_file="/tmp/fifo_${$}.tmp"; mkfifo ${rv_temp_fifo_file}
exec {MYFD}<>${rv_temp_fifo_file}
rm -f ${rv_temp_fifo_file}

for ((rv_i=0; rv_i<rv_max_num_of_concurrent_tasks; ++rv_i))
do
    echo >&${MYFD}
done

rv_command_num=${#rv_command_array[@]}
for ((rv_i=0; rv_i<rv_command_num; ++rv_i))
do
    read -u ${MYFD}
    {
        ${rv_command_array[${rv_i}]} &&
        {
            echo "Subprocess execution complete."
        } ||
        {
            echo "Subprocess execution error."
            echo "${rv_command_array[${rv_i}]}" >> "${error_log_file}"
        }
        echo >&${MYFD}
    } &
done
wait

exec {MYFD}>&-