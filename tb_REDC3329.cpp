#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "VREDC3329_pipeline.h"
#include "VREDC3329_pipeline___024root.h"

#define MAX_SIM_TIME 400
vluint64_t sim_time = 0;

int main(int argc, char** argv, char** env) {
    VREDC3329_pipeline *dut = new VREDC3329_pipeline;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("wav.vcd");
    int a=1234;
    int b=1232;
    int a1,a2,b1,b2;
    while (sim_time < MAX_SIM_TIME) {
     
        dut->clk ^= 1;
        if(sim_time == 2) {
            dut->rst_n = 0;
        }
         if(sim_time ==3){
            dut->rst_n = 1;
            dut->en = 1;
        }
        if(sim_time >200){
            dut->en = 0;
        }
  
        // if(sim_time ==3+4*2){
        //     dut->rst_n = 1;
        //     dut->en = 1;
        //     dut->a = 989;
        //     dut->b = 678;
        // }
        if((sim_time-3)%8==0){
            a2=a1; b2=b1;
            a1=a; b1=b;
            if(b<2048){b++;a++;}
            else {
                b=0;
                a++;
            }
            if(a>3328) break;
            dut->a = a;
            dut->b = b;
            
        }

        if(dut->done&&dut->clk){
            int r = (a2*b2)%3329;
            if(dut->r!=r){
                std::cout<<"Error: a="<<a2<<" b="<<b2<<" r="<<dut->r<<" != "<<r<<std::endl;
                //exit(EXIT_FAILURE);
               // break;
            } else {
               // if(b2%1==0)
                std::cout<<"OK: a="<<a2<<" b="<<b2<<" r="<<dut->r<<std::endl;
            }
        }
        if(1){
            dut->en = (b%2==0)?1:1;
        }
        dut->eval();

        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
