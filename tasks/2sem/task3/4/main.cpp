#include <fstream>
#include <vector>
#include <queue>
#include <iostream>
#include <cassert>

using std::vector;
using std::queue;
using std::string;
using std::cin;
using std::cout;

const int INF = 2147483647;

/******************************************************************************/

class Net{
public:
    Net() = default;
    Net(unsigned size);

    void setThroughput(unsigned s, unsigned t, int throughput);
    void changeThroughput(unsigned s, unsigned t, int delta);
    int getThroughput(unsigned s, unsigned t) const;

    void changeFlow(unsigned s, unsigned t, int delta);
    int getFlow(unsigned s, unsigned t) const;

    unsigned size() const;

    void residualNet(Net & net);

private:
    unsigned size_;
    vector<vector<int>> throughputs_;
    vector<vector<int>> flows_;
};

/******************************************************************************/

class FindMaxFlow{
public:
    unsigned operator()(Net &net, unsigned s_, unsigned t_){
        s = s_;
        t = t_;
        n = net.size();
        unsigned maxFlow = 0;
        while (bfs(net)) {
            p.clear();
            p.assign(n, 0);
            int flow = dfs(s, INF, net);
            while (flow != 0) {
                maxFlow += flow;
                flow = dfs(s, INF, net);
            }
        }
        return maxFlow;
    }

private:
    bool bfs(Net &net){
        d.clear();
        d.assign(n, INF);
        d[s] = 0;
        queue<unsigned> q;
        q.push(s);
        while (!q.empty()) {
            unsigned u = q.front();
            q.pop();
            for (unsigned v = 0; v < n; v++) {
                if (net.getFlow(u, v) < net.getThroughput(u, v) &&
                    d[v] == INF) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[t] != INF;
    }


    int dfs(unsigned u, int minC, Net &net){
        if (u == t || minC == 0)
            return minC;
        for (unsigned v = p[u]; v < n; v++) {
            if (d[v] == d[u] + 1) {
                int delta = dfs(v, std::min(minC, net.getThroughput(u, v) -
                                                  net.getFlow(u, v)), net);
                if (delta != 0) {
                    net.changeFlow(u, v, delta);
                    net.changeFlow(v, u, -delta);
                    return delta;
                }
            }
            p[u]++;
        }
        return 0;
    }
    vector<unsigned> d;
    vector<unsigned> p;
    unsigned n;
    unsigned s;
    unsigned t;
};

/******************************************************************************/

void BFS(const Net & net, unsigned s, vector<bool>& component){
    component.assign(net.size(), false);
    component[1] = true;
    queue<unsigned> q;
    q.push(s);
    while(!q.empty()){
        unsigned u = q.front();
        q.pop();
        for(unsigned v = 0; v < net.size(); v++){
            if(net.getThroughput(u, v) > 0 && !component[v]){
//                std::cout << v << '\n';
                component[v] = true;
                q.push(v);
            }
            if(net.getThroughput(v, u) > 0 && !component[v]){
//                std::cout << v << '\n';
                component[v] = true;
                q.push(v);
            }
        }
    }
}

/******************************************************************************/

int main()
{
    string str, temp;
    cin >> str >> temp;

    unsigned conflicts = 0;
    Net net(str.length() + temp.length() + 2);
    for (int i = 0; i < str.length() - temp.length() + 1; i++)
        for (int j = 0; j < temp.length(); j++) {
            if (str[i + j] == '?') {
                if (temp[j] == '?') {
                    net.changeThroughput(i + j + 2, str.length() + j + 2, 1);
                    net.changeThroughput(str.length() + j + 2, i + j + 2, 1);
                } else if (temp[j] == '0') {
                    net.changeThroughput(i + j + 2, 0, 1);
                    net.changeThroughput(0, i + j + 2, 1);
                } else {
                    net.changeThroughput(i + j + 2, 1, 1);
                    net.changeThroughput(1, i + j + 2, 1);
                }
            } else if (temp[j] == '?') {
                if (str[i + j] == '0') {
                    net.changeThroughput(str.length() + j + 2, 0, 1);
                    net.changeThroughput(0, str.length() + j + 2, 1);
                } else {
                    net.changeThroughput(str.length() + j + 2, 1, 1);
                    net.changeThroughput(1, str.length() + j + 2, 1);
                }
            } else if (str[i + j] != temp[j]) {
                conflicts++;
            }
        }


    FindMaxFlow findMaxFlow;
    cout << conflicts + findMaxFlow(net, 0, 1) << '\n';

    vector<bool> components;
    Net res;
    net.residualNet(res);
    BFS(res, 1, components);

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '?') {
            if (components[i + 2]) {
                cout << 1;
            } else {
                cout << 0;
            }
        } else {
            cout << str[i];
        }
    }
    cout << '\n';
    for (int i = 0; i < temp.length(); i++) {
        if (temp[i] == '?') {
            if (components[str.length() + i + 2]) {
                cout << 1;
            } else {
                cout << 0;
            }
        } else {
            cout << temp[i];
        }
    }

//    fout << '\n';
//
//    for(int i = 0; i < res.size(); i++){
//        for(int j = 0; j < res.size(); j++){
//            fout << res.getThroughput(i, j);
//        }
//        fout << '\n';
//    }
}

/******************************************************************************/

Net::Net(unsigned size):
    size_(size),
    throughputs_(size, vector<int>(size, 0)),
    flows_(size, vector<int>(size, 0)){}



void Net::setThroughput(unsigned s, unsigned t, int throughput)
{
    throughputs_[s][t] = throughput;
}



void Net::changeThroughput(unsigned s, unsigned t, int delta)
{
    throughputs_[s][t] += delta;
}



int Net::getThroughput(unsigned s, unsigned t) const
{
    return throughputs_[s][t];
}



void Net::changeFlow(unsigned s, unsigned t, int delta)
{
    flows_[s][t] += delta;
}



int Net::getFlow(unsigned s, unsigned t) const
{
    return flows_[s][t];
}



unsigned Net::size() const
{
    return size_;
}



void Net::residualNet(Net & other)
{
    other.size_ = size_;
    other.flows_.assign(size_, vector<int>(size_, 0));
    other.throughputs_.assign(size_, vector<int>(size_, 0));
    for(unsigned i = 0; i < size_; i++)
        for(unsigned j = 0; j < size_; j++)
            if(getFlow(i, j) >= 0)
                other.setThroughput(i, j, getThroughput(i, j) - getFlow(i, j));
}