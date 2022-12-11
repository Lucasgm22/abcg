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
| vertex.hpp
|
|___assets
    | box.obj
    | light.frag
    | light.vert
    |
    |___maps
        |___sky
            | negx.jpg
            | negy.jpg
            | negz.jpg
            | posx.jpg
            | posy.jpg
            |_posz.jpg
```

### **Window**
Classe responsável por chamar as funções públicas de create, paint, resize, update destroy em seus respectivos onCreate, onPaint, onResize e onDestroy, além de no Create carregar os sharders da pasta assets e implementar o onEvent para capturar os eventos de entrada do teclado.

**Funções**

- **onCreate:** Responsável por criar o programa para renderizar o Board e chamar a função create de Ground, Cube, skyBox, e chamar loadObj de Cube;
- **onUpdate:** Responsável por chamar a função update de Cube para recalcular a matriz de animação passando `deltaTime` como parâmetro;
- **onPaint:** Responsável por chamar a função paint de Ground, Cube e rederizar a skyBox;
- **onPaintUI:** Responsável por criar a UI (interface), contêm somente um box para selecionar o tipo de projeção (ortográfica ou perspectiva);
- **onResize:** Responsável por redefinir a dimensão da janela em m_viewportSize;
- **onDestroy:** Responsável por chamar a função destroy de Ground e Cube, deletar o programa e o destroy da skybox.
- **onEvent:** Responsável por capturar os eventos de entrada do teclado:
  - **Setas ou WASD:** Responsável por chamar a respectiva função que inicia o movimento de Cube;
- **createSkybox:** Cria o VBO e VAO da skyBox, salva a localização das variávies uniformes do programa de renderização, cria toda geometria do cubo;
- **renderSkybox:** Responsável por atualizar o valor das variáveis uniformes e continuamento pintar a skyBox;
- **loadSkyTexture:** Carrega a textura da skyBox dos assets;
- **destroySkybox:** Libera o VBO e VAO da skyBox;

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
- **m_cube:** Obejeto do Cube;
- **m_skyPositions:** Posições dos vértices do skyBox;
- **m_skyShaderName:** Nome do shade da skyBox ;
- **m_skyVAO:** VAO para skyBox;
- **m_skyVBO:** VBO para skyBox;
- **m_skyProgram:** Programa de renderização para a skyBox;
- **m_skyTexture:** Textura da skyBox;
- **m_lightPositionLoc:** Localização da posição da luz no programa de renderização;
- **m_lightPos:** Posição da luz pontual no mundo em `(0,1,0)`;
- **m_IaLoc:** Localização do valor da propriedade ambiente da luz no programa de renderização;
- **m_IdLoc:** Localização do valor da propriedade difuza da luz no programa de renderização;
- **m_IsLoc:** Localização do valor da propriedade especular da luz no programa de renderização;
- **m_Ia:** Valor da propriedade ambiente da luz;
- **m_Ia:** Valor da propriedade difusa da luz;
- **m_Ia:** Valor da propriedade especular da luz.

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
- **m_KaLoc:** Localização no programa de renderização da propriedade ambiente do material;
- **m_KdLoc:** Localização no programa de renderização da propriedade difusa do material;
- **m_KsLoc:** Localização no programa de renderização da propriedade ambiente do material;
- **m_KaLoc:** Valor da propriedade ambiente do material;
- **m_KdLoc:** Valor da propriedade difusa do material;
- **m_KsLoc:** Valor da propriedade ambiente do material.

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
- **computeNormals:** Calcula as normais dos vértices de acordo com os triângulos que fazem parte.


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
- **m_KaLoc:** Localização no programa de renderização da propriedade ambiente do material;
- **m_KdLoc:** Localização no programa de renderização da propriedade difusa do material;
- **m_KsLoc:** Localização no programa de renderização da propriedade ambiente do material;
- **m_KaLoc:** Valor da propriedade ambiente do material;
- **m_KdLoc:** Valor da propriedade difusa do material;
- **m_KsLoc:** Valor da propriedade ambiente do material.

### **Vertex**
Classe que implementa o objeto de vértice.

### **Shaders**
**Shaders dos objetos**

**light.vert**
```
#version 300 es

precision mediump float;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

out vec3 fragV;
out vec3 fragN;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;

  fragV = -P;
  fragN = N;

  gl_Position = projMatrix * vec4(P, 1.0);
}
```
Calcula Posição e a Normal N para passar para o fragment shader em fragV e fragN respectivamente, além de calcular a aplicar as matriz de modelview e projeção no Vértice.

**light.frag**

```
#version 300 es

precision mediump float;

uniform vec4 color;
uniform mat4 viewMatrix;

//Light Properties
uniform vec3 lightPosition;
uniform float Ia;
uniform float Id;
uniform float Is;
//Material properties
uniform float Ka;
uniform float Kd;
uniform float Ks;

uniform samplerCube cubeTex;



in vec3 fragN;
in vec3 fragV;





out vec4 outColor;

vec4 BlinnPhong(vec3 N, vec3 V) {
  N = normalize(N);
  
  vec3 L = normalize((viewMatrix * vec4(lightPosition, 1.0)).xyz + V);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, 25.0);
  }

  vec4 diffuseColor = vec4(Ka * Ia * lambertian);
  vec4 specularColor = vec4(Kd * Id * specular);
  vec4 ambientColor = vec4(Ks * Is); 

  return ambientColor + diffuseColor + specularColor;
}

void main() {
  if (gl_FrontFacing) {
    outColor = color * BlinnPhong(fragN, fragV);
  } else {
    float i = (color.r + color.g + color.b) / 3.0;
    outColor = vec4(i, 0, 0, 1.0);
  }
}

```
Calcula o sombreamendo pelo algoritmo de Blinn-Phong, note que o vetor L (Direção da incidência da luz no fragmento) é calculo aqui (É uma luz pontal posicionado no ponto `lightPosition`), L é calculado como `L = Posição da Luz - Posição do fragmento` normalizado.

**Shaders do skyBox**

**skybox.vert**
```
#version 300 es

layout(location = 0) in vec3 inPosition;

out vec3 fragTexCoord;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
  fragTexCoord = inPosition;

  vec4 P = projMatrix * viewMatrix * vec4(inPosition, 1.0);
  gl_Position = P.xyww;
}

Aplica a matriz de Visão e projeção na posição do vértice e passa a `fragTexCood` para o Vertex Shader como a posição.
```

**skybox.frag**
```
#version 300 es

precision mediump float;

in vec3 fragTexCoord;

out vec4 outColor;

uniform samplerCube skyTex;

void main() { outColor = texture(skyTex, fragTexCoord); }
```
A textura é um CubeMap, e simplesmente aplica a textura de acordo com a `fragTexCoord` vindo do Vertex Shader.
