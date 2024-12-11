#include "../include/BaseGraph.h"
#include <iostream>
#include <cstring>
#include <string>
#include <filesystem>
#include <cstdio>
#include <algorithm>

using namespace std;

BaseGraph::BaseGraph() {
    if (filesystem::exists(config["serialize_path"])){
        read_bin();
        return;
    }

    n_left = stoi(data_config["n_left"]);
    n_right = stoi(data_config["n_right"]);
    n = n_right+n_left;
    m = stoi(data_config["m"]);
    con = vector<vector<int>>(n+1);

    FILE* fin;
    fin = fopen(data_config["data_path"].c_str(),"r");

    int a,b;
    char line[1024];
    while (fgets(line,1024,fin)){
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
    deg.resize(n+1);
    for(int i=1;i<=n;i++){
        deg[i]=con[i].size();
    }

    save_bin();
}

void BaseGraph::save_bin() {
    if (!filesystem::exists(config["serialize_path"])){
        cout<<"文件没有存在"<<endl;
        FILE* fout = fopen(config["serialize_path"].c_str(),"wb");
        fwrite(&n_left, sizeof(int), 1, fout);
        fwrite(&n_right, sizeof(int), 1, fout);
        fwrite(&n, sizeof(int), 1, fout);
        fwrite(&m, sizeof(int), 1, fout);
        fwrite(deg.data(), sizeof(int), n+1, fout);
        for(int i=1;i<=n;i++){
            sort(con[i].begin(), con[i].end());
            fwrite(con[i].data(),sizeof(int),con[i].size(),fout);
        }
        fclose(fout);
    }
}

void BaseGraph::read_bin() {
    if (filesystem::exists(config["serialize_path"])){
        cout<<"文件已存在"<<endl;
        FILE* fin = fopen(config["serialize_path"].c_str(),"rb");
        cout<<fin<<endl;

        fread(&n_left, sizeof(int), 1, fin);
        fread(&n_right, sizeof(int), 1, fin);
        fread(&n, sizeof(int), 1, fin);
        fread(&m, sizeof(int), 1, fin);
        deg.resize(n+1);
        fread(deg.data(), sizeof(int), n+1, fin);
        con.resize(n+1);
        for (int i = 1; i <= n; ++i) {
            int size = deg[i]; // 根据度数推导出数组的大小
            con[i].resize(size);
            fread(con[i].data(), sizeof(int), size, fin);
        }
    }
}
