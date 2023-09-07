#include "PathTree.h"

namespace path_tree {
    PathTree::PathTree(PathTreeGraph &graph) : g(graph) {
        int maxid = g.num_vertices();
        labels = new int *[maxid];
        for (int i = 0; i < maxid; i++)
            labels[i] = new int[3];
        ng = PathTreeGraph(maxid);
        nextVertex = vector<int>(maxid);
        out_uncover = vector<set<int> >(maxid);
        for (int i = 0; i < maxid; i++) {
            ng.addVertex(i);
            nextVertex[i] = -1;
            out_uncover[i] = set<int>();
        }
    }

    PathTree::PathTree(PathTreeGraph &graph, vector<int> ts) : g(graph) {
        grts = ts;
        int maxid = g.num_vertices();
        labels = new int *[maxid];
        for (int i = 0; i < maxid; i++)
            labels[i] = new int[3];
        ng = PathTreeGraph(maxid);
        nextVertex = vector<int>(maxid);
        out_uncover = vector<set<int> >(maxid);
        for (int i = 0; i < maxid; i++) {
            ng.addVertex(i);
            nextVertex[i] = -1;
            out_uncover[i] = set<int>();
        }
    }

    PathTree::~PathTree() {
        for (int i = 0; i < g.num_vertices(); i++)
            delete[]labels[i];
        delete[]labels;
    }

    void PathTree::compute_tcm() {
#ifdef _TEST_
        PathTreeGraph tc(g.num_vertices());
        PathTreeGraphUtil::transitive_closure(g, tc);
        PathTreeEdgeList el;
        PathTreeEdgeList::iterator eit;
        for (int i = 0; i < tc.num_vertices(); i++) {
            el = tc.out_edges(i);
            for (eit = el.begin(); eit != el.end(); eit++)
                tcm[make_pair(i, *eit)] = true;
        }
        // for test
/*
	cout << "=======================TC===================" << endl;
	tc.printGraph();
*/
#endif
    }

    void PathTree::index_size(int *ind_size) {
        std::size_t isize = 0;
        int uncover_size = 0;
        vector<set<int> >::iterator mit;
        for (mit = out_uncover.begin(); mit != out_uncover.end(); mit++) {
            uncover_size += mit->size();
        }
        isize += uncover_size;
        isize += g.num_vertices();
        // labels
        isize += g.num_vertices() * 3;
//        isize += pathMap.size() * 2;
/*	
	cout << "uncover set rate: " << uncover_size*1.00/(isize*1.00) << endl; 
	cout << "path tree cover set size percentage: " << pathMap.size()*2.00/(isize*1.00) << endl;
*/
        ind_size[0] = isize; //total size
        ind_size[1] = uncover_size; //transitive closure size
/*
	int psize = 0;
	for (int i = 0; i < pathMap.size(); i++)
		psize += pathMap[i].size()-1;
	cout << "path cover edge size: " << psize << endl;
*/
    }

    void PathTree::transform(DWGraph branch, PathTreeGraph &graph) {
        DWVertexList dwvl = branch.vertices();
        DWVertexList::iterator dwvit;
        DWEdgeList dwel;
        DWEdgeList::iterator dweit;
        for (int i = 0; i < branch.num_vertices(); i++)
            graph.addVertex(i);
        for (dwvit = dwvl.begin(); dwvit != dwvl.end(); dwvit++) {
            dwel = branch.out_edges(dwvit->first);
            for (dweit = dwel.begin(); dweit != dwel.end(); dweit++) {
                graph.addEdge(dwvit->first, branch.target(*dweit));
            }
        }
/*
	cout << "------------------------------------" <<endl;
	graph.printGraph();
*/
    }

