#pragma once

#include <cstdio>
#include <cstring>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>

typedef std::map<std::string,std::vector<int>> CsvData;
typedef std::map<std::string,std::vector<double>> CsvDataDbl;

inline int num_digits(int i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

class Csv
{
private:
    CsvData m_data;
    size_t max_col;
    std::vector<std::string> cols;

    typedef CsvDataDbl(Csv::*Func)(CsvData&);
    std::map<std::string, Func> func_map {{"sum", &Csv::getSum}, {"median",&Csv::getMedian},
                                          {"mean",&Csv::getMean}, {"std",&Csv::getStd}};
    std::vector<std::string> to_tokens(std::string& s, char delim);

    void readFromFile(std::string& filename);

    template <typename T>
    void print(T data){
        unsigned mc = max_col+9;
        std::cout << "\n";
        std::cout << std::string(6, '-');
        for(auto& kv : data){
            std::cout << std::string(mc, '-');
        }
        std::cout << "\n" << "|  ";
        int cnt = 0;
        for(std::string& c : cols)
        {
            std::cout << c;
            unsigned nsp = mc - c.length();
            std::cout << std::string(nsp-2, ' ');
            if(cnt != data.size() - 1) {
                std::cout << "|  ";
            }
            cnt++;
        }
        std::cout << "|";
        std::cout << "\n";
        std::cout << std::string(6, '-');
        for(auto& kv : data){
            std::cout << std::string(mc, '-');
        }
        std::cout << "\n";
        for(int i = 0; i < data.begin()->second.size(); ++i)
        {
            int ct = 0;
            std::cout << "|  ";
            for(auto& kv : data)
            {
                std::cout << std::fixed << std::setprecision(2) << kv.second[i];
                int f = num_digits(kv.second[i]);
                int nsp = (mc - num_digits(kv.second[i]));
                if (std::is_same<T, CsvDataDbl>::value){
                    nsp -= 3;
                }
                std::cout << std::string(nsp-2, ' ');
                if(ct != data.size() - 1) {
                    std::cout << "|  ";
                }
                ct++;
            }
            std::cout << "|";
            std::cout << "\n";
        }
        std::cout << std::string(6, '-');
        for(auto& kv : data){
            std::cout << std::string(mc, '-');
        }
    };

public:
    Csv() = default;
    explicit Csv(std::string filename){
        readFromFile(filename);
    }

    CsvDataDbl getSum(CsvData& data);

    CsvDataDbl getMedian(CsvData& data);

    CsvDataDbl getMean(CsvData& data);

    CsvDataDbl getStd(CsvData& data);

    void printRes(std::string& cmd);
};
