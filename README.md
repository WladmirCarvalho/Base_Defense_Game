# Base Defense - Projeto C++ com SFML

Projeto desenvolvido para a disciplina de programação do curso de Bacharelado em Tecnologia da Informação (BTI) da UFRN. 

Trata-se de um jogo de sobrevivência 2D *top-down* onde o jogador controla um herói que deve defender sua base central contra ondas progressivas de inimigos, gerenciando sua vida, integridade da base e recursos limitados de munição.

## ⚙️ Tecnologias Utilizadas
* **Linguagem:** C++ (Padrão C++17)
* **Biblioteca Gráfica/Áudio:** SFML 3.0
* **Compilador:** MinGW-w64 (GCC)
* **Build System:** Make

## 🎮 Funcionalidades Implementadas (Base 8,0 pontos)
* Movimentação do jogador baseada em coordenadas do mouse.
* Sistema de spawn aleatório de inimigos nas 4 bordas da tela.
* Disparo de projéteis com cálculo de vetores normalizados.
* Sistema de colisão preciso (`findIntersection` do SFML 3.0).
* Drop e coleta de caixas de munição.
* HUD em tempo real exibindo Vida e Munição (fonte customizada).
* Trilha sonora em loop dinâmico.

## 🚀 Incrementos e Novas Mecânicas (Fase Avançada)

Para além do escopo inicial, o projeto recebeu uma reestruturação completa de sua lógica de jogabilidade, introduzindo progressão de dificuldade, gerenciamento avançado de memória/entidades e uma batalha final:

* **Gerenciamento de Partida com Fim Definido:** A sobrevivência agora é limitada pela duração da trilha sonora (200 segundos), estabelecendo condições claras e sintonizadas de vitória.
* **Progressão de Dificuldade em 3 Estágios:** * *Estágio 1 (0s - 50s):* Ritmo cadenciado para acúmulo de recursos.
  * *Estágio 2 (50s - 110s):* Aumento do limite de inimigos em tela e aceleração do spawn.
  * *Estágio 3 (110s - 170s):* Entrada no modo frenético, com inimigos surgindo em duplas em intervalos reduzidos.
* **Sistema de Drops e Balanço de Odds:** Probabilidades refinadas para a morte de inimigos comuns (60% Munição Normal, 20% Vida, 10% MegaAmmo e 10% PowerUp), com um limitador de munição máxima fixado em 150 cartuchos (`maxAmmo`).
* **Inimigos Especiais e Inteligência do Final Boss:**
  * *MiniBoss:* Surge a cada 20 inimigos comuns, com tamanho escalado, maior quantidade de HP e dano ampliado.
  * *Final Boss:* Desativa o spawn comum aos 167s. Possui 1500 HP e uma máquina de estados que executa **teleportes aleatórios táticos** a cada 150 HP perdidos.
  * *Mecânica de Drops Dinâmicos do Chefe:* O Boss dropa `MegaAmmo` ao teleportar e libera insumos críticos (Vida e PowerUp) ao atingir 50% de seu HP (750 HP).
  * *Animação Dramática de Morte:* Ao ser derrotado, o chefe congela sua movimentação, para de atirar e executa uma animação de rotação contínua combinada com encolhimento de escala até sumir do mapa.
* **Polimento Visual e Evolução da HUD:**
  * *Muro Dinâmico da Base:* A espessura do contorno da base reage proporcionalmente à sua integridade real (Muro grosso acima de 50%, médio acima de 20%, fino abaixo disso).
  * *Danger Border:* Uma borda vermelha pulsante e translúcida é desenhada nos limites exatos da janela durante toda a batalha do chefe, gerando efeito de urgência.
  * *Telas de Status Não-Bloqueantes:* A tela de "YOU WIN!" aparece centralizada em uma caixa escura de destaque no exato momento da morte do chefe sem congelar a aplicação, permitindo que as animações de fundo e os créditos finais se encerrem de forma fluida.
  * *Créditos Finais:* Rolagem de texto automatizada na lateral esquerda da tela com parada elegante no quadrante inferior para não sobrepor a base.

## ⌨️ Controles
* **Botão Direito do Mouse:** Move o herói para o local clicado.
* **Tecla Q:** Dispara um projétil na direção do cursor do mouse.

## 🚀 Como Compilar e Rodar Localmente

1. Certifique-se de ter o MinGW e o SFML 3.0 instalados e configurados nas variáveis de ambiente (`C:\msys64\...`).
2. Abra o terminal na pasta raiz do projeto.
3. Compile o código executando o comando:
   ```powershell
   mingw32-make