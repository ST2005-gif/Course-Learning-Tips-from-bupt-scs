module v18(
    input clk,
    input clr,
    input [3:0]d,
    input left,
    input right,
    output reg [3:0] q
);
always@(posedge clk or posedge clr)begin
    if(clr)
    q<=4'b0000;
    else if(right)
    q<={q[0],q[3],q[2],q[1]};
    else if(left)
    q<={q[2],q[1],q[0],q[3]};
    else
    q<=d;
end
endmodule