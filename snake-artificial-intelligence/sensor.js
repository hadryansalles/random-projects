class Sensor{
    constructor(direcao){
        this.direcao = direcao.copy()
        this.fruta = 0
        this.parede = 0
        this.corpo = 0
    }

    atualizar(corpos, fruta){
        this.fruta = 0
        this.parede = 0
        this.corpo = 0
        let posicao = corpos[0].copy()
        posicao.add(this.direcao)
        for(let d = 1; this.parede == 0; d++){
        //for(let d = 1; this.parede == 0 && this.fruta == 0 && this.corpo == 0; d++){
            if(this.fruta == 0 && posicao.equals(fruta)){
                this.fruta = 1
            }
            if(this.corpo == 0){
                for(let i = 1; i < corpos.length; i++){
                    if(posicao.equals(corpos[i])){
                        this.corpo = 1/d
                    }
                }
            }
            if(posicao.x >= DIMENSAO || posicao.x < 0 || 
               posicao.y >= DIMENSAO || posicao.y < 0 ){
                   this.parede = 1/d
            }
            posicao.add(this.direcao)
        }
    }

    desenhar(cabeca){
        stroke(this.corpo * 255, this.fruta * 255, this.parede * 255)
        line(cabeca.x * ESCALA, cabeca.y * ESCALA, 
            (cabeca.x + (this.direcao.x * 5)) * ESCALA,
            (cabeca.y + (this.direcao.y * 5)) * ESCALA) 
    }
}