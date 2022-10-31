Lucas Guimarães Miranda - RA:11070916
# [Colored Sun](https://lucasgm22.github.io/cg/coloredsun/)

## O que é?
Colored Sun se trata de uma aplicação que acompanha o movimento do mouse para exibir um degradê radial (fazendo lembrar a luz do sol no céu claro em diferentes cores), é posível movimentar o mouse para mudar centro do degradê e é possível alterar as cores usando a tecla espaço e os cliques direito e esquerdo do mouse.

## Como foi implementado?

### Estrutura
```
coloredsun/
| board.cpp
| board.hpp
| CMakeLists.txt
| main.cpp
| window.cpp
| window.hpp
|
|___assets
    | objects.frag
    |_objects.vert
```

### Window
Classe responsável por chamar as funções públicas de create, paint, resize e destroy em seus respectivos onCreate, onPaint, onResize e onDestroy, além de no Create carregar os sharders da pasta assets e implementar o onEvent para capturar os eventos de entrada do teclado e mouse.

**Funções**

- **onCreate:** Responsável por criar o programa para renderizar o Board e chamar a função create de Board;
- **onPaint:** Responsável por chamar a função paint de Board;
- **onResize:** Responsável por chamar a função resize de Board e redefinir a dimensão da janela em m_viewportSize;
- **onDestroy:** Responsável por chamar a função destroy de Board e deletar o programa.
- **onEvent:** Responsável por capturar os eventos de entrada do teclado e mouse e enviar para a classe Board poder tratar:
  - **Movimento e posição atual do mouse:** Para trocar o foco do degradê(é feito um tratamento na posição do mouse para o mapemento ser como o `gl_FragCood` (posição do pixel na janela) do fragment shader, atualiza `m_mouse` de Board;
  - **Botão esquedo do Mouse:** Trocar aleatoriamente a cor do fodo do degradê, chama a função `randomColor1` de Board;
  - **Botão direito do Mouse:** Trocar aleatoriamente cor externa do degradê, chama a função `randomColor2` de Board;
  - **Tecla espaço:** Trocar aleatoriamente ambas as cores, chama a função `randomColors` de Board.
- **restart:** Reinicia a janela;

**Atributos:**
- **m_viewportSize:** Guarda a resolução da janela;
- **m_objectsProgam:** Guarda o programa para renderizar o Board;
- **m_board:** Objeto do quadro.

### Board
Classe responsável por renderizar um quadro para ocupar toda a tela e receber o shader para aplicar a pintura em degradê.

**Funções**
- **create:** Cria o VBO e VAO do quadro, salva a localização das variávies uniformes do programa de renderização, cria toda geometria do quadro;
- **paint:** Responsável por atualizar o valor das variáveis uniformes e continuamento pintar o quadro;
- **destroy:** Libera o VBO e VAO;
- **resize:** Atualiza m_resolution com a resolução atual da tela;
- **randomColor1:** Escolhe aleatoriamente uma cor para o foco do degradê (`m_color1`);
- **randomColor2:** Escolhe aleatoriamente uma cor para o exterior do degradê (`m_color2`);
- **randomColors:** Escolhe aleatoriamente tanto o foco como o exterior do degradê;
- **randomColor:** Escolhe aleatoriamente uma cor;

**Atributos**
- **m_mouse:** Posição em pixels do mouse na tela;
- **m_mouseLoc:** Guarda a posição da variável uniforme `u_mouse` do programa de renderização;
- **m_color1:** Cor do foco do degradê;
- **m_color1Loc:** Guarda a posição da variável uniforme `u_color1` do programa de renderização;
- **m_color2:** Cor externa do degradê;
- **m_color2Loc:** Guarda a posição da variável uniforme `u_color2` do programa de renderização;
- **m_resolution:** Resolução da tela;
- **m_resolutionLoc:** Guarda a posição da variável uniforme `u_resolution` do programa de renderização;
- **m_program:** Programa de renderização para o quadro;
- **m_VAO:** VAO do quadro;
- **m_VBO:** VBO do quadro;
- **m_EBO:** EBO para os índices do quadro;
- **m_randomEngine:** Engine para gerar números pseudo-aleatórios usado na geração de cores.


### Shaders

**objects.vert**
```
#version 300 es

layout(location = 0) in vec2 inPosition;

void main() {
  gl_Position = vec4(inPosition, 0, 1);
}
```
O Vertex shader não faz nada em especial, ele simplemente mantêm as posições dos  vértices de entrada;

**objects.frag**
É onde é criado o efeito de degradê comparando a posição do pixel com a distância para a posição do mouse(`u_mouse`)
```
#version 300 es

precision mediump float;

in vec4 fragColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform vec3 u_color1;
uniform vec3 u_color2;

out vec4 outColor;

void main() {
  vec2 st = gl_FragCoord.xy/u_resolution.xy;

  float mixValue = distance(st,u_mouse/u_resolution.xy);
  vec3 color = mix(u_color1,u_color2,mixValue);

  outColor = vec4(color,1);
}
```
**u_resolution:** Resolução atual da janela;
**u_mouse:** Posição atual do mouse;
**u_color1:** Cor do centro do degradê;
**u_color2:** Cor externa do degradê;
**outColor:** Cor de saída;
**gl_FragCoord:** Coordenada do pixel;

Note como gl_FragCoord e u_mouse são dividos por u_resolution, isso é para normalizar as coordenadas no espaço [0,1]x[0,1]. 

É a calculado um valor de mix sendo a distância entre `gl_FragCoor` e `u_mouse` que é usado para fazer uma interpolação linear na função `mix` entre `u_color1` e `u_color2` e esse resultado é a cor de sa
