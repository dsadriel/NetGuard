# NetGuard: In a world of chaos, be the firewall

![Header](.github/header.png)

## Descrição

**NetGuard** é um jogo 3D desenvolvido como trabalho final para a disciplina de **Fundamentos de Computação Gráfica (INF01047)**, ministrada pelo Prof. Eduardo Gastal na Universidade Federal do Rio Grande do Sul (UFRGS).

O objetivo do jogo é defender uma base, que representa um servidor, de uma série de ataques cibernéticos. O jogador controla a colocação de torres de defesa, que representam métodos de proteção contra invasões, e deve impedir que os inimigos, representados por invasores, cheguem até a base. O jogo é uma simulação de um cenário de defesa cibernética, onde o jogador deve usar estratégia e posicionamento para vencer as ondas de ataque. Essa abordagem lúdica visa ensinar conceitos de segurança cibernética de forma interativa e envolvente, permitindo que os jogadores aprendam sobre a importância da defesa em rede enquanto se divertem.

Ao desenvolver o NetGuard, buscamos aplicar conceitos fundamentais de computação gráfica, como renderização 3D, manipulação de texturas e iluminação, para criar uma experiência visualmente atraente e imersiva. O jogo utiliza a biblioteca **OpenGL** para renderização gráfica, permitindo a criação de ambientes tridimensionais dinâmicos e interativos.

> [!IMPORTANT]  
> O jogo ainda não está totalmente pronto, mas já é jogável e apresenta uma boa base para futuras melhorias e expansões. Estamos trabalhando para adicionar mais recursos e polir a experiência de jogo.

## Demonstração

> TODO: Imagens do jogo em ação, mostrando a base, a torre e os inimigos.

## Manual de Utilização

> TODO


## Conceitos de Computação Gráfica Aplicados

Durante o desenvolvimento do NetGuard, tivemos a oportunidade de aplicar diversos conceitos fundamentais de computação gráfica vistos na disciplina. Abaixo estão os principais conceitos implementados no projeto:

- **Conceitos matemáticos**: Utilização de álgebra linear para manipulação de vetores e matrizes, essenciais para transformações geométricas, movimentação de câmera e cálculos de iluminação.
- **Transformações geométricas**: Implementamos operações de translação, rotação e escala, compostas diretamente nos métodos de desenho dos objetos para garantir o posicionamento correto na cena.
- **Malhas poligonais**: Utilizamos o `tinyobjloader` para carregar arquivos `.obj`, com suporte a vértices, normais e coordenadas de textura. Os modelos representam inimigos, torres, cenário e outros elementos do jogo.
- **Pipeline gráfico**: Utilizamos pipeline programável com shaders escritos em GLSL, adaptados a partir dos laboratórios da disciplina. Todo o sistema de iluminação e texturização é controlado por esses shaders personalizados.
- **Shaders e modelos de iluminação**: 
    - Cada objeto possui um modelo de interpolação (Phong ou Gouraud), definido individualmente.
    - Aplicamos o modelo de iluminação Blinn-Phong, com suporte a componentes ambiente, difusa e especular.
    - Apenas uma fonte de luz é utilizada na cena, com configuração direcional.
    - Mapeamento de texturas aplicado com coordenadas carregadas diretamente dos arquivos `.obj`.
- **Animações**:
    - **Curva Bézier cúbica**: utilizada para controlar a movimentação dos inimigos ao longo de caminhos suaves e definidos.
    - **Movimentação circular da câmera**: animações orbitais em torno da base para apresentar o ambiente ao jogador.
    - **Movimentação linear em direção a um ponto-alvo**: empregada tanto para inimigos quanto para transições de câmera.
- **Câmeras**: Implementamos três modos distintos:
    - **Câmera livre**, controlada diretamente pelo jogador.
    - **Câmera look-at**, com movimento animado programado.
    - **Câmera top-down**, com visão fixa voltada à estratégia de posicionamento.
- **Testes de colisão**: No arquivo `collisions.hpp`, implementamos várias técnicas de detecção, incluindo interseção entre raios e planos, raios e triângulos, colisões entre caixas alinhadas aos eixos (AABB), esferas com caixas e entre esferas. Essas funções foram desenvolvidas para oferecer flexibilidade no projeto, mas no momento nem todas estão efetivamente utilizadas na lógica do jogo, servindo como base para expansões e experimentos futuros.


## Processo de Desenvolvimento

O desenvolvimento ocorreu de forma incremental, com etapas definidas conforme surgiam as demandas do projeto. Primeiro, foi definida a proposta do jogo em alinhamento com os requisitos da disciplina. Em seguida, foi realizada a refatoração do código base para garantir uma estrutura mais modular e organizada, facilitando futuras melhorias. Depois, foram criadas as principais abstrações em classes, como a implementação da câmera, o aprimoramento dos objetos de cena e um gerenciador básico do jogo. A partir daí, foi realizada a implementação das funcionalidades principais: modelos de iluminação, carregamento e renderização de malhas poligonais, e por fim a lógica do jogo, incluindo a movimentação dos inimigos, o posicionamento das torres e a detecção de colisões.


### Contribuições da Dupla

O trabalho foi desenvolvido em colaboração, com as seguintes contribuições principais:

- **Adriel de Souza ([@dsadriel](https://github.com/dsadriel))**: 
    > TODO
- **Nicolas Chin Lee ([@nicolasclee](https://github.com/nicolasclee))**:
    > TODO


### Análise do Uso de Ferramentas de IA

Foram empregadas ferramentas de IA para geração de assets, como texturas e modelos 3D, além de otimizar etapas do desenvolvimento. A IA também contribuiu na geração parcial da documentação, organização do projeto e na resolução de problemas técnicos, bugs e erros. As ferramentas utilizadas foram:

- **GitHub Copilot** (GPT-4.1 e Claude Sonnet 4): suporte à escrita de código, sugestões de melhorias, documentação e resolução de problemas/bugs.
- **ChatGPT** (diversos modelos): esclarecimento de dúvidas pontuais, suporte na resolução de problemas técnicos, bugs e erros.
- **Meshy**: criação de assets 3D, incluindo modelos de torres e inimigos.

No geral, as ferramentas aceleraram o desenvolvimento, especialmente no aspecto de computação gráfica. Contudo, geraram código e assets que nem sempre estavam alinhados às especificações, demandando ajustes manuais. Em problemas mais complexos, a IA falhou em oferecer soluções úteis, produzindo respostas vagas, imprecisas ou equivocadas, o que exigiu intervenção detalhada.

## Como Compilar e Executar

### Pré-requisitos

  * Um compilador C++ moderno (g++, Clang, etc.)
  * **CMake** (versão 3.10 ou superior)
  * **Make**
  * **Git** para clonar o repositório

### Passos para Execução

1.  **Clone o repositório:**

    ```bash
    git clone git@github.com:dsadriel/NetGuard.git
    cd NetGuard
    ```

2.  **Execute o script de compilação e execução:**

    No macOS, Linux ou Windows (via terminal), siga os passos abaixo para compilar e executar o projeto manualmente:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    make run
    ```

    Esses comandos irão criar a pasta `build`, gerar os arquivos de build com o CMake, compilar o projeto e executar o jogo.
