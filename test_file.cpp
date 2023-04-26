#include <iostream>
#include <random>
#include <algorithm>
#include <filesystem>
#include <string>
#include <exception>
#include "gtest/gtest.h"
#include "csv.h"

unsigned random_int(int s, int e){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(s, e);
    return dis(gen);
}

double gen_normal() {
    std::random_device rd{};
    std::mt19937 rng{rd()};
    double mean = 200.3;
    double stddev  = 4.5;
    std::normal_distribution<double> normal(mean, stddev);
    return normal(rng);
}

double gen_bernoulli() {
    std::random_device rd{};
    std::mt19937 rng{rd()};
    double p = 0.3;
    std::bernoulli_distribution bern(p);
    return bern(rng);
}

double gen_gamma() {
    std::random_device rd{};
    std::mt19937 rng{rd()};
    double k = .5;
    double theta = 2.0;
    std::gamma_distribution gamma(k,theta);
    return gamma(rng);
}

double gen_poisson(){
    std::random_device rd{};
    std::mt19937 rng{rd()};
    unsigned lambda = 750;
    std::poisson_distribution<unsigned> d(lambda);
    return d(rng);
}

std::map<std::string,std::vector<unsigned>> generate_data(int rows){

    std::map<std::string,std::vector<unsigned>> container;

    std::vector<unsigned> rd_data;
    rd_data.reserve(rows);
    for(int i = 0; i < rows; ++i){
        rd_data.push_back((unsigned)random_int(0,999));
    }
    container["rand"] = rd_data;

    std::vector<unsigned> norm_data;
    norm_data.reserve(rows);
    for(int i = 0; i < rows; ++i){
        auto dt = gen_normal();
        if(dt > 0){
            norm_data.push_back((unsigned)dt);
        } else {
            norm_data.push_back(0);
        }
    }
    container["normal"] = norm_data;

    std::vector<unsigned> bn_data;
    bn_data.reserve(rows);
    for(int i = 0; i < rows; ++i){
        bn_data.push_back((unsigned)gen_bernoulli());
    }
    container["bern"] = bn_data;

    std::vector<unsigned> gamma_data;
    gamma_data.reserve(rows);
    for(int i = 0; i < rows; ++i){
        gamma_data.push_back((unsigned)gen_gamma());
    }
    container["gamma"] = gamma_data;

    std::vector<unsigned> ps_data;
    ps_data.reserve(rows);
    for(int i = 0; i < rows; ++i){
        ps_data.push_back((unsigned)gen_poisson());
    }
    container["pssn"] = ps_data;

    return container;
}


void generate_test_files(int n, std::map<std::string,std::vector<unsigned>>& test_data){
        try{
            std::filesystem::remove_all("test_data");
            if(std::filesystem::create_directory("test_data")){
                for(int f_num = 0; f_num < n; ++f_num){
                    std::ofstream out_file("test_data/test_"+std::to_string(f_num)+".csv");
                    int cnt = 0;
                    for(auto& kv : test_data)
                    {
                        out_file << kv.first;

                        if(cnt != test_data.size() - 1)
                            out_file << "   ";
                        cnt++;
                    }
                    out_file << "\n";
                    for(int i = 0; i < test_data.begin()->second.size(); ++i)
                    {
                        int ct = 0;
                        for(auto& kv : test_data)
                        {
                            out_file << kv.second[i];
                            unsigned nsp = 9 - num_digits(kv.second[i]);
                            if(ct != test_data.size() - 1)
                                out_file << std::string(nsp, ' ');
                            ct++;
                        }
                        out_file << "\n";
                    }
                    out_file.close();
                }
            }
            else
                std::cerr << "Failed to create a directory\n";
        }catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
}

class csv_test : public ::testing::Test {
protected:

    std::vector<Csv> csv_list;
    std::map<std::string,std::vector<unsigned>> test_data;

    csv_test() {
        test_data = generate_data(50);
        generate_test_files(8,test_data);
        for(int f_num = 0; f_num < 20; ++f_num) {
            std::string fname = "test_data/test_" + std::to_string(f_num) + ".csv";
            Csv csv(fname);
            csv_list.emplace_back(csv);
        }
    }
    ~csv_test() {
        //std::filesystem::remove_all("test_data");
    }
};

TEST_F(csv_test, print_test) {
    std::string cmd = "print";
    csv_list[0].printRes(cmd);
}

TEST_F(csv_test, printSum_test) {
    try{
        std::string cmd = "sum";
        csv_list[0].printRes(cmd);
        unsigned sum_before = 0;
        for(auto kv : test_data[0]){
            sum_before += kv;
        }  //check sum_before matches the output
    } catch(std::exception& e){
      //  //std::cout << e.what() << '\n';
      //  //throw std::runtime_error(msg " at " `__FILE__` ":" `__LINE__`);
      //  try{
      //      throw file_line_exception(__FILE__, __LINE__, "some kind of descrition");
      //  }
      //  catch (const file_line_exception& e)
      //  {
      //      std::cout << "exception from : " << e.file() << "(" << e.line() << ") : " << e.what() << std::endl;
      //  }
    }

}

TEST_F(csv_test, printMedian_test) {
    try {
        std::string cmd = "median";
        csv_list[0].printRes(cmd);
        //check the output matches the median for the distribution
    } catch(std::exception& e){
        std::cout << e.what() << '\n';
    }
}

TEST_F(csv_test, printMean_test) {
    try {
        std::string cmd = "mean";
        csv_list[0].printRes(cmd);
        //check the output matches the mean for the distribution
    } catch(std::exception& e){
        std::cout << e.what() << '\n';
    }
}

TEST_F(csv_test, printStd_test) {
    try{
        std::string cmd = "std";
        csv_list[0].printRes(cmd);
        //check the output matches the std deviation for the distribution
    } catch(std::exception& e){
        std::cout << e.what() << '\n';
    }
}