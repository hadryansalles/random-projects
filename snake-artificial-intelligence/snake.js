class Snake{
    constructor(pesos){
        this.morreu = false
        this.tempo_viva = 0
        this.tempo_restante = 200
        
        // corpos -- DEBUGADO
        this.corpos = []
        for(let i = 0; i < 4; i++){
            this.corpos.push(createVector(DIMENSAO/2 - i, DIMENSAO/2))
        }
        
        // direcao -- DEBUGADO
        this.direcao = createVector(1, 0)

        // fruta -- DEBUGADO
        this.fruta = this.novaFruta().copy()

        // sensores -- DEBUGADO  
        this.sensores = []
        this.sensores[0] = new Sensor(createVector(0, -1))
        this.sensores[1] = new Sensor(createVector(1, -1))
        this.sensores[2] = new Sensor(createVector(1, 0))
        this.sensores[3] = new Sensor(createVector(1, 1))
        this.sensores[4] = new Sensor(createVector(0, 1))
        this.sensores[5] = new Sensor(createVector(-1, 1))
        this.sensores[6] = new Sensor(createVector(-1, 0))
        this.sensores[7] = new Sensor(createVector(-1, -1))

        // cerebro -- DEBUGADO
        this.cerebro = new RedeNeural(this.sensores.length,this.sensores.length, 3)
        if(pesos){
            this.cerebro.setPesos(pesos)
        }

        // pontuacoes
        this.score = 0
        this.pontos_movimento = 0
        this.fitness = 0
    }

    novaFruta(){
        let posicao = createVector(floor(random(0, DIMENSAO)), 
                                    floor(random(0, DIMENSAO)))
        for(let i = 0; i < this.corpos.length; i++){
            if(posicao.equals(this.corpos[i])){
                posicao = createVector(floor(random(0, DIMENSAO)), 
                                        floor(random(0, DIMENSAO)))
                i = 0
            }     
        }
        return posicao
    }

    pontuar(antiga, nova){
        if(this.fruta.dist(antiga) > this.fruta.dist(nova)){
            this.pontos_movimento += 1
        }
        else{
            this.pontos_movimento -= 1.5
        }
    }

    atualizar(){
        // movimentar cobra -- DEBUGADO
        this.tempo_viva++
        this.tempo_restante--

        if(this.tempo_restante < 0 || 
            this.colidiuParede() || 
            this.colidiuCorpo()){
            this.morreu = true
        }

        if(this.vaiComer()){
            this.corpos.push(createVector(0, 0))
            this.fruta.set(this.novaFruta())
            this.tempo_restante += 100
            this.tempo_restante = constrain(this.tempo_restante, 0, 500)
            this.score++
        }

        for(let i = this.corpos.length - 1; i > 0; i--){
            this.corpos[i].set(this.corpos[i-1])
        }
        let posicao_antiga = this.corpos[0].copy()
        this.corpos[0].add(this.direcao)
        let posicao_nova = this.corpos[0].copy()
        this.pontuar(posicao_antiga, posicao_nova)
    }

    procriarCom(snake){
        this.cerebro.procriarCom(snake.cerebro)
    }

    vaiComer(){
        return (this.corpos[0].x + this.direcao.x == this.fruta.x &&
                this.corpos[0].y + this.direcao.y == this.fruta.y)
    }

    colidiuParede(){
        return (this.corpos[0].x < 0 ||
           this.corpos[0].y < 0 ||
           this.corpos[0].x >= DIMENSAO ||
           this.corpos[0].y >= DIMENSAO)
    }

    colidiuCorpo(){
        for(let i = 1; i < this.corpos.length; i++){
            if(this.corpos[0].equals(this.corpos[i])){
                return true
            }
        }
        return false
    }

    atualizarDirecao(){
        let entradas = []
        for(let sensor of this.sensores){
            sensor.atualizar(this.corpos, this.fruta)
            //sensor.desenhar(this.corpos[0])
            entradas.push(sensor.fruta)
            entradas.push(sensor.parede)
            entradas.push(sensor.corpo)
        }

        let saida = this.cerebro.tomarDecisao(entradas)
        //console.log(saida)
        let saida_max = 0
        for(let i = 1; i < saida.length; i++){
            if(saida[i] > saida[saida_max]){
                saida_max = i
            }
        }
        switch (saida_max) {
            case 0:
                this.direcao.set(1, 0)
                break;
            case 1:
                this.direcao.set(0, 1)
                break;
            case 2:
                this.direcao.set(-1, 0)
                break;
            case 3:
                this.direcao.set(0, -1)
                break;
            default:
                break;
        }
    }

    atualizarDirecaoAngulo(){
        let entradas = []
        for(let sensor of this.sensores){
            sensor.atualizar(this.corpos, this.fruta)
            entradas.push(sensor.fruta)
            entradas.push(sensor.parede)
            entradas.push(sensor.corpo)
        }

        let saida = this.cerebro.tomarDecisao(entradas)
        let movimento = 0
        if(saida[1] > saida[0] && saida[1] > saida[2]){
            movimento = 0
        } else if(saida[0] > saida[2]){ // virar para esquerda
            movimento = 1
        
        } else { // virar para direita   
            movimento = -1
        } 
        //console.log(movimento)
        if(movimento != 0){
            if(this.direcao.x == 0 && 
                this.direcao.y == 1){
                // para baixo
                this.direcao.set(1 * movimento, 0)
            } else if(this.direcao.x == 1 && 
                    this.direcao.y == 0){
                // para direita
                this.direcao.set(0, -1 * movimento)
            } else if(this.direcao.x == 0 && 
                    this.direcao.y == -1){
                // para cima
                this.direcao.set(-1 * movimento, 0)
            } else {
                // para esquerda
                this.direcao.set(0, 1 * movimento)
            }  
        }

        this.atualizarSensores()
    }

    atualizarSensores(){
        if(this.direcao.equals(1, 0)){
            // esta para direita
            this.sensores[0].direcao.set(-1, -1)
            this.sensores[1].direcao.set(0, -1)
            this.sensores[2].direcao.set(1, -1)
            this.sensores[3].direcao.set(1, 0)
            this.sensores[4].direcao.set(1, 1)
            this.sensores[5].direcao.set(0, 1)
            this.sensores[6].direcao.set(-1, 1)
            
        } else if(this.direcao.equals(0, 1)){
            // esta para baixo
            this.sensores[0].direcao.set(1, -1)
            this.sensores[1].direcao.set(1, 0)
            this.sensores[2].direcao.set(1, 1)
            this.sensores[3].direcao.set(0, 1)
            this.sensores[4].direcao.set(-1, 1)
            this.sensores[5].direcao.set(-1, 0)
            this.sensores[6].direcao.set(-1, -1)
            
        } else if(this.direcao.equals(-1, 0)){
            // esta para esquerda
            this.sensores[0].direcao.set(1, 1)
            this.sensores[1].direcao.set(0, 1)
            this.sensores[2].direcao.set(-1, 1)
            this.sensores[3].direcao.set(-1, 0)
            this.sensores[4].direcao.set(-1, -1)
            this.sensores[5].direcao.set(0, -1)
            this.sensores[6].direcao.set(1, -1)
            
        } else {
            // esta para cima
            this.sensores[0].direcao.set(-1, 1)
            this.sensores[1].direcao.set(-1, 0)
            this.sensores[2].direcao.set(-1, -1)
            this.sensores[3].direcao.set(0, -1)
            this.sensores[4].direcao.set(1, -1)
            this.sensores[5].direcao.set(1, 0)
            this.sensores[6].direcao.set(1, 1)
        }
    }



    mutate(){
        this.cerebro.mutate()
    }

    desenhar(){
        //for(let sensor of this.sensores){
           // sensor.desenhar(this.corpos[0])
        //}
        
        fill(255, 0, 0)
        rect(this.fruta.x * ESCALA, this.fruta.y * ESCALA, ESCALA, ESCALA)

        for(let corpo of this.corpos){
            strokeWeight(2)
            fill(255, 255, 255)
            rect(corpo.x * ESCALA, corpo.y * ESCALA, ESCALA, ESCALA)
        }
    }
}