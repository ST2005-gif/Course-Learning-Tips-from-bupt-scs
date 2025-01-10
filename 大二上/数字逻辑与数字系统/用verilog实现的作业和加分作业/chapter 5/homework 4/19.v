module v19(
    input clk,
    input reset,
    output reg [3:0] count,
    output reg carry
);
always @(posedge clk or posedge reset) begin
    if (reset) 
    begin
        count <= 4'b0000;
        carry <= 1'b0;    
    end 
    else if (count == 4'b1001) 
    begin 
            count <= 4'b0000;       
            carry <= 1'b1;          
        end 
        else begin
            count <= count + 1;     
            carry <= 1'b0;       
        end
end

endmodule