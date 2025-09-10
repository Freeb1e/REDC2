`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// combany:
// Engineer:
//
// Create Date: 2025/08/28 13:29:54
// Design Name:
// Module Name: REDC3329
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module REDC3329_pipeline(
        input clk,
        input rst_n,
        input en,
        input [11:0] a,
        input [11:0] b,
        output busy,
        output done,
        output reg [11:0] r
    );

    parameter WIDTH = 12;
    parameter MOD = 3329;
    parameter MOD_INV = 3327;
    parameter R2_MOD = 2385;
    assign done=(state==3'd2||state==3'd6)?1'b1:1'b0;
    reg [2:0] state;
    reg [11:0] in_a_1,in_b_1;
    reg [11:0] in_2;
    reg [11:0] in_3;
    reg [23:0] T_2,T_3;
    wire [23:0] out_1;
    wire [11:0] out_2;
    wire [11:0] out_3;
    reg [11:0] out_4,out_5;

    always_ff @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            state<=3'd0;
        end
        else begin
            if (state==3'd7) begin
                state<=3'd0;
            end
            else begin
                state<=state+3'd1;
            end
        end
    end

    REDC_part1 part1_1(
        .a(in_a_1),
        .b(in_b_1),
        .r(out_1)
    );

    REDC_part2 part2_1(
        .a(in_2),
        .r(out_2)
    );
    REDC_part3 part3_1(
        .a(in_3),
        .T_3(T_3),
        .r(out_3)
    );
    assign r=done?out_3:12'hFFF;

    always_ff@(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            in_a_1<=0;
            in_b_1<=0;
        end
        else begin
            if(state==3'd0||state==3'd4) begin
                if(en)begin
                    in_a_1<=a;
                    in_b_1<=R2_MOD;
                end else begin
                    in_a_1<=12'd0;
                    in_b_1<=12'd0;
                end
            end
            else if(state==3'd2||state==3'd6) begin
                if(en)begin
                in_a_1<=b;
                in_b_1<=R2_MOD;
                end else begin
                    in_a_1<=12'd0;
                    in_b_1<=12'd0;
                end
            end
            else if(state==3'd1||state==3'd5) begin
                in_a_1<=out_3;
                in_b_1<=out_5;
            end
            else begin
                in_a_1<=12'd0;
                in_b_1<=12'd0;
            end

        end

    end

    always_ff@(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            in_2<=0;
            T_2<=0;
        end 
        else begin
        if(state==3'd4) begin
            in_2<=out_3;
            T_2<={12'b0,out_3};
        end
        else if(state==3'd0)begin
            in_2<=out_3;
            T_2<={12'b0,out_3};
        end
        else begin
            in_2<=out_1[11:0];
            T_2<=out_1;
        end
    end
    end
    always_ff @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            in_3<=0;
            T_3<=0;
            out_4<=0;
            out_5<=0;
        end
        else begin
            in_3<=out_2;
            T_3<=T_2;
            out_4<=out_3;
            out_5<=out_4;
        end
    end
endmodule

module REDC_part1(
    input [11:0] a,
    input [11:0] b,
    output [23:0] r
);
    assign r=(a*b);
endmodule

module REDC_part2(
    input [11:0] a,
    output [11:0] r
);
    parameter MOD_INV = 3327;
    wire [23:0] r_raw;
    assign r_raw=(a*MOD_INV);
    assign r=r_raw[11:0];
endmodule

module REDC_part3(
    input [11:0] a,
    input [23:0] T_3,
    output [11:0] r
);
    parameter MOD = 3329;
    wire [24:0] r_raw,m;
    wire [24:0] r_test;
    assign m=(a*MOD);
    assign r_test={1'b0,T_3}+m;
    assign r_raw=(T_3+m)>>12;
    wire [12:0] r_final;
    assign r_final=(r_raw[12:0]>=MOD)?(r_raw[12:0]-MOD):r_raw[12:0];
    assign r=r_final[11:0];
endmodule


module REDC_unit_comb(
        input [11:0] a,
        input [11:0] b,
        output [11:0] r
    );
    parameter WIDTH = 12;
    parameter MOD = 3329;
    parameter MOD_INV = 3327;
    wire [2*WIDTH-1:0] T;
    wire [WIDTH-1:0] m;
    assign T=a*b;
    assign m=((T[WIDTH-1:0])*MOD_INV)&({WIDTH{1'b1}});
    wire [2*WIDTH-1:0] result_raw;
    assign result_raw=(T+m*MOD)>>WIDTH;
    wire [12:0] r_raw;
    assign r_raw=(result_raw[12:0]>=MOD)?(result_raw[12:0]-MOD):result_raw[12:0];
    assign r=r_raw[11:0];
endmodule
