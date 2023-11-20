#!/bin/bash

cd ./build || cd ./cmake-build-debug || exit
#input_directory="/Users/xingliwang/Documents/研究生/研一/研一夏/实验室/亢虎权/图/converted_graphs"
input_directory="/home/kang/xingliwang/data/converted_graphs"
#input_directory="/home/kang/xingliwang/data/tc_graphs_grouped_by_factor"
output_file="../output/result.csv"
output_query_time_dir="../output/query_time"
max_time="1000"  # second

echo "algorithm,graph,params,construction(ns),index(B),query_num,query_mean(ns)" > ${output_file}
rm "${output_query_time_dir}"/*.txt

graphs=(
# random graphs
# "--random 32 10"
# "--random 64 10"
# "--random 128 10"
# "--random 256 10"
# "--random 512 10"
# "--random 1024 10"
# "--random 2048 10"
# "--random 4096 10"
# "--random 8192 10"
# "--random 16384 10"
# "--random 32768 10"
# "--random 65536 10"
# "--random 1000 10"
#  "--random 10000 10"
#  "--random 100000 10"
#  "--random 1000 50"
#  "--random 1000 100"
#  "--random 10000 50"
#  "--random 10000 100"
# "--random 10000 200"
# graphs from files
# large real
#"--file ${input_directory}/large_real/uniprotenc_100m.scc.txt"
#"--file ${input_directory}/large_real/uniprotenc_150m.scc.txt"
#"--file ${input_directory}/large_real/go_uniprot.txt"
#"--file ${input_directory}/large_real/citeseer.scc.txt"
#"--file ${input_directory}/large_real/cit-Patents.scc.txt"
#"--file ${input_directory}/large_real/uniprotenc_22m.scc.txt"
#"--file ${input_directory}/large_real/citeseerx.txt"
# small real dense
# "--file ${input_directory}/small_real_dense/go_sub_6793.txt"
# "--file ${input_directory}/small_real_dense/citeseer_sub_10720.txt"
# "--file ${input_directory}/small_real_dense/arXiv_sub_6000-1.txt"
# "--file ${input_directory}/small_real_dense/pubmed_sub_9000-1.txt"
# "--file ${input_directory}/small_real_dense/yago_sub_6642.txt"
# small real sparse
# "--file ${input_directory}/small_real_sparse/agrocyc_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/amaze_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/anthra_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/ecoo_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/kegg_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/vchocyc_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/xmark_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/mtbrv_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/nasa_dag_uniq.txt"
# "--file ${input_directory}/small_real_sparse/human_dag_uniq.txt"
# econ
#  "--file ${input_directory}/econ/econ-beacxc.txt"
#  "--file ${input_directory}/econ/econ-beaflw.txt"
#  "--file ${input_directory}/econ/econ-beause.txt"
#  "--file ${input_directory}/econ/econ-mahindas.txt"
#  "--file ${input_directory}/econ/econ-mbeacxc.txt"
#  "--file ${input_directory}/econ/econ-mbeaflw.txt"
#  "--file ${input_directory}/econ/econ-mbeause.txt"
#  "--file ${input_directory}/econ/econ-poli.txt"
#  "--file ${input_directory}/econ/econ-wm2.txt"
#  "--file ${input_directory}/econ/econ-wm1.txt"
#  "--file ${input_directory}/econ/econ-wm3.txt"
#  "--file ${input_directory}/econ/econ-orani678.txt"
#  "--file ${input_directory}/econ/econ-poli-large.txt"
#  "--file ${input_directory}/econ/econ-psmigr1.txt"
#  "--file ${input_directory}/econ/econ-psmigr3.txt"
#  "--file ${input_directory}/econ/econ-psmigr2.txt"
# Kronecker
# "--file ${input_directory}/Kronecker/kron_g500-simple-logn12.txt"
# "--file ${input_directory}/Kronecker/kron_g500-simple-logn16.txt"
# "--file ${input_directory}/Kronecker/kron_g500-simple-logn17.txt"
# web
#  "--file ${input_directory}/web/web-uk-2005.txt"
# email
#  "--file ${input_directory}/email/email-dnc.txt"
#  "--file ${input_directory}/email/email-dnc-corecipient.txt"
# knowledge graph
#  "--file ${input_directory}/knowledge_graph/FB15k-237.txt"
# higgs twitter
#"--file ${input_directory}/higgs_twitter/higgs-social_network.txt"
# brain
#  "--file ${input_directory}/brain/c.elegans_neural.male_1.txt"
#  "--file ${input_directory}/brain/c.elegans.herm_pharynx_1.txt"
#  "--file ${input_directory}/brain/mixed.species_brain_1.txt"
#  "--file ${input_directory}/brain/drosophila_medulla_1.txt"
#  "--file ${input_directory}/brain/mouse_brain_1.txt"
#  "--file ${input_directory}/brain/mouse_visual.cortex_2.txt"
#  "--file ${input_directory}/brain/p.pacificus_neural.synaptic_2.txt"
#  "--file ${input_directory}/brain/kasthuri_graph_v4.txt"
#  "--file ${input_directory}/brain/mouse_visual.cortex_1.txt"
#  "--file ${input_directory}/brain/p.pacificus_neural.synaptic_1.txt"
#  "--file ${input_directory}/brain/rattus.norvegicus_brain_1.txt"
#  "--file ${input_directory}/brain/rhesus_cerebral.cortex_1.txt"
#  "--file ${input_directory}/brain/rattus.norvegicus_brain_2.txt"
#  "--file ${input_directory}/brain/rhesus_brain_1.txt"
#  "--file ${input_directory}/brain/rhesus_brain_2.txt"
#  "--file ${input_directory}/brain/rattus.norvegicus_brain_3.txt"
#  "--file ${input_directory}/brain/rhesus_interareal.cortical.network_2.txt"
#  "--file ${input_directory}/brain/mouse_retina_1.txt"
# brain human
# "--file ${input_directory}/brain_human/bn-human-BNU_1_0025890_session_1.txt"
#"--file ${input_directory}/brain_human/bn-human-Jung2015_M87101234.txt"
#"--file ${input_directory}/brain_human/bn-human-Jung2015_M87119044.txt"
#"--file ${input_directory}/brain_human/bn-human-Jung2015_M87116523.txt"

# # grouped by factors
#  # 0
#  "--file ${input_directory}/0/scc_rt_obama.txt"
#  "--file ${input_directory}/0/scc_rt_lebanon.txt"
#  # 1
#  "--file ${input_directory}/1/scc_rt_libya.txt"
#  "--file ${input_directory}/1/scc_rt_tlot.txt"
#  "--file ${input_directory}/1/scc_rt_voteonedirection.txt"
#  "--file ${input_directory}/1/scc_rt_p2.txt"
#  "--file ${input_directory}/1/scc_rt_qatif.txt"
#  "--file ${input_directory}/1/scc_rt_ksa.txt"
#  "--file ${input_directory}/1/scc_rt_tcot.txt"
#  "--file ${input_directory}/1/scc_rt_uae.txt"
#  "--file ${input_directory}/1/scc_rt_israel.txt"
#  "--file ${input_directory}/1/scc_rt_oman.txt"
#  "--file ${input_directory}/1/scc_rt_http.txt"
#  "--file ${input_directory}/1/scc_rt_gop.txt"
#  # 2
#  "--file ${input_directory}/2/scc_rt_occupy.txt"
#  "--file ${input_directory}/2/scc_rt_assad.txt"
#  "--file ${input_directory}/2/scc_rt_mittromney.txt"
#  "--file ${input_directory}/2/scc_rt_saudi.txt"
#  "--file ${input_directory}/2/scc_rt_bahrain.txt"
#  "--file ${input_directory}/2/scc_rt_dash.txt"
#  "--file ${input_directory}/2/scc_rt_barackobama.txt"
#  "--file ${input_directory}/2/scc_rt_damascus.txt"
#  # 3
#  "--file ${input_directory}/3/scc_rt_justinbieber.txt"
#  "--file ${input_directory}/3/scc_rt_onedirection.txt"
#  # "--file ${input_directory}/3/scc_retweet-crawl.txt"
#  "--file ${input_directory}/3/scc_rt_gmanews.txt"
#  "--file ${input_directory}/3/scc_rt_alwefaq.txt"
#  # 4
#  # "--file ${input_directory}/4/citeseer.scc.txt"
#  "--file ${input_directory}/4/scc_rt_occupywallstnyc.txt"
#  "--file ${input_directory}/4/scc_rt_lolgop.txt"
#  # 5
#  "--file ${input_directory}/5/human_dag_uniq.txt"
#  "--file ${input_directory}/5/scc_retweet.txt"
#  "--file ${input_directory}/5/anthra_dag_uniq.txt"
#  "--file ${input_directory}/5/email-dnc.txt"
#  "--file ${input_directory}/5/amaze_dag_uniq.txt"
#  "--file ${input_directory}/5/go_sub_6793.txt"
#  "--file ${input_directory}/5/citeseer_sub_10720.txt"
#  "--file ${input_directory}/5/agrocyc_dag_uniq.txt"
#  "--file ${input_directory}/5/mouse_visual.cortex_1.txt"
#  "--file ${input_directory}/5/xmark_dag_uniq.txt"
#  "--file ${input_directory}/5/econ-poli.txt"
#  "--file ${input_directory}/5/mouse_visual.cortex_2.txt"
#  "--file ${input_directory}/5/ecoo_dag_uniq.txt"
#  "--file ${input_directory}/5/nasa_dag_uniq.txt"
#  "--file ${input_directory}/5/econ-poli-large.txt"
#  "--file ${input_directory}/5/kegg_dag_uniq.txt"
#  "--file ${input_directory}/5/vchocyc_dag_uniq.txt"
#  "--file ${input_directory}/5/mtbrv_dag_uniq.txt"
#  "--file ${input_directory}/5/pubmed_sub_9000-1.txt"
#  "--file ${input_directory}/5/kasthuri_graph_v4.txt"
#  # 6
#  "--file ${input_directory}/6/scc_infect-dublin.txt"
#  "--file ${input_directory}/6/soc-firm-hi-tech.txt"
#  "--file ${input_directory}/6/econ-mahindas.txt"
#  "--file ${input_directory}/6/arXiv_sub_6000-1.txt"
#  "--file ${input_directory}/6/email-dnc-corecipient.txt"
#  "--file ${input_directory}/6/FB15k-237.txt"
#  "--file ${input_directory}/6/p.pacificus_neural.synaptic_2.txt"
#  "--file ${input_directory}/6/drosophila_medulla_1.txt"
#  "--file ${input_directory}/6/p.pacificus_neural.synaptic_1.txt"
#  "--file ${input_directory}/6/yago_sub_6642.txt"
#  # 7
#  "--file ${input_directory}/7/c.elegans_neural.male_1.txt"
#  "--file ${input_directory}/7/econ-wm3.txt"
#  "--file ${input_directory}/7/rhesus_brain_1.txt"
#  "--file ${input_directory}/7/econ-wm2.txt"
#  "--file ${input_directory}/7/rhesus_brain_2.txt"
#  "--file ${input_directory}/7/econ-wm1.txt"
#  "--file ${input_directory}/7/c.elegans.herm_pharynx_1.txt"
#  "--file ${input_directory}/7/econ-orani678.txt"
#  # "--file ${input_directory}/7/scc_twitter-copen.txt"
#  # 8
#  "--file ${input_directory}/8/econ-beause.txt"
#  "--file ${input_directory}/8/scc_fb-messages.txt"
#  "--file ${input_directory}/8/econ-mbeause.txt"
#  "--file ${input_directory}/8/mixed.species_brain_1.txt"
#  "--file ${input_directory}/8/econ-psmigr3.txt"
#  "--file ${input_directory}/8/econ-psmigr2.txt"
#  "--file ${input_directory}/8/econ-beaflw.txt"
#  "--file ${input_directory}/8/econ-mbeacxc.txt"
#  "--file ${input_directory}/8/econ-psmigr1.txt"
#  "--file ${input_directory}/8/rhesus_interareal.cortical.network_2.txt"
#  "--file ${input_directory}/8/rattus.norvegicus_brain_3.txt"
#  "--file ${input_directory}/8/rhesus_cerebral.cortex_1.txt"
#  "--file ${input_directory}/8/rattus.norvegicus_brain_2.txt"
#  "--file ${input_directory}/8/mouse_retina_1.txt"
#  "--file ${input_directory}/8/rattus.norvegicus_brain_1.txt"
#  "--file ${input_directory}/8/econ-beacxc.txt"
#  "--file ${input_directory}/8/scc_fb-forum.txt"
#  "--file ${input_directory}/8/econ-mbeaflw.txt"
#  # 9
#  "--file ${input_directory}/9/scc_infect-hyper.txt"
#  "--file ${input_directory}/9/scc_enron-only.txt"
#  "--file ${input_directory}/9/mouse_brain_1.txt"
#  "--file ${input_directory}/9/moreno_sampson.txt"
#  "--file ${input_directory}/9/rgg010.txt"

## factor sample
# # 0
# "--file ${input_directory}/0/scc_rt_obama.txt"
# # 1
# "--file ${input_directory}/1/scc_rt_tcot.txt"
# # 2
# "--file ${input_directory}/2/scc_rt_mittromney.txt"
# # 3
# "--file ${input_directory}/3/scc_rt_alwefaq.txt"
# # 4
# "--file ${input_directory}/4/scc_rt_lolgop.txt"
# # 5
# "--file ${input_directory}/5/email-dnc.txt"
# # 6
# "--file ${input_directory}/6/arXiv_sub_6000-1.txt"
# # 7
# "--file ${input_directory}/7/c.elegans_neural.male_1.txt"
# # 8
# "--file ${input_directory}/8/econ-mbeause.txt"
# # 9
# "--file ${input_directory}/9/scc_enron-only.txt"

# factor after merging sample
"--file ${input_directory}/econ/econ-poli.txt"
"--file ${input_directory}/brain/mouse_visual.cortex_1.txt"
"--file ${input_directory}/small_real_dense/yago_sub_6642.txt"
"--file ${input_directory}/econ/econ-beaflw.txt"
"--file ${input_directory}/hb/bcsstm27.txt"
"--file ${input_directory}/hb/bcsstk04.txt"
"--file ${input_directory}/labeled/BA-1_10_60-L5.txt"
"--file ${input_directory}/dimacs/c-fat500-10.txt"
"--file ${input_directory}/tscc/scc_infect-hyper.txt"
"--file ${input_directory}/artificial/complete-1000.txt"
)

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
  # "orse_toy 32 1.2"
  # "orse_toy 32 1.4"
  # "orse_toy 32 1.6"
  # "orse_toy 32 1.8"
  # "orse_toy 32 2.0"
  # "orse_toy 32 2.2"
  # "orse_toy 32 2.4"
  # "orse_toy 32 2.6"
  # "orse_toy 32 2.8"
  # "orse_toy 32 3.0"
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
  "pathtree 3"
  "pathtree 4"
# TOL
  "tol 0 1"
  "tol 0 2"
  "tol 1 2"
# Gripp
  "gripp"
# Ferrari
  "ferrari 2 32 1"
  "ferrari 2 2 1"
  "ferrari 5 5 1"
  "ferrari 2 32 0"
  "ferrari 2 2 0"
  "ferrari 5 5 0"
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

for algorithm in "${algorithms[@]}"
do
  echo "Start testing ${algorithm}"
  for graph in "${graphs[@]}"
  do
    tmp_command="timeout ${max_time} ./reachability --time ${max_time} ${graph} --algorithm ${algorithm} --accuracy"
    eval "${tmp_command}"
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        echo "Timeout: Algorithm \"${algorithm}\" on graph \"${graph}\"."
    fi
  done
done
