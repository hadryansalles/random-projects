    MOV 10, $1
    MOV 20, $2
    JRL  add  # chamando a funcao

    MOV 30, $3
    MOV 40, $4
    MOV 50, $5
    JR  fim

add:
    ADD $2, $1  # $1 = $1 + $2
    JMP 0[$7]      # retorna da funcao

fim:
