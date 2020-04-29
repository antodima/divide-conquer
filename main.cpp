#include <iostream>
#include <functional>
#include <numeric>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>
#include <mutex>
#include "dc.hpp"

using namespace std;
using namespace std::chrono;


static bool basecase(vector<int> v) {
        return(v.size() <= 2);
}

static vector<int> solve(vector<int> v) {
        vector<int> solved = { std::accumulate(v.begin(), v.end(), 0) };
        return(solved);
}

static vector<vector<int> > divide(vector<int> v) {
        size_t half_size = v.size() / 2;
        size_t rest = v.size() % 2;
        vector<int> first_half(v.begin(), v.end()-half_size);
        vector<int> second_half(v.begin()+half_size+rest, v.end());
        vector<vector<int> > subproblems = { first_half, second_half };
        return(subproblems);
}

static vector<int> conquer(vector<vector<int> > subproblems) {
        vector<int> sums_subproblems;
        for (vector<int> subproblem : subproblems) {
                int sum = 0;
                for (int e : subproblem) sum += e;
                sums_subproblems.push_back(sum);
        }
        return(sums_subproblems);
}

vector<vector<int> > get_chunks(vector<int> v, int blocks) {
        vector<vector<int> > chunks;
        size_t chunk_size = v.size() / blocks; //cout<<"Chunk size: "<<chunk_size<<endl;
        size_t chunk_rest = v.size() % blocks; //cout<<"Chunk rest: "<<chunk_rest<<endl;
        for (size_t i = 0; i < v.size(); i += chunk_size) {
                bool b = false;
                size_t end_idx = i+chunk_size;
                if (chunk_rest && i+chunk_size*2 > v.size()) {
                        end_idx = i + chunk_size + chunk_rest;
                        b = true;
                }
                vector<int> chunk(&v[i], &v[end_idx]);
                chunks.push_back(chunk);
                if (b) break;
        }
        // cout<<"Chunks:"<<endl;
        // for (vector<int> chunk : chunks) { for (size_t c : chunk) cout<<c<<" "; cout<<"\n"; }
        return(chunks);
}


int main(int argc, char const *argv[]) {

        if(argc == 1) cerr<<"Usage: ./main 'num. seed' 'num. workers'"<<endl;
        int seeds = atoi(argv[1]);
        int nw = atoi(argv[2]);

        vector<int> input;
        for (size_t i = 1; i <= seeds; i++) input.push_back(i);
        for (size_t e : input) cout<<e<<" "; cout<<endl;

        auto start = high_resolution_clock::now();
        auto res = dc<vector<int>, vector<int> >(input, *basecase, *solve, *divide, *conquer);
        auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
        cout<<"Expected sum = "<<std::accumulate(input.begin(), input.end(), 0)<<endl;
        cout<<"Sequential sum = "<<std::accumulate(res.begin(), res.end(), 0)<<endl;
        cout<<"Sequential time: "<<duration.count()<<" usec"<<endl;

        mutex mtx;

        vector<thread> workers;
        start = high_resolution_clock::now();
        vector<vector<int> > chunks = get_chunks(input, nw);
        for (vector<int> chunk : chunks) {
                workers.push_back(thread([&](vector<int> c) {
                        vector<int> res = dc<vector<int>, vector<int> >(c, *basecase, *solve, *divide, *conquer);
                        int sum = std::accumulate(res.begin(), res.end(), 0);
                        {
                                unique_lock<mutex> l(mtx);
                                cout<<"Chunk: ";
                                for (int e : c) cout<<e<<" ";
                                cout<<" - Partial sum = "<<sum<<endl;
                        }
                }, chunk));
        }
        for (thread &t : workers) t.join();
        auto duration_par = duration_cast<microseconds>(high_resolution_clock::now() - start);
        cout<<"Parallel time: "<<duration_par.count()<<" usec"<<endl;

        return 0;
}
