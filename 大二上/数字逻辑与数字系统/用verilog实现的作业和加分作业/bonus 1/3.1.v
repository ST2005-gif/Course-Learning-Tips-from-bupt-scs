`timescale 10ps/1ps

module Verilog1(A,B,C,Y);
input A,B,C;
output Y;
reg Y;

always @(A, B, C) begin
        Y<=((~B)|(~C)|(A&Y));
end

initial begin
    Y<= 1;
    $dumpfile("3.1.vcd");
    $dumpvars(0,Verilog1);
end
endmodule

module Verilog1_tb;
reg aa,bb,cc;
wire yy;
Verilog1 Verilog1(.A(aa),.B(bb),.C(cc),.Y(yy));
initial begin
        aa<=1;
        bb<=1;
        cc<=1;
    #10 aa<=0;
    #10 aa<=1;
    #5  bb<=0;
    #5  cc<=0;
    #5  bb<=1;
    #5  cc<=1;
    #5  bb<=0;
    #10 bb<=1;
    #10 aa<=0;
    #10 aa<=1;
    #5  cc<=0;
    #10 $finish;
end
endmodule
