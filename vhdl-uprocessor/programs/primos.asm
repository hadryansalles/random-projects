#       r1 -> contador
#       r2 -> parada
#       r3 -> incremento para o contador
#       r6 -> 1 registrador somente leitura nesse arquivo
#       r7 -> endereco de retorno da funcao (usado pelo montador em JRL)
        MOV   1, r6      # configuracao inicial

        # Coloca os numeros em ordem na ram
        MOV   50, r2     # Parada
        MOV    1, r3     # Incremento
        MOV    1, r1     # Inicia em 1
        loop0:
        ADD   r3, r1     # Incrementa 1 em r1
        ST.W  r1, 0[r1]  # Salva o valor de r1 em r1
        CMP   r2, r1     # Compara r1 com r2
        BLT   loop0      # Volta para intrução 3

        # remove todos os nao primos
        MOV    2, r3     # começa no numero 2
        loop1:
        JRL   remove_multiplos
        JRL   prox_nao_nulo # r3 = prox valor não nulo
        CMP   r2, r3     # compara r3 com r2
        BLT   loop1         # se r3 eh menor que r2 continua o loop
        JR    organizar_primos

        # remove todos os multiplos de r3 se eh menor que o valor de parada
        remove_multiplos: # altera o registrador r1
        MOV   r3, r1      # contador comeca no valor do incremento
        JR    loop2       # inicia sem remover a primo
        remove:
        ST.W  r0, 0[r1]   # coloca 0 no lugar que deseja remover
        loop2:
        ADD   r3, r1      # incrementa o contador
        CMP   r2, r1      # compara r1 com r2
        BLT   remove      # remove e continua o loop
        JMP   0[r7]       # retorna da funcao

        # coloca o proximo nao nulo a partir de r3 em r3
        prox_nao_nulo:    # altera os registradores r3, r5
        ADD  r6, r3       # r3 = r3 + 1
        LD.W 0[r3], r5    # r5 = RAM(r3)
        CMP r0, r5        # Compara r5 com zero
        BE  prox_nao_nulo  # Se r5 == 0, continua a busca
        # retorna o valor de r5 em r3 pois RAM(r3) = r3
        JMP   0[r7]       # retorna da funcao
        
        organizar_primos: # Coloca os primos em sequencia na ram comecando em 2
        MOV   2, r3    # o proximo nao nulo comeca em 2
        MOV   2, r1    # contador de enderecos comeca em 2
        loop3:
        ST.W r3, 0[r1] # salva o prox nao nulo no endereco do contador
        ADD  r6, r1    # incrementa o contador em 1
        JRL   prox_nao_nulo # coloca o prox nao nulo em r3
        CMP  r2, r3    # compara r3 com o valor de parada
        BLT  loop3     # continua o loop se o prox nao nulo for menor que r2
        
        # A organizar_primos garante que os primos estão em sequencia nos 
        # enderecos da ram menores que o valor de r1
        mostrar_primos:
        MOV      2, r5    # contador de enderecos comeca em 2
        loop4:
        LD.W 0[r5], r3    # coloca o primo do endereco do contador em r3
        ADD     r6, r5    # incrementa o contador
        CMP     r1, r5    # compara r5 com r1
        BLT     loop4     # se eh menor continua o loop