    void PathTree::buildWeightPathGraph_Pred() {
        int gs = g.num_vertices();
        // perform path decomposition
//	PathTreeGraphUtil::pathDecomposition(g, pathMap);
        PathTreeGraph tree(gs);
        for (int i = 0; i < gs; i++)
            tree.addVertex(i);

        gettimeofday(&before_time, NULL);
        vector<set<int> > predMap(g.num_vertices(), set<int>());
        PathTreeGraphUtil::findTreeCover(g, tree, predMap, grts);
        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "find tree cover time:" << run_time << " (ms)" << endl;

        gettimeofday(&before_time, NULL);
        PathTreeGraphUtil::treePathDecomposition(tree, g, pathMap);

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "path decomposition time:" << run_time << " (ms)" << endl;

        // build weight path graph
        hash_map<int, int> fastMap;
        hash_map<int, int> weightMap;
        hash_map<int, int>::iterator hmit;
        vector<vector<int> >::iterator mit;
        vector<int> path;
        vector<int>::iterator lit;
        PathTreeEdgeList el;
        PathTreeEdgeList::iterator eit;
        vector<int> vec;
        vector<int>::iterator v_end;
        int edgeid = 0;
        int index, i, k, pre, addval;
        int gsize = pathMap.size() + 10;

        for (mit = pathMap.begin(), k = 0; mit != pathMap.end(); mit++, k++) {
            pg.addVertex(k);
            path = (*mit);
            for (lit = path.begin(), i = 0; lit != path.end(); lit++, i++) {
                el = g.in_edges(*lit);
                if (i == 0) {
                    for (eit = el.begin(); eit != el.end(); eit++) {
                        if (g[*eit].path_id == k) continue;
                        index = g[*eit].path_id * gsize + k;
                        weightMap[index] += predMap[*eit].size();
                        hmit = fastMap.find(index);
                        if (hmit != fastMap.end()) {
                            //		pg.addEdge(g[*eit].path_id, mit->first, weightMap[index], hmit->second);
                            pg.addEdge(g[*eit].path_id, k, weightMap[index], hmit->second);
                            //	pg.addEdge(g[*eit].path_id, mit->first, kk++, hmit->second);
                        } else {
                            fastMap[index] = edgeid;
                            //		pg.addEdge(g[*eit].path_id, mit->first, weightMap[index], edgeid);
                            pg.addEdge(g[*eit].path_id, k, weightMap[index], edgeid);
                            //	pg.addEdge(g[*eit].path_id, mit->first, kk++, edgeid);
                            edgeid++;
                        }
                    }
                    pre = *lit;
                } else {
                    for (eit = el.begin(); eit != el.end(); eit++) {
                        index = g[*eit].path_id * gsize + k;
                        if (g[*eit].path_id != k) {
                            vec.clear();
                            set_difference(predMap[*eit].begin(), predMap[*eit].end(),
                                           predMap[pre].begin(), predMap[pre].end(), back_inserter(vec));
                            addval = vec.size();
                            if (addval == 0) addval = 1;
                            weightMap[index] += addval;
                            hmit = fastMap.find(index);

                            if (hmit != fastMap.end()) {
                                //	pg.addEdge(g[*eit].path_id, mit->first, weightMap[index], hmit->second);
                                pg.addEdge(g[*eit].path_id, k, weightMap[index], hmit->second);
                                //	pg.addEdge(g[*eit].path_id, mit->first, kk++, hmit->second);
                            } else {
                                fastMap[index] = edgeid;
                                //	pg.addEdge(g[*eit].path_id, mit->first, weightMap[index], edgeid);
                                pg.addEdge(g[*eit].path_id, k, weightMap[index], edgeid);
                                //	pg.addEdge(g[*eit].path_id, mit->first, kk++, edgeid);
                                edgeid++;
                            }
                        }
                    }
                    pre = *lit;
                }
            }
        }
    }

// build weighted path graph
    void PathTree::buildWeightPathGraph() {
        // perform path decomposition
        PathTreeGraphUtil::pathDecomposition(g, pathMap, grts);
/*
	PathTreeGraph tree;
	PathTreeGraphUtil::findTreeCover(g, tree);
	PathTreeGraphUtil::treePathDecomposition(tree, g, pathMap);
*/
        // build weight path graph
        hash_map<int, int> fastMap;
        hash_map<int, int>::iterator hmit;
        vector<vector<int> >::iterator mit;
        vector<int> path;
        vector<int>::iterator lit;
        PathTreeEdgeList el;
        PathTreeEdgeList::iterator eit;
        int edgeid = 0;
        int depth, k = 0;
        int gsize = pathMap.size() + 10;

        for (mit = pathMap.begin(), k = 0; mit != pathMap.end(); mit++, k++) {
            pg.addVertex(k);
            depth = 1;
            path = (*mit);
            for (lit = path.begin(); lit != path.end(); lit++) {
                el = g.out_edges(*lit);
                for (eit = el.begin(); eit != el.end(); eit++) {
                    if (g[*eit].path_id != k) {
                        hmit = fastMap.find(k * gsize + g[*eit].path_id);
                        if (hmit != fastMap.end())
                            pg.addEdge(k, g[*eit].path_id, depth, hmit->second);
                            //	pg.addEdge(mit->first, g[*eit].path_id, depth, hmit->second);
                        else {
                            //	fastMap[mit->first*gsize+g[*eit].path_id] = edgeid;
                            fastMap[k * gsize + g[*eit].path_id] = edgeid;
                            //	pg.addEdge(mit->first, g[*eit].path_id, depth, edgeid);
                            pg.addEdge(k, g[*eit].path_id, depth, edgeid);
                            edgeid++;
                        }
                    }
                }
                depth++;
            }
        }
    }

// calculate minimal equivalent edgeset 
    void PathTree::buildEquGraph() {
        // add all vertices from original graph
        vector<vector<int> >::iterator mit;
        vector<int> path;
        vector<int>::iterator lit;
        int p, q;
        for (mit = pathMap.begin(); mit != pathMap.end(); mit++) {
            path = (*mit);
            if (path.size() == 1) continue;
            for (lit = path.begin(); lit != path.end();) {
                p = *lit;
                lit++;
                if (lit == path.end()) break;
                q = *lit;
                nextVertex[p] = q;
                ng.addEdge(p, q);
            }
        }

        PathTreeEdgeList el, el1;
        PathTreeEdgeList::iterator eit, eit1;
        int source_path_maxtopo = 0;
        int max_id;
        int max_topo_id = MIN_VAL;
        int depth;
        int gsize = newbranch.num_vertices();
        for (int i = 0; i < gsize; i++) {
            el = newbranch.out_edges(i);
            for (eit = el.begin(); eit != el.end(); eit++) {
                path = pathMap[*eit];
                source_path_maxtopo = MIN_VAL;
                for (lit = path.begin(); lit != path.end(); lit++) {
                    // find maximum topological id
                    max_id = -1;
                    max_topo_id = MIN_VAL;
                    el1 = g.in_edges(*lit);
                    for (eit1 = el1.begin(); eit1 != el1.end(); eit1++) {
                        if (g[*eit1].path_id == i && g[*eit1].topo_id > max_topo_id) {
                            max_id = *eit1;
                            max_topo_id = g[*eit1].topo_id;
                        }
                    }
                    if (max_id == -1 || max_topo_id <= source_path_maxtopo)
                        continue;
                    source_path_maxtopo = max_topo_id;
                    ng.addEdge(max_id, *lit);
                }
            }
        }
    }

