import networkx as nx
import math
import os
import sys


def read_graph_from_file(path: str) -> nx.DiGraph:
    d = {}
    with open(path, 'r') as f:
        for line in f:
            l = line.split()
            d[int(l[0][:-1])] = [int(x) for x in l[1:]]
    G_original = nx.DiGraph(d)
    scc_map = [0] * G_original.number_of_nodes()
    G_new = nx.DiGraph()
    for i, scc in enumerate(nx.strongly_connected_components(G_original)):
        G_new.add_node(i)
        for u in scc:
            scc_map[u] = i
    for u, v in G_original.edges:
        if scc_map[u] != scc_map[v]:
            G_new.add_edge(scc_map[u], scc_map[v])
    return G_new


def get_factor(n: int, m: int) -> float:
    try:
        return math.log(m, n)
    except ValueError:
        return -1


def output_graph_statistics_under_dir(dir: str) -> None:
    output_path = "../output/all_graph_statistics.csv"
    with open(output_path, 'w') as f:
        f.write("path,graph,n,m,factor\n")
    # exclude = {"brain_human", "Kronecker", "large_real"}
    for root, dirs, files in os.walk(dir):
        # dirs[:] = [d for d in dirs if d not in exclude]
        for file in files:
            if not file.endswith(".txt"):
                continue
            full_path = os.path.join(root, file)
            if os.path.getsize(full_path) > 16 * 1024 * 1024:  # 16MB
                continue
            print(f"Calculating {full_path}...")
            G = read_graph_from_file(full_path)
            with open(output_path, 'a') as f:
                f.write(f"{full_path},{file[:-4]},{G.number_of_nodes()},{G.number_of_edges()},{get_factor(G.number_of_nodes(),G.number_of_edges())}\n")


def get_graph_n_and_m_of_file(path: str) -> dict:
    G = read_graph_from_file(path)
    return {'n': G.number_of_nodes(), 'm': G.number_of_edges()}


if __name__ == '__main__':
    graph_arguments = sys.argv[1].split()
    if len(graph_arguments) == 3 and graph_arguments[0] == "--random":
        n = int(graph_arguments[1])
        m = int(graph_arguments[2]) * n
        graph_name = f"random-{graph_arguments[1]}-{graph_arguments[2]}"
    elif len(graph_arguments) == 2 and graph_arguments[0] == "--file" and graph_arguments[1].endswith(".txt"):
        nm = get_graph_n_and_m_of_file(graph_arguments[1])
        n = nm['n']
        m = nm['m']
        graph_name = os.path.basename(graph_arguments[1])[:-4]
    else:
        print(f"Illegal graph argument: {sys.argv[1]}")
        exit()
    with open("../output/graph.csv", 'a') as f:
        f.write(f"{graph_name},{n},{m},{get_factor(n, m)}\n")
