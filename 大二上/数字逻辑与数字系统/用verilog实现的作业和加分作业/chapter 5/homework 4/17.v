module v17(
    input clk,
    input clr,
    input [3:0]d,
    output reg [3:0] q
);
always@(posedge clk or posedge clr)begin
    if(clr)
    q<=4'b0000;
    else
    q[0]<=d[3];
    q[1]<=d[2];
    q[2]<=d[1];
    q[3]<=d[0];
end
endmodule