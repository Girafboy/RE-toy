#!/bin/bash

cd ./cmake-build-release || exit
input_directory="/Users/xingliwang/Documents/研究生/研一/研一夏/实验室/亢虎权/图/converted_graphs"
output_file="../output/result.csv"
max_time="1000"  # second

echo "algorithm,graph,params,construction(ns),index(B),query_num,query_mean(ns),query_samples" > ${output_file}

graphs=(
# random graphs
# "--random 1000 10"
 "--random 10000 10"
# "--random 100000 10"
# graphs from files
# small real sparse
# "--file ${input_directory}/small_real_sparse/human_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/anthra_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/amaze_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/agrocyc_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/xmark_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/ecoo_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/nasa_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/kegg_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/vchocyc_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/mtbrv_dag_uniq.txt"
# small real dense
# "--file ${input_directory}/small_real_dense/go_sub_6793.txt"
# "--file ${input_directory}/small_real_dense/citeseer_sub_10720.txt"
# "--file ${input_directory}/small_real_dense/arXiv_sub_6000-1.txt"
# "--file ${input_directory}/small_real_dense/pubmed_sub_9000-1.txt"
# "--file ${input_directory}/small_real_dense/yago_sub_6642.txt"
# large real
#"--file ${input_directory}/large_real/citeseer.scc.txt"
#"--file ${input_directory}/large_real/uniprotenc_22m.scc.txt"
#"--file ${input_directory}/large_real/cit-Patents.scc.txt"
#"--file ${input_directory}/large_real/citeseerx.txt"
#"--file ${input_directory}/large_real/uniprotenc_100m.scc.txt"
#"--file ${input_directory}/large_real/go_uniprot.txt"
#"--file ${input_directory}/large_real/uniprotenc_150m.scc.txt"
)

algorithms=(
# XJump
#"xjump 32"
#"xjump 64"
#"xjump 128"
#"xjump 256"
#"xjump 512"
#"xjump 1024"
#"xjump 2048"
#"xjump 4096"
# BFL
"bfl 1"
#"bfl 2"
#"bfl 5"
#"bfl 10"
#"bfl 20"
#"bfl 50"
#"bfl 100"
#"bfl 200"
#"bfl 500"
# Grail
#"grail 1 0 2"
#"grail 1 0 3"
#"grail 1 0 4"
#"grail 1 0 5"
#"grail 1 1 2"
#"grail 1 1 3"
#"grail 1 1 4"
#"grail 1 1 5"
#"grail -2 0 2"
#"grail -2 0 3"
#"grail -2 0 4"
#"grail -2 0 5"
#"grail -2 1 2"
#"grail -2 1 3"
#"grail -2 1 4"
#"grail -2 1 5"
# PathTree
#"pathtree 1"
#"pathtree 2"
#"pathtree 3"
#"pathtree 4"
# TOL
#"tol 0 1"
#"tol 0 2"
#"tol 1 2"
# Gripp
#"gripp"
# Ferrari
"ferrari 2 32 1"
# IP
# "ip 2 2 100"
# PLL
"pll 1"
"pll 0"
# PReaCH
"preach"
# DBL
"dbl"
)

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
