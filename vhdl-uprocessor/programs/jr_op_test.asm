    MOV 10, $1
    JR   loop
outro:
    MOV 30, $3
    JR  fim
loop:
    MOV 20, $2
    JR  outro
#
fim:
    MOV 40, $4
