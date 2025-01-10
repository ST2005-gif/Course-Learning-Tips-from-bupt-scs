`timescale 10ps/1ps

module Verilog1(CLK,Q1,Q2,Q3);
    input CLK;
    output Q1,Q2,Q3;
    reg Q1,Q2,Q3;
    always @(posedge CLK) begin
        Q1<=(~Q3)|(~Q2&Q1);
        Q2<=(Q2&Q1)|(~Q2&~Q1)|(~Q3&Q1);
        Q3<=Q2;
    end
    initial begin
        Q1<=1;
        Q2<=0;
        Q3<=0;
        $dumpfile("3.16.vcd");
        $dumpvars(0,Verilog1);
    end
endmodule

module Verilog1_tb;
    reg c;
    wire q1,q2,q3;
    integer i;
    Verilog1 Verilog1(.Q1(q1),.Q2(q2),.Q3(q3),.CLK(c));
    initial begin
        c<=0;
        for(i=0;i<8;i=i+1)begin
        #10 c<=1;
        #10 c<=0;
        end
        #10 $finish;
    end
endmodule
