#include "../include/BaseGraph.h"
#include <iostream>
#include <string>
using namespace std;

BaseGraph::BaseGraph() {
    n_left = stoi(data_config["n_left"]);
    n_right = stoi(data_config["n_right"]);
    n = n_right+n_left+1; // for index from 0
    m = stoi(data_config["m"]);
    con = vector<vector<int>>(n);

    FILE* fin;
    fin = fopen(config["data_path"].c_str(),"r");

    int a,b;
    int cnt = 0;
    char line[1024];
    while (fgets(line,1024,fin)){
        if (cnt++ % 10000000 == 0) cout<<cnt<<endl;

        if(line[0]=='%') continue;

        char* num1,*num2;
        num1 = line;
        int len = (int)strlen(line);
        for(int i=1;i<len;i++){
            if (!isdigit(line[i])) {
                num2 = line + i + 1;
                line[i] = '\0';
                for (int j = i + 1; j < len; j++) {
                    if (!isdigit(line[j])) {
                        line[j]='\0';
                        break;
                    }
                }
                break;
            }
        }
        sscanf(num1, "%d", &a);
        sscanf(num2, "%d", &b);
        b = b+n_left;
        con[a].push_back(b);
        con[b].push_back(a);
    }



}