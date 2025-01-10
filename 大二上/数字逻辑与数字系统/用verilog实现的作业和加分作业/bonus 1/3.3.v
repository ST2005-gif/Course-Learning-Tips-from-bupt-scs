`timescale 10ps/1ps

module Verilog1(J,K,CLK,Q);
    input J,K,CLK;
    output Q;
    reg Q;
    always @(negedge CLK) begin
        Q <=((J&(~Q))|((~K)&Q));
    end
    initial begin
        Q<= 0;
        $dumpfile("3.3.vcd");
        $dumpvars(0,Verilog1);
    end
endmodule

module Verilog1_tb;
    reg j,k,c;
    wire q;
    Verilog1 Verilog1(.J(j),.K(k),.CLK(c),.Q(q));
    initial begin
        c<=0;
        forever
            #10 c=~c;
    end
    initial begin
        j<=0;
        k<=0;
        #15 j=1;
        #6 j=0;
        #4 j=1;
        k=1;
        #30 k=0;
        #10 j=0;
        #2 k=1;
        #8 k=0;
        #10 k=1;
        #8 j=1;
        #2 j=0;
        #14 k=0;
        #2 k=1;
        #2 k=0;
        #10 $finish;
    end
endmodule
