module v15(
    input [0:7]a,
    output reg [2:0]y
);
always@(a) begin
    if(a[7]==0)
    y=3'b000;
    else if(a[6]==0)
    y=3'b001;
    else if(a[5]==0)
    y=3'b010;
    else if(a[4]==0)
    y=3'b011;
    else if(a[3]==0)
    y=3'b100;
    else if(a[2]==0)
    y=3'b101;
    else if(a[1]==0)
    y=3'b110;
    else
    y=3'b111;
end
endmodule