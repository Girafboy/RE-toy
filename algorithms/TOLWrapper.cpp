//
// Created by 王星力 on 2022/12/14.
//

#include "TOLWrapper.h"

#include "TolUtils/Butterfly.h"

namespace tol {
    string TOLWrapper::getName() const {
        return "TOL";
    }

    string TOLWrapper::getParams() const {
        return "style=" + std::to_string(style) + " opr=" + std::to_string(opr);
    }

    TOLWrapper::TOLWrapper(int style, int opr) : style(style), opr(opr) {

    }

    void TOLWrapper::construction(const Graph &graph) {
        Butterfly run;

        run.convert_graph(graph);

        if ( style == 0 )
        {
            run.compute_index_r1();
        }
        else if ( style == 1 )
        {
            run.compute_index_p1();
        }


        run.build_backlink(0);


        if ( opr == 0 )
        {
            run.compute_order();
            run.AddDelTest(1000);
        }
        else if ( opr == 1 )
        {
            run.optimize();
        }

        auto tmp = run.generate_index_storage();

        run.free();

        for (const auto &x : tmp.first) {
            labels.push_back(x);
            if ( x == -2 ) N++;
        }

        Index = new int*[N*2];

        int i;

        unsigned int p;

        for (i = 0; i < tmp.second.size(); ++i) {
            p = tmp.second[i];
            Index[i] = (labels.m_data + p);
        }
    }

    void TOLWrapper::reset() {
        labels.free_mem();
        labels = iVector<int>();
        delete []Index;
        N = 0;
    }

    bool TOLWrapper::TC_haspath(int source, int target) {
        int *p1 = Index[source * 2];
        int *p2 = Index[target * 2 + 1];

        if (source == target) {
            return true;
        }

        int x, y;

        if ( (*p1) < (*p2) )
        {
            x = *(++p1);
            y = *(++p2);

            if ( x >= 0 && y >= 0 )
            {
                while (true)
                {
//					printf("%d %d\n",*p1,*p2);
                    if ( x == y )
                    {
                        return true;
                    }
                    else if ( x < y )
                    {
                        x = *(++p1);
                        if ( x < 0 ) break;
                    }
                    else
                    {
                        y = *(++p2);
                        if ( y < 0 ) break;
                    }
                }
            }
        }
        return false;
    }

    long long TOLWrapper::getIndexSize() const {
        return labels.m_num * sizeof(int) + N * 2 * sizeof(int*);
    }
} // tol