    void PathTree::pathDFS(int vid, int &order, int &first_order, vector<bool> &visited) {
        visited[vid] = true;
        g[vid].first_visit = first_order;
        first_order++;
        if (nextVertex[vid] != -1) {
            if (!visited[nextVertex[vid]])
                pathDFS(nextVertex[vid], order, first_order, visited);
        }

        PathTreeEdgeList el = ng.out_edges(vid);
        PathTreeEdgeList::iterator eit;
        for (eit = el.begin(); eit != el.end(); eit++) {
            if (!visited[*eit])
                pathDFS(*eit, order, first_order, visited);
        }
        g[vid].dfs_order = order;
        order--;
    }

// type specify PTree-1 or PTree-2
    void PathTree::createLabels(int type) {
        struct timeval after_time1, before_time1;
        // build weighted path graph
//	cout << "building weighted path graph" << endl;
        gettimeofday(&before_time1, NULL);
        if (type == 1)
            buildWeightPathGraph_Pred();
        else
            buildWeightPathGraph();

        gettimeofday(&after_time1, NULL);
        run_time = (after_time1.tv_sec - before_time1.tv_sec) * 1000.0 +
                   (after_time1.tv_usec - before_time1.tv_usec) * 1.0 / 1000.0;
//	cout << "building weighted path graph time:" << run_time << " (ms)" << endl;

/*	
	pg.printGraph();
	cout << "path size " << pathMap.size() << endl;
*/
        // find maximum branching
//	cout << "finding max branching" << endl;
        gettimeofday(&before_time, NULL);
        if (type == 1)
            DWGraphUtil::findMaxBranching(pg, branch);
        else
            DWGraphUtil::findMaxBranching1(pg, branch);

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "finding max branching time:" << run_time << " (ms)" << endl;

/*
	cout << "find max branching" << endl;
	branch.printGraph();
*/
        // graph transform
        gettimeofday(&before_time, NULL);
        newbranch = PathTreeGraph(branch.num_vertices());
        transform(branch, newbranch);

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "weighted graph transformation time:" << run_time << " (ms)" << endl;
/*
	cout << "++++++++++++++++++++++++++++++++++++++++++++" << endl;
	newbranch.printGraph();
*/
        // calculate minimal equivalent EdgeSet
//	cout << "calculating equivalent edgeset" << endl;
        gettimeofday(&before_time, NULL);
        buildEquGraph();

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "building equivalent graph time:" << run_time << " (ms)" << endl;
/*
	cout << "equivalent graph" << endl;
	ng.printGraph();
*/
        // labeling branch by GRIPP's algorithm
//	cout << "labeling found max branching" << endl;
        gettimeofday(&before_time, NULL);
        PathTreeGraphUtil::pre_post_labeling(newbranch);

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "labeling branching time:" << run_time << " (ms)" << endl;
/*
	// for test
	cout << "pre post labeling" << endl;
	newbranch.printGraph();
*/

        // create labels for every vertex
//	cout << "labeling equivalent graph by depth-first-search" << endl;
        vector<int> reverse_topo_sort;
        PathTreeGraphUtil::topological_sort(newbranch, reverse_topo_sort);
        gettimeofday(&before_time, NULL);
        vector<int> path;
        vector<int>::iterator lit;
        vector<int>::iterator vit;
        vector<int>::reverse_iterator rit;
        int order = g.num_vertices();
        int first_order = 1;
        vector<bool> visited(order, false);
        for (rit = reverse_topo_sort.rbegin(); rit != reverse_topo_sort.rend(); rit++) {
            path = pathMap[*rit];
            lit = path.begin();
            for (lit = path.begin(); lit != path.end(); lit++) {
                if (!visited[*lit])
                    pathDFS(*lit, order, first_order, visited);
            }
        }

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "labeling equivalent graph by DFS time:" << run_time << " (ms)" << endl;

        // update label vector
        int gsize = g.num_vertices();
        for (int i = 0; i < gsize; i++) {
            labels[i][0] = newbranch[g[i].path_id].pre_order;
            labels[i][1] = newbranch[g[i].path_id].post_order;
            labels[i][2] = g[i].dfs_order;
        }

        // handling edges not covered
//	cout << "collecting path-tree uncovered vertices" << endl;
        gettimeofday(&before_time, NULL);
//	reverse_topo_sort = vector<int>();
//	PathTreeGraphUtil::topological_sort(g, reverse_topo_sort);
        PathTreeEdgeList el;
        PathTreeEdgeList::iterator eit;
        int pre1, post1, pre2, post2;
        for (vit = grts.begin(); vit != grts.end(); vit++) {
            el = g.out_edges(*vit);
            pre1 = labels[*vit][0];
            post1 = labels[*vit][1];
            for (eit = el.begin(); eit != el.end(); eit++) {
                insertSet(out_uncover[*vit], out_uncover[*eit]);
                if (labels[*vit][2] <= labels[*eit][2] && labels[*eit][0] >= pre1 && labels[*eit][1] <= post1)
                    continue;
                set<int> temp;
                temp.insert(*eit);
                insertSet(out_uncover[*vit], temp);
            }
        }

        gettimeofday(&after_time, NULL);
        run_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                   (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//	cout << "computing uncover set time:" << run_time << " (ms)" << endl;

    }

    void PathTree::insertSet(set<int> &s1, set<int> &s2) {
        set<int>::iterator sit1, sit2;
        bool insert;
        int pre1, post1, pre2, post2;
        for (sit2 = s2.begin(); sit2 != s2.end(); sit2++) {
            insert = true;
            pre2 = labels[*sit2][0];
            post2 = labels[*sit2][1];
            for (sit1 = s1.begin(); sit1 != s1.end();) {
                pre1 = labels[*sit1][0];
                post1 = labels[*sit1][1];
                if (pre2 >= pre1 && post2 <= post1 && labels[*sit1][2] < labels[*sit2][2]) {
                    insert = false;
                    break;
                } else if (pre2 <= pre1 && post2 >= post1 && labels[*sit1][2] > labels[*sit2][2]) {
                    // todo: 检查，我修改了这里
                    sit1 = s1.erase(sit1);
//				s1.erase(*sit1);
//				sit1++;
                } else
                    sit1++;
            }
            if (insert) s1.insert(*sit2);
        }
    }

    void PathTree::displayLabels() {
    }

    bool PathTree::reach(int src, int trg) {
        int pre1, post1, pre2, post2;
        pre1 = labels[src][0];
        post1 = labels[src][1];
        pre2 = labels[trg][0];
        post2 = labels[trg][1];

        if (labels[src][2] <= labels[trg][2] && post2 >= pre1 && post2 <= post1)
            return true;

        set<int> si = out_uncover[src];
        set<int>::iterator sit;
        for (sit = si.begin(); sit != si.end(); sit++) {
            pre1 = labels[*sit][0];
            post1 = labels[*sit][1];
            if (labels[*sit][2] <= labels[trg][2] && post2 >= pre1 && post2 <= post1)
                return true;
        }

        return false;
    }

// for test
    bool PathTree::test_reach(int src, int trg) {
        bool r = reach(src, trg);
        if (r != tcm[make_pair(src, trg)]) {
            cout << "Wrong: [" << src << "] to [" << trg << "] reach = " << r << endl;
            return false;
        }

        return true;
    }
}