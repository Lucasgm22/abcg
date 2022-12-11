Lucas Guimarães Miranda - RA:11070916
# [Cube](https://lucasgm22.github.io/cg/cube/)



## O que é?
Cube se trata de uma aplicação onde se controla um cube que pode se mover em um tabuleiro, é insperada na movimentação do jogo [Bloxers](https://www.addictinggames.com/puzzle/bloxorz), o objetivo da aplicação é explorar a matriz de modelo para criar animações.

## Como foi implementado?

### **Estrutura**
```
cube/
| cube.cpp
| cube.hpp
| ground.cpp
| groud.hpp
| CMakeLists.txt
| main.cpp
| window.cpp
| window.hpp
|
|___assets
    | box.obj
    | light.frag
    |_light.vert
```

### **Window**
Classe responsável por chamar as funções públicas de create, paint, resize, update destroy em seus respectivos onCreate, onPaint, onResize e onDestroy, além de no Create carregar os sharders da pasta assets e implementar o onEvent para capturar os eventos de entrada do teclado.

**Funções**

- **onCreate:** Responsável por criar o programa para renderizar o Board e chamar a função create de Ground e Cube, e chamar loadObj de Cube;
- **onUpdate:** Responsável por chamar a função update de Cube para recalcular a matriz de animação passando `deltaTime` como parâmetro;
- **onPaint:** Responsável por chamar a função paint de Ground e Cube;
- **onPaintUI:** Responsável por criar a UI (interface), contêm somente um box para selecionar o tipo de projeção (ortográfica ou perspectiva);
- **onResize:** Responsável por redefinir a dimensão da janela em m_viewportSize;
- **onDestroy:** Responsável por chamar a função destroy de Ground e Cube, e deletar o programa.
- **onEvent:** Responsável por capturar os eventos de entrada do teclado:
  - **Setas ou WASD:** Responsável por chamar a respectiva função que inicia o movimento de Cube;

**Atributos:**
- **m_viewportSize:** Guarda a resolução da janela;
- **m_objectsProgam:** Guarda o programa para renderizar os modelos;
- **m_scale:** Valor de scala que será aplicado em todos os modelos;
- **m_N:** Configura quantos quadriculados o Ground terá;
- **m_viewMatrix:** Matriz de visão;
- **m_viewMatrixLoc:** Localização da matriz de visão no programa de renderização;
- **m_projMatrix:** Matriz de visão;
- **m_viewMatrixLoc:** Localização da matriz de visão no programa de renderização;
- **m_colorLoc:** Localização do vetor de cor no programa de renderização; 
- **m_ground:** Objeto do Ground;
- **m_cube:** Obejeto do Cube.

### **Ground**
Classe responsável por renderizar um tabuleiro no plano `xz` onde o cubo se movimenta;

**Funções**
- **create:** Cria o VBO e VAO do tabuleiro, salva a localização das variávies uniformes do programa de renderização, cria toda geometria de um quadrado no tabuleiro;
- **paint:** Responsável por atualizar o valor das variáveis uniformes e continuamento pintar uma matrix de `2N+1 x 2N+1` quadrados em cores alternadas (branco e cinza);
- **destroy:** Libera o VBO e VAO;


**Atributos**
- **m_colorLoc:** Guarda a posição da variável uniforme `color` do programa de renderização;
- **m_program:** Programa de renderização para o Ground;
- **m_VAO:** VAO de um quadrado do tabuleiro;
- **m_VBO:** VBO de um quadrado do tabuleiro;

### **Cube**
Classe responsável por renderizar o cubo que se movimentará em cima do tabuleiro no plano `xz`;

**Funções**
- **create:** Cria o VBO, EBO e VAO do Cubo, salva a localização das variávies uniformes do programa de renderização, cria toda geometria do cubo;
- **loadObj:** Carrega o modelo do Cubo da pasta de assets;
- **createBuffers:** Carrega o VBO e EBO do Cubo;
- **paint:** Responsável por atualizar o valor das variáveis uniformes e continuamento pintar o cubo;
- **update:** Resposável por continuamente chamar a função move;
- **destroy:** Libera o VBO, VAO e EBO;
- **moveLetf:** Responsável por iniciar (inicia caso não esteja se movendo e caso o movimento não faça o cubo sair do tabuleiro) o movimento do cubo e atribuir orientação para esquerda;
- **moveRight:** Responsável por iniciar (inicia caso não esteja se movendo e caso o movimento não faça o cubo sair do tabuleiro) o movimento do cubo e atribuir orientação para direita;
- **moveUp:** Responsável por iniciar (inicia caso não esteja se movendo e caso o movimento não faça o cubo sair do tabuleiro) o movimento do cubo e atribuir orientação para cima;
- **moveDown:** Responsável por iniciar (inicia caso não esteja se movendo e caso o movimento não faça o cubo sair do tabuleiro) o movimento do cubo e atribuir orientação para baixo;
- **move:** Responsável por recalcular a matriz de rotação incrementando o ângulo do movimento do cubo (de acordo com o `deltaTime`), para realizar a animação de movimento a matriz  recebe as seguintes transformações:
    - Posiciona o modelo no eixo `x`;
    - Rotaciona o modelo com relação ao eixo `x`;
    - Posiciona novamento o modelo na origem;
    - Rotaciona o modelo de acordo com a orientão do movimento.

    Após o ângulo chear em 90º, a matriz de animação é reiniciada (recebe a matriz identidade) e o modelo é transladado na direção da orientação.
- **translate:** Translada o modelo na direção da orientação do movimento;
- **resetAnimation:** Atribui a matriz identidade à matriz de animação, reinicia o ângulo de rotação e encerra o movimento;
- **increaseAngle:** Incrementa o ângulo até o máximo de 90º;

**Atributos**

- **m_program:** Programa de renderização para Cube;
- **m_VAO:** VAO de um quadrado do cubo;
- **m_VBO:** VBO de um quadrado do cubo;
- **m_EBO:** EBO para os índices de um quadrado do cubo;
- **m_vertices:** Vertices do modelo;
- **m_indices:** Indices do modelo;
- **m_colorLoc:** Guarda a posição da variável uniforme `color` do programa de renderização;
- **m_modelMatrixLoc:** Guarda a posição da variável uniforme `modelMatrix` do programa de renderização;
- **m_positionMatrix:** Matriz responsável por transladar o cubo para a sua posição (`m_position`);
- **m_animationMatrix:** Matriz responsável pela animação de movimento do cubo;
- **m_modelMatrix:** Matriz de modelo do Cubo, no paint ela recebe o valor de `m_positionMatrix * m_animationMatix` (Primeiro o cubo é animado e depois posto na posição no mundo);
- **m_position:** Posição atual do cubo;
- **m_scale:** Fator de escala do modelo;
- **m_angle:** Ângulo atual de rotação do movimento (com relação eixo `x`);
- **m_angleVelocity:** Velocidade da alteração do ângulo no movimento;
- **m_isMoving:** Flag para indicar se o cubo está em movimento;
- **m_maxPos:** Valores absolutos máximos para Coordenadas `x` e `z`;

### Shaders

**light.vert**
```
#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - distance(vec4(0,0,0,1), modelMatrix * vec4(inPosition, 1))/1.5f;
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}
```
O Vertex Shader recebe a cor do vétice e calcula um valor de intensidade para ele de acordo com a distância do ponto para a origem, fazendo com que objetos mais distantes da origem fique mais escuro, foi criado para dar a impressão de haver uma fonte de luz acima da origem, além de aplicar as matrizes de modelo, vizão e projeção no vértice;

**light.frag**

```
#version 300 es

precision mediump float;

in vec4 fragColor;

out vec4 outColor;

void main() {
    outColor = fragColor;
}
```
O Fragment Shader não faz nada em especial, ele simplesmente atribui a cor de entrada à saida no fragmento;

