#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "VSampler.h"
#include "VSampler___024root.h"

#define MAX_SIM_TIME 200000
#define C_RED "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_RESET "\033[0m"
vluint64_t sim_time = 0;

static inline std::string trim(const std::string &s)
{
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

int main(int argc, char **argv, char **env)
{
    VSampler *dut = new VSampler;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");
    std::string line, answer_line;
    std::ifstream fin("a.txt");
    std::ifstream fanswer("b.txt");
    uint32_t line_vals[3] = {0, 0, 0}, ans_vals[8] = {0, 0, 0,0,0,0,0,0};
    uint8_t start_counter = 0,mode_val=0;
    bool awating_done = false;
    int samp_data[8]={0};
    int samp_cnt=0;
    int sampling_counter=0;
    bool property_ALL=true;
    bool ans_property=false;
    while (sim_time < MAX_SIM_TIME)
    {
        if (sim_time == 2)
        {
            dut->rst_n = 0;
        }
        if (sim_time == 7)
        {
            dut->rst_n = 1;
        }
        if (dut->clk == 1)
        {
            if (!awating_done && dut->rst_n)
            {
                std::getline(fin, line);
                line = trim(line);
                std::stringstream ss(line);
                ss >>std::hex>>mode_val>>line_vals[0] >> line_vals[1] >> line_vals[2];
                VlWide<3UL> random_data;
                std::copy(std::rbegin(line_vals), std::rend(line_vals), std::begin(random_data.m_storage));
                dut->random = random_data;
                mode_val=u_int8_t(mode_val-'0');
                printf("mode=%d\n",mode_val);
                dut -> mode = mode_val;
                dut->en = 1;
                awating_done = true;
                start_counter = 0;
                samp_cnt=0;
                if (!fin)
                {
                    std::cout << "[INFO] 输入文件读完" << std::endl;
                    break;
                }
            }
            if (dut->en && start_counter < 1)
            {
                start_counter++;
            }
            else
            {
                dut->en = 0;
            }
            if (awating_done)
            {
                if(dut->done){
                    awating_done = false;
                    std::getline(fanswer, answer_line);
                    answer_line = trim(answer_line);
                    std::stringstream ss2(answer_line);
                    ans_property=true;
                    int ansnum=0;
                    sampling_counter++;
                    if(dut->mode==0){
                        ansnum=8;
                    }
                    else{
                        ansnum=4;
                    }
                    {
                        for(int i=0;i<ansnum;i++){
                            ss2 >> std::hex >> ans_vals[i];
                            if(ans_vals[i]==samp_data[i]){
                                std::cout << C_GREEN << "[INFO] cycle" << i << " 采样结果正确: 0x" << std::hex << samp_data[i] << C_RESET << std::endl;
                            }else{
                                std::cout<<C_RED<<"[ERROR] cycle"<<i<<" 采样结果错误, 期望值:0x "<< std::hex<<ans_vals[i]<<" 实际值: "<<samp_data[i]<<C_RESET<<std::endl;
                                property_ALL=false;
                            }
                        }
                    }
                }
                else {
                    if(samp_cnt>0)
                    if(dut->mode==0){
                        samp_data[samp_cnt-1]=dut->bin_sampling;
                        printf("bin%d=%2x\n",samp_cnt,dut->bin_sampling);
                    }else{
                        samp_data[samp_cnt-1]=dut->rej_sampling;
                        printf("rej%d=%6x\n",samp_cnt,dut->rej_sampling);
                    } 
                    samp_cnt++;
                }
            }
        }
        dut->clk ^= 1;
        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }
    printf("总采样次数=%d\n",sampling_counter);
    if(property_ALL){
        std::cout << C_GREEN << "[INFO] ALL PASS" << C_RESET << std::endl;
    }else{
        std::cout << C_RED << "[ERROR] SOME FAIL" << C_RESET << std::endl;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
