module mod60(
    input clk,
    input clr,
    output reg [0:6] lg,
    output reg [0:6] ls
);
reg [3:0] g;
reg [3:0] s;
always @(posedge clk or negedge clr) begin
    if(!clr | (g==4'b1001 && s==4'b0101))begin
        g<=4'b0000;
        s<=4'b0000;
    end
    else begin
        if(g==4'b1001)begin
            g<=4'b0000;
            s<=s+1;
        end
        else
            g<=g+1;
            s<=s;
    end
end
always @(g) begin
    case(g)
        4'b0000:lg<=7'b1111110;
        4'b0001:lg<=7'b0110000;
        4'b0010:lg<=7'b1101101;
        4'b0011:lg<=7'b1111001;
        4'b0100:lg<=7'b0110011;
        4'b0101:lg<=7'b1011011;
        4'b0110:lg<=7'b1011111;
        4'b0111:lg<=7'b1110000;
        4'b1000:lg<=7'b1111111;
        4'b1001:lg<=7'b1111011;
        default:lg<=7'b0000000;
    endcase
end
always @(s) begin
    case(s)
        4'b0000:ls<=7'b1111110;
        4'b0001:ls<=7'b0110000;
        4'b0010:ls<=7'b1101101;
        4'b0011:ls<=7'b1111001;
        4'b0100:ls<=7'b0110011;
        4'b0101:ls<=7'b1011011;
        4'b0110:ls<=7'b1011111;
        4'b0111:ls<=7'b1110000;
        4'b1000:ls<=7'b1111111;
        4'b1001:ls<=7'b1111011;
        default:ls<=7'b0000000;
    endcase
end
endmodule