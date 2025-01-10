module v21(
    input clk,
    input clr,
    input k,
    output reg [1:0] s
);
    always @(posedge clk, posedge clr)begin
        if(clr)
        s<=2'b00;
        else begin
            if(k)begin
                if(s==2'b01)
                s<=2'b10;
                else if(s==2'b11)
                s<=2'b00;
            end
            if(~k)begin
                if(s==2'b00)
                s<=2'b01;
                else if(s==2'b10)
                s<=2'b11;
            end
        end
    end
endmodule