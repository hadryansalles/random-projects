const DIMENSAO = 50
const ESCALA = 10

const POPULACAO = 100
const PROCRIADORES = 4
const CHANCE_MUTACAO = 0.20

let snakes = []
let temporizador
let soma_fitness = 0

function setup(){
    createCanvas(500, 500)
    frameRate(15)
    temporizador = createSlider(1, 10, 1)
    for(let i = 0; i < POPULACAO; i++){
        snakes[i] = new Snake()
    }
}

function draw(){
    background(0, 0, 0)
    for(let n = 0; n < temporizador.value(); n++){
        let todas_mortas = true
        for(let snake of snakes){
            if(!snake.morreu){
                todas_mortas = false
                snake.atualizar()
                snake.atualizarDirecaoAngulo()    
            }
        }
        if(todas_mortas){
            novaGeracao()
        }    
    }
    for(let snake of snakes){
        if(!snake.morreu) snake.desenhar()
    }
}

function novaGeracao(){
    let novas_snakes = []
    calcFitness()
    melhoresSnake()
    for(let i = 0; i < PROCRIADORES; i++){
        novas_snakes[i] = new Snake(snakes[i].cerebro.getPesos())
    }
    for(let i = PROCRIADORES; i < POPULACAO; i++){
        //let mae = int(random(0, PROCRIADORES))
        let pai = int(random(0, PROCRIADORES))
        //let child  = new Snake(snakes[mae].procriarCom(snakes[mae]))
        //let child  = new Snake(pickOne().cerebro.getPesos())
        let child = new Snake(snakes[pai].cerebro.getPesos())
        child.mutate()
        novas_snakes[i] = child
    }
    snakes = novas_snakes.slice()
}

function melhoresSnake(){
    snakes.sort(function(a, b) {
        if(a.fitness < b.fitness){
            return 1
        }
        return -1
    })
    console.log(snakes[0].score)
}

function escolherSnake(){
    let rand = random(soma_fitness)
    let acumulador = 0
    for(let i = 0; i < snakes.length; i++){
        acumulador += snakes[i].fitness
        if(acumulador > rand){
            return snakes[i]
        }
    }
    return snakes[0]
}

function pickOne(){
    let i = 0
    let r = random(0, soma_fitness)
    while(r > 0){
        r -= (snakes[i].fitness * snakes[i].fitness)
        i++
    }
    i--
    return snakes[i]
}

function calcFitness(){
    soma_fitness = 0
    for(snake of snakes){
/*
        snake.fitness = (snake.score*snake.score) + snake.pontos_movimento 
*/
        if(snake.score < 10) {
            snake.fitness = floor(snake.tempo_viva * snake.tempo_viva) * pow(2,snake.score); 
         } else {
            snake.fitness = floor(snake.tempo_viva * snake.tempo_viva);
            snake.fitness *= pow(2,10);
            snake.fitness *= (snake.score-9);
         }
/*
         snake.fitness = snake.score + 
                        constrain(snake.pontos_movimento, 0, Infinity)
*/
        soma_fitness += snake.fitness
    }
}