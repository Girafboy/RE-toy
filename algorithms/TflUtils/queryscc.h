#ifndef TC_QUERYSCC_H
#define TC_QUERYSCC_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "runtimecounter.h"
#include <fstream>

namespace tfl_queryscc {
    using namespace ::std;

    const int SZ_INT=sizeof(int);
    const int SZ_PTR=sizeof(void*);
    const long int BLK_SZ = 4194304;  //1024*1024*4 sizeof blk for I/O
    const int SZ_VERTEX = sizeof(int);
    const int VERTEX_PER_BLK = BLK_SZ/SZ_VERTEX;
    int *scc,*dag,*topo,**TL,*tlsize;
    char* read_buff_1, *read_buff_2;
    char label_name[50];
    int tgraph=0,pt=0;
    FILE *sccf,*dagf,*topof,*TLf,*tlsizef;

    int tc_graph_size;

    static void usage() {
        cout << "\nUsage:\n"
                "	TF-Label [-scc sccNumber] [-h] [-index indexfile] [-q queryfile]  queryNum \n"
                "Description:\n"
                "	-h	Print the help message.\n"
                "	-scc	the number of SCC in DAG.\n"
                "	-i	Set the index filename .\n"
                "	-q	Set the query filename .\n"
                "	queryNum   the number of queries(default number is 1000000)\n"
             << endl;
    }

    void load(FILE *filename, int * label){
        int * buff = (int *) read_buff_1;

        int num_read=fread(buff,SZ_VERTEX,VERTEX_PER_BLK,filename);
        int ptr_buff = 0,ptr=0;
        while(!feof(filename) || ptr_buff!=num_read){
            label[ptr] = buff[ptr_buff];
            ptr_buff++,ptr++;
            if(ptr_buff==num_read){
                num_read = fread(buff,SZ_VERTEX,VERTEX_PER_BLK,filename);
                ptr_buff = 0;
            }
        }
    }
    void load_2(){
        int * buff = (int *) read_buff_1;

        int num_read=fread(buff,SZ_VERTEX,VERTEX_PER_BLK,TLf);
        int ptr1=0,ptr2=0,ptr_buff=0;
        while(!feof(TLf) || ptr_buff!=num_read){
            TL[ptr1][ptr2] = buff[ptr_buff];
            ptr_buff++,ptr2++;
            if(ptr2==tlsize[ptr1])
                ptr1++,ptr2=0;
            if(ptr_buff==num_read){
                num_read = fread(buff,SZ_VERTEX,VERTEX_PER_BLK,TLf);
                ptr_buff = 0;
            }
        }
    }

    void getLabelName(char* tmp){
        tgraph =pt;
        for(char *i=tmp; *i != '\0'; i++){
            label_name[tgraph++]=*i;
        }
        label_name[tgraph]='\0';
    }

    void before_queryscc(int sccN, string graph_name) {
        int queryNum=1000000,i=1;
//        char *ind_filename,*query_filename;
        string tmp_ind_file_name = "../tmp/TFL/"s + graph_name;
        const char *ind_filename = tmp_ind_file_name.c_str();
//        while (i < argc) {
//            if (strcmp("-h", argv[i]) == 0) {
//                usage();
//                return 1;
//            }
//            if (strcmp("-scc", argv[i]) == 0) {
//                i++;
//                sccN=atoi(argv[i++]);// vertex number(scc) after callapsing into super nodes
//            }
//            if (strcmp("-i", argv[i]) == 0) {
//                i++;
//                ind_filename = argv[i++];
//            }
//            if (strcmp("-q", argv[i]) == 0) {
//                i++;
//                query_filename = argv[i++];
//            }
//            else{
//                queryNum=atoi(argv[i++]);
//            }
//        }
        for(const char *i=ind_filename; *i != '\0'; i++){
            label_name[tgraph++]=*i;
        }
        pt=tgraph;
        char dag_l[20]="_dag_label",topo_l[20]="_topo_label",tlsize_l[20]="_tlstart",TL_l[20]="_TL";
        getLabelName(dag_l);
        dagf=fopen(label_name, "rb");   //dag file
        getLabelName(topo_l);
        topof=fopen(label_name, "rb");  //topological level file
        getLabelName(TL_l);
        TLf= fopen(label_name, "rb");  // label(TFhierarchy) file
        getLabelName(tlsize_l);
        tlsizef=fopen(label_name, "rb"); //each vertex's label size file


        dag=(int*)malloc(SZ_INT * sccN);   //dag
        topo=(int*)malloc(SZ_INT * sccN);  //topological level
        tlsize=(int*)malloc(SZ_INT * 2 * sccN); // each vertex's label size
        read_buff_1 = (char *)malloc(BLK_SZ);
        load(tlsizef,tlsize);  //load each vertex's label size
        load(dagf,dag);  //load dag
        load(topof,topo); //load topological level
        fclose(tlsizef);
        fclose(dagf);
        fclose(topof);


        TL=(int**)malloc(SZ_PTR * 2 * sccN);
        for (int i = 0; i < 2 * sccN; i++) {
            TL[i]=(int*)malloc(SZ_INT * tlsize[i]);
        }
        load_2();  //load label
        free(read_buff_1);
        read_buff_1 = nullptr;
        int vertexN = sccN;
        tc_graph_size = sccN;
        fclose(TLf);

//   query
//        int* s=(int*)malloc(SZ_INT*queryNum);
//        int* t=(int*)malloc(SZ_INT*queryNum);
//
//        FILE * query_file =fopen(query_filename, "r");
//
//        int ret;
//        for (int i = 0; i < queryNum; i++) {
//            ret=fscanf(query_file, "%d %d\n", &s[i], &t[i]);
//        }
    }

