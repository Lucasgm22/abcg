Lucas Guimarães Miranda - RA:11070916
# Colored Sun

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
- **onResize:** Responsável por chamar a função resize de Board;
- **onDestroy:** Responsável por chamar a função destroy de Board e deletar o programa.
- **onEvent:** Responsável por capturar os eventos de entrada do teclado e mouse e enviar para a classe Board poder tratar:
  - **Movimento e posição atual do mouse:** Para trocar o foco do degradê;
  - **Botão esquedo do Mouse:** Trocar aleatoriamente a cor do fodo do degradê;
  - **Botão direito do Mouse:** Trocar aleatoriamente cor externa do degradê;
  - **Tecla espaço:** Trocar aleatoriamente ambas as cores.
- **restart:** Reinicia a janela;

**Atributos:**
- ****

### Board
Classe responsável por renderizar um quadro para ocupar toda a tela e receber o shader para aplicar a pintura em degradê.


### Shaders
