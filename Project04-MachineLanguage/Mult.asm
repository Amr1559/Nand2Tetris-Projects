//init values
@i
M=0 //i = 0

@sum
M=0 //sum = 0

(LOOP)
@i
D=M
@R1
D=D-M
@STOP
D;JEQ // i == n goto STOP

@R0
D=M
@sum
M=D+M //sum = sum + R0

@i
M=M+1 //i = i + 1

@LOOP
0;JMP

(STOP)
@sum
D=M
@R2
M=D

(END)
@END
0;JMP