    bool query_queryscc(int p, int q) {
        int sccN = tc_graph_size;
        int m,n,i2,j2,size1,size2;
        int *intArray1, *intArray2;
        //	query(s, t);
//        p=s[i];
//        q=t[i];
        if (p == q) {
            return true;
        }
        if (dag[p] != dag[q]){    //after callapsing into super node, in the same connected DAG?
            return false;
        }
        else {
            if (topo[p] >= topo[q]){      //topo means the topo order
                return false;
            }
            else {
                i2 = j2 = 0;

                //SL should int** type
                intArray1=TL[p];
                intArray2=TL[q + sccN];

                //SLsize[i] is the size of SL[i]
                size1=tlsize[p];
                size2=tlsize[q + sccN];
                while ((i2 < size1) && (j2 < size2)) {
                    //SL[0~vertexN-1] store the out label of each vertex, SL[vertexN~2*vertexN-1] store the in label of each vertex

                    if(intArray1[i2] < intArray2[j2]){
                        ++i2;
                    }
                    else if(intArray1[i2] > intArray2[j2]){
                        ++j2;
                    }
                    else{//		if (intArray1[i2] == intArray2[j2])
                        return true;
                    }
                }

                if((i2 == size1) || (j2 == size2)){
                    return false;
                }
            }
        }
        return false;
    }

    void after_queryscc() {
        int sccN = tc_graph_size;
//        free(s);
//        free(t);

        for (int i = 0; i < 2 * sccN; i++) {
            free(TL[i]);
            TL[i] = nullptr;
        }
        free(TL);
        TL = nullptr;
        free(tlsize);
        tlsize = nullptr;
        //   free(scc);
        free(dag);
        dag = nullptr;
        free(topo);
        topo = nullptr;
        tgraph = 0;
        pt = 0;
        tc_graph_size = 0;
    }

//    int main_queryscc() {
//        int sccN, queryNum=1000000,i=1;
//        char *ind_filename,*query_filename;
//        while (i < argc) {
//            if (strcmp("-h", argv[i]) == 0) {
//                usage();
//                return 1;
//            }
//            if (strcmp("-scc", argv[i]) == 0) {
//                i++;
//                sccN=atoi(argv[i++]);// vertex number(scc) after callapsing into super nodes
//            }
//            if (strcmp("-i", argv[i]) == 0) {
//                i++;
//                ind_filename = argv[i++];
//            }
//            if (strcmp("-q", argv[i]) == 0) {
//                i++;
//                query_filename = argv[i++];
//            }
//            else{
//                queryNum=atoi(argv[i++]);
//            }
//        }
//        for(char *i=ind_filename; *i != '\0'; i++){
//            label_name[tgraph++]=*i;
//        }
//        pt=tgraph;
//        char dag_l[20]="_dag_label",topo_l[20]="_topo_label",tlsize_l[20]="_tlstart",TL_l[20]="_TL";
//        getLabelName(dag_l);
//        dagf=fopen(label_name, "rb");   //dag file
//        getLabelName(topo_l);
//        topof=fopen(label_name, "rb");  //topological level file
//        getLabelName(TL_l);
//        TLf= fopen(label_name, "rb");  // label(TFhierarchy) file
//        getLabelName(tlsize_l);
//        tlsizef=fopen(label_name, "rb"); //each vertex's label size file
//
//
//        dag=(int*)malloc(SZ_INT * sccN);   //dag
//        topo=(int*)malloc(SZ_INT * sccN);  //topological level
//        tlsize=(int*)malloc(SZ_INT * 2 * sccN); // each vertex's label size
//        read_buff_1 = (char *)malloc(BLK_SZ);
//        load(tlsizef,tlsize);  //load each vertex's label size
//        load(dagf,dag);  //load dag
//        load(topof,topo); //load topological level
//
//        TL=(int**)malloc(SZ_PTR * 2 * sccN);
//        for (int i = 0; i < 2 * sccN; i++) {
//            TL[i]=(int*)malloc(SZ_INT * tlsize[i]);
//        }
//        load_2();  //load label
//        free(read_buff_1);
//        int vertexN = sccN;
//
////   query
//        int* s=(int*)malloc(SZ_INT*queryNum);
//        int* t=(int*)malloc(SZ_INT*queryNum);
//
//        FILE * query_file =fopen(query_filename, "r");
//
//        int ret;
//        for (int i = 0; i < queryNum; i++) {
//            ret=fscanf(query_file, "%d %d\n", &s[i], &t[i]);
//        }
//
//        int reach, unreach1, unreach2, unreach3;
//        reach=unreach1=unreach2=unreach3=0;
//
//
//        clock_t start,end;
//        start=clock();
//        Runtimecounter rt;
//        rt.start();
//        int m,n,p,q,i2,j2,size1,size2;
//        int *intArray1, *intArray2;
//        std::ofstream myfile;
//        myfile.open("../query/query_result/random-1000-10_results");
//        for (int i = 0; i < queryNum; i++) {
//
//            //	query(s, t);
//            p=s[i];
//            q=t[i];
//            if (p == q) {
//                myfile << 1 << '\n';
//                continue;
//            }
//            if (dag[p] != dag[q]){    //after callapsing into super node, in the same connected DAG?
//                ++unreach1;
////            cout << p << " to " << q << ": false" << endl;
//                myfile << 0 << '\n';
//            }
//            else {
//                if (topo[p] >= topo[q]){      //topo means the topo order
//                    ++unreach2;
////                cout << p << " to " << q << ": false" << endl;
//                    myfile << 0 << '\n';
//                }
//                else {
//                    i2 = j2 = 0;
//
//                    //SL should int** type
//                    intArray1=TL[p];
//                    intArray2=TL[q + sccN];
//
//                    //SLsize[i] is the size of SL[i]
//                    size1=tlsize[p];
//                    size2=tlsize[q + sccN];
//                    while ((i2 < size1) && (j2 < size2)) {
//                        //SL[0~vertexN-1] store the out label of each vertex, SL[vertexN~2*vertexN-1] store the in label of each vertex
//
//                        if(intArray1[i2] < intArray2[j2]){
//                            ++i2;
//                        }
//                        else if(intArray1[i2] > intArray2[j2]){
//                            ++j2;
//                        }
//                        else{//		if (intArray1[i2] == intArray2[j2])
//                            ++reach;
////                        cout << p << " to " << q << ": true" << endl;
//                            myfile << 1 << '\n';
//                            break;
//                        }
//                    }
//
//                    if((i2 == size1) || (j2 == size2)){
//                        ++unreach3;
////                    cout << p << " to " << q << ": false" << endl;
//                        myfile << 0 << '\n';
//                    }
//                }
//            }
//        }
//        myfile.close();
//        rt.stop();
//        end=clock();
//        printf("Time used: %lf\n",rt.GetRuntime());
//        printf("Time: %lu\n",(end-start));
//        printf("%d reach out of %d queries\n",reach,queryNum);
//        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
//        free(s);
//        free(t);
//
//        for (int i = 0; i < 2 * sccN; i++) {
//            free(TL[i]);
//        }
//        free(TL);
//        free(tlsize);
//        //   free(scc);
//        free(dag);
//        free(topo);
//        return 0;
//    }
}
#endif //TC_QUERYSCC_H
