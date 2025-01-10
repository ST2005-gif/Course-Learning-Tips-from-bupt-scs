`timescale 10ps/1ps

module Verilog1(CLK,Q1,Q2,Q3,Q4);
    input CLK;
    output Q1,Q2,Q3,Q4;
    reg Q1,Q2,Q3,Q4;
    always @(posedge CLK) begin
        Q1<=(~Q2&Q1)|(Q1&~Q3)|(~Q1&Q2&Q3);
        Q2<=(~Q2&Q3)|(Q2&~Q3);
        Q3<=~Q3;
        Q4<=Q1&Q2&Q3;
    end
    initial begin
        Q1<=0;
        Q2<=0;
        Q3<=0;
        Q4<=0;
        $dumpfile("3.10.vcd");
        $dumpvars(0,Verilog1);
    end
endmodule

module Verilog1_tb;
    reg c;
    wire q1,q2,q3,q4;
    Verilog1 Verilog1(.Q1(q1),.Q2(q2),.Q3(q3),.Q4(q4),.CLK(c));
    initial begin
        c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 c<=1;
        #10 c<=0;
        #10 $finish;
    end
endmodule
