class RedeNeural{
    constructor(entradas, ocultas, saidas){
        this.modelo = new synaptic.Architect.Perceptron(entradas, ocultas, saidas)
    }

    setPesos(pesos){
        this.modelo = new synaptic.Architect.Perceptron.fromJSON(pesos)
    }

    getPesos(){
        return this.modelo.toJSON()
    }

    tomarDecisao(entradas){
        //console.log(entradas)
        let saida = this.modelo.activate(entradas)
        //console.log(saida)
        return saida
    }

    procriarCom(cerebro){
        let json_parceiro = cerebro.getPesos()
        let json_filho = this.modelo.toJSON()
        let r = random(0, json_filho.connections.length)
        for(let i = 0; i < json_filho.connections.length; i++){
            if(i < r){
                json_filho.connections[i].weight = json_parceiro.connections[i].weight
            }
        }
        return json_filho
    }
    

    mutate(){
        let pesos = this.modelo.toJSON()
        for(let i = 0; i < pesos.connections.length; i++){
            if(random() < CHANCE_MUTACAO){
                pesos.connections[i].weight = randomGaussian()
            }
        }
        this.modelo = new synaptic.Architect.Perceptron.fromJSON(pesos)
    }
}