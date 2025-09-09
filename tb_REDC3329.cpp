#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "VREDC3329_pipeline.h"
#include "VREDC3329_pipeline___024root.h"

#define MAX_SIM_TIME 200
vluint64_t sim_time = 0;

int main(int argc, char** argv, char** env) {
    VREDC3329_pipeline *dut = new VREDC3329_pipeline;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("wav.vcd");
    int a=1234;
    int b=1234;
    while (sim_time < MAX_SIM_TIME) {
        dut->clk ^= 1;
        if(sim_time == 2) {
            dut->rst_n = 0;
        }
        if(sim_time ==3){
            dut->rst_n = 1;
            dut->en = 1;
            dut->a = 1234;
            dut->b = 2345;
        }
        
        if(sim_time ==3+4*2){
            dut->rst_n = 1;
            dut->en = 1;
            dut->a = 989;
            dut->b = 678;
        }
        if((sim_time-3)%4==0){
            if(b<2048)b++;
            else {
                b=0;
                a++;
            }
            dut->a = a;
            dut->b = b;
        }
        
        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
