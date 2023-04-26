#include <algorithm>
#include <numeric>
#include "csv.h"


std::vector<std::string> Csv::to_tokens(std::string& s, char delim) {
    size_t pos = 0;
    std::vector<std::string> tks;
    while ((pos = s.find(delim)) != std::string::npos) {
        tks.push_back(s.substr(0, pos));
        s.erase(0, pos + 1);
    }
    tks.push_back(s.substr(0, pos));

    tks.erase(
            std::remove_if(
                    tks.begin(),
                    tks.end(),
                    [](std::string const& s) { return s.size() == 0; }),
            tks.end());
    auto l_str = tks[tks.size() - 1];
    auto l_chr = l_str[l_str.length()-1];
    if (!tks.empty() && l_chr == '\r')
        tks[tks.size() - 1].pop_back();
    return tks;
}



void Csv::readFromFile(std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        char delim = ' ';
        if (line.find(',') != std::string::npos) {
            delim = ',';
        }
        cols = to_tokens(line, delim);
        max_col = (*std::max_element(cols.begin(), cols.end(),
                                   [](const auto& a, const auto& b) {
                                       return a.size() < b.size();
                                   })).length();
        while (std::getline(file, line)) {
            std::vector<std::string> data_tks = to_tokens(line, delim);
            for(int i = 0; i < data_tks.size(); ++i){
                m_data[cols[i]].push_back(std::stoi(data_tks[i]));
            }
        }
    }
}

CsvDataDbl Csv::getSum(CsvData& data) {
    CsvDataDbl res;
    for(auto& kv : data){
        auto kv_res = static_cast<double>(std::reduce(kv.second.begin(), kv.second.end()));
        res[kv.first] = std::vector<double>{kv_res};
    }
    return res;
}

CsvDataDbl Csv::getMedian(CsvData& data) {
    CsvDataDbl res;
    for(auto& kv : data){
        double kv_res;
        std::sort(kv.second.begin(), kv.second.end());
        if (kv.second.size() % 2 == 0) {
            kv_res = (kv.second[kv.second.size() / 2 - 1] + kv.second[kv.second.size() / 2]) / 2.0;
        } else {
            kv_res = kv.second[kv.second.size() / 2.0];
        }
        res[kv.first] = std::vector<double>{kv_res};
    }
    return res;
}

CsvDataDbl Csv::getMean(CsvData& data){
    CsvDataDbl res = getSum(data);
    for(auto& kv : res){
        kv.second[0] /= static_cast<double>(data.begin()->second.size());
    }
    return res;
}

CsvDataDbl Csv::getStd(CsvData& data){
    CsvDataDbl res = getMean(data);
    for(auto& kv : data) {
        long double square_sum_of_difference = 0;
        long double mean_var = res[kv.first][0];
        auto len = kv.second.size();
        long double xv;
        for (auto &each: kv.second) {
            xv = each - mean_var;
            square_sum_of_difference += xv * xv;
        }
        res[kv.first][0] = std::sqrt(square_sum_of_difference / (len - 1));
    }
    return res;
}

void Csv::printRes(std::string& cmd){
    Func get_res = func_map[cmd];
    if(cmd == "print"){
        print(m_data);
    } else {
        CsvDataDbl res = (this->*get_res)(m_data);
        if(!res.empty()){
            print(res);
        }
    }
}