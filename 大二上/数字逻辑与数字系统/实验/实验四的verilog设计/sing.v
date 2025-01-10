module sing(
    input clk,          // 时钟信号
    input reset,        // 复位信号
    input [0:7] keys,   // 8个开关输入
    output reg speaker,  // 扬声器控制信号
	 output reg [0:7] freq,
     output reg [0:7] temp
);

// 定义音频频率常量
parameter FREQ_C1 = 190;  // C1 频率 (262 Hz)
parameter FREQ_D1 = 170;  // D1 频率 (294 Hz)
parameter FREQ_E1 = 152;  // E1 频率 (330 Hz)
parameter FREQ_F1 = 143;  // F1 频率 (349 Hz)
parameter FREQ_G1 = 128;  // G1 频率 (392 Hz)
parameter FREQ_A1 = 114;  // A1 频率 (440 Hz)
parameter FREQ_B1 = 101;  // B1 频率 (494 Hz)
parameter FREQ_C2 = 96;  // C2 频率 (523 Hz)

// 计算计数器模值
always@(keys)begin
		freq <= (keys[0]) ? FREQ_C1 :
              (keys[1]) ? FREQ_D1 :
              (keys[2]) ? FREQ_E1 :
              (keys[3]) ? FREQ_F1 :
              (keys[4]) ? FREQ_G1 :
              (keys[5]) ? FREQ_A1 :
              (keys[6]) ? FREQ_B1 :
              (keys[7]) ? FREQ_C2 : 8'd0;
end
always@(posedge clk)begin
    if(temp==freq-1)begin
        speaker<=~speaker;
        temp<=8'd0;
    end
    else begin
    temp<=temp+1;
    end
end
endmodule