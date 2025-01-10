module v20(
    input y,
    input clk,
    input clr,
    output reg [2:0] q
);
reg [2:0] q1;
always@(posedge clk or posedge clr)begin
    if(clr)
    q1<=3'b000;
    else if(y)begin
        if(q1==3'b111)
        q1<=3'b000;
        else
        q1<=q1+1;
    end
    else if(~y)begin
        if(q1==3'b000)
        q1<=3'b111;
        else
        q1<=q1-1;
    end
    q[2]=q1[2];
    q[1]=q1[2]^q1[1];
    q[0]=q1[1]^q1[0];
end

endmodule






