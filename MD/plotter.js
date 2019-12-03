function setup() {
  createCanvas(800, 800);
  angleMode(DEGREES);
  G = {
    1: [2, 3, 4, 6, 7, 8, 9, 15, 16, 17, 20],
    2: [1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 20],
    3: [1, 5, 6, 7, 9, 10, 11, 13, 14, 18, 19],
    4: [1, 2, 7, 8, 9, 12, 15, 18],
    5: [2, 3, 6, 7, 8, 10, 11, 18],
    6: [1, 2, 3, 5, 7, 8, 12, 15, 17, 18, 19],
    7: [1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 18, 19, 20],
    8: [1, 2, 4, 5, 6, 13, 14, 15, 16, 17, 20],
    9: [1, 2, 3, 4, 7, 10, 12, 15, 18, 19],
    10: [2, 3, 5, 7, 9, 12, 14, 16, 18, 19],
    11: [2, 3, 5, 7, 14],
    12: [2, 4, 6, 7, 9, 10, 13, 15, 18],
    13: [2, 3, 8, 12, 14, 15, 16, 17, 19, 20],
    14: [2, 3, 8, 10, 11, 13, 15, 16, 17, 20],
    15: [1, 2, 4, 6, 8, 9, 12, 13, 14, 16, 18, 19, 20],
    16: [1, 8, 10, 13, 14, 15, 18, 20],
    17: [1, 2, 6, 8, 13, 14, 18],
    18: [2, 3, 4, 5, 6, 7, 9, 10, 12, 15, 16, 17, 19],
    19: [3, 6, 7, 9, 10, 13, 15, 18],
    20: [1, 2, 7, 8, 13, 14, 15, 16]
  };
  C = {
    1: 5,
    2: 1,
    3: 1,
    4: 6,
    5: 5,
    6: 4,
    7: 2,
    8: 3,
    9: 4,
    10: 6,
    11: 3,
    12: 5,
    13: 4,
    14: 5,
    15: 2,
    16: 1,
    17: 2,
    18: 3,
    19: 5,
    20: 6
  };
  max_color = 0;
  for (var v in C) {
    max_color = max(max_color, C[v]);
  }
  n_colors = max_color
  color_set = [
    [0, 0, 0]
  ]
  for (let i = 0; i < n_colors; i++) {
    color_set.push([int(255 * random()), int(255 * random()), int(255 * random())]);
  }
}

function draw() {
  background(220);
  center = [width / 2, height / 2];
  radius = min(width / 3, height / 3);
  delta_angle = 360 / Object.keys(G).length;
  i = 0;
  vs_p = [[0,0,0]]
  for (var v in C) {
    v_color = color(color_set[C[v]][0], color_set[C[v]][1], color_set[C[v]][2])
    fill(v_color);
    vs_p.push([center[0] + radius * cos(i*delta_angle),
           center[1] + radius * sin(i*delta_angle)])
    circle(vs_p[vs_p.length-1][0], vs_p[vs_p.length-1][1], 30);
    i++;
  }
  for(var v in G){
    for(var e in G[v]){
      v_color = color(color_set[C[v]][0], color_set[C[v]][1], color_set[C[v]][2])
      e_color = color(color_set[C[G[v][e]]][0], color_set[C[G[v][e]]][1], color_set[C[G[v][e]]][2])
      if(e_color == v_color){
        stroke('red');
        strokeWeight(15);
      }
      line(vs_p[v][0], vs_p[v][1], vs_p[G[v][e]][0], vs_p[G[v][e]][1])
    }
  }
}