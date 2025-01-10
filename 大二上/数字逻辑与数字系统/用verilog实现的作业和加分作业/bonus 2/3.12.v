`timescale 10ps/1ps
module Verilog1(
    input wire clk,      // 时钟信号
    output reg [3:0] q   // 计数输出
);

    always @(posedge clk) begin
            if (q == 4'b1011) begin  // 11 (12进制的最大值)
                q <= 4'b0000;  // 溢出后回到0
            end else begin
                q <= q + 1;  // 计数加1
            end
    end
    initial begin
        q<=4'b0000;
        $dumpfile("3.12.vcd");
        $dumpvars(0,Verilog1);
    end
endmodule
module Verilog1_tb;
    reg c;
    wire [3:0]q;
    integer i;
    Verilog1 Verilog1(.clk(c),.q(q));
    initial begin
        c<=0;
        for(i=0;i<13;i=i+1)begin
        #10 c<=1;
        #10 c<=0;
        end
        #10 $finish;
    end
endmodule