module v16(
    input [3:0] bcd,
    output reg [3:0] bin
);
always @(bcd) begin
    bin[3]=bcd[3];
    bin[2]=bcd[2]^bcd[3];
    bin[1]=bcd[1]^bcd[2]^bcd[3];
    bin[0]=bcd[0]^bcd[1]^bcd[2]^bcd[3];
end

endmodule