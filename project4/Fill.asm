@8192
D=A
@n
M=D //n = RAM[0] //iteraions to paint the screen

(START)
@SCREEN
D=A
@addr
M=D //save screen adress in addr

@i
M=0 // i=0

@KBD
D=M //bring keyboard value
@BLACK
D;JNE //jump to black if not equal to 0

(WHITE)
(LOOPW)
    @i
    D=M
    @n
    D=D-M
    @START
    D;JEQ   // if i = n goto END

    @addr
    A=M
    M=0    // RAM[addr] = 1111111111111111

    @i
    M=M+1   // i = i + 1
    @addr
    M=M+1   // addr = addr + 1
    @LOOPW
    0;JEQ   // goto LOOPW

(BLACK)
(LOOPB)
    @i
    D=M
    @n
    D=D-M
    @START
    D;JEQ   // if i == n goto END

    @addr
    A=M
    M=-1    // RAM[addr] = 1111111111111111

    @i
    M=M+1   // i = i + 1
    @addr
    M=M+1   // addr = addr + 1
    @LOOPB
    0;JMP   // goto LOOPB