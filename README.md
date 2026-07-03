# Base Defense - Projeto C++ com SFML

Projeto desenvolvido para a disciplina de programação do curso de Bacharelado em Tecnologia da Informação (BTI) da UFRN. 

Trata-se de um jogo de sobrevivência tático 2D *top-down* com dinâmica **Time Attack**. O jogador controla um herói que deve defender sua base central contra ondas progressivas de inimigos, com o objetivo mandatório de eliminar o Final Boss antes que o tempo se esgote.

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

## 🚀 Incrementos Avançados (Modo Time Attack)

O projeto passou por uma reestruturação arquitetural completa para integrar os sistemas de Menu Inicial, Leaderboard persistente e uma progressão de hordas com fim definitivo:

* **Sistema de Jogo Baseado em Estados (State Machine):** Gerenciamento fluido entre as telas de `START` (Menu), `PLAYING` (Gameplay), `ENTER_NAME` (Inclusão de Recorde) e `GAME_OVER`, tratando transições de áudio e eventos periféricos de forma isolada.
* **Cronômetro Regressivo e Condição de Vitória:** A HUD exibe um timer em contagem regressiva sintonizado com a duração da trilha sonora (200 segundos). A vitória só é computada se o Final Boss for inteiramente destruído antes do relógio zerar.
* **Três Condições Distintas de Defeat:** O encerramento abrupto da partida ocorre se:
  1. A integridade estrutural da Base Central chegar a zero.
  2. Os pontos de vida (`HP`) do Herói zerarem.
  3. O tempo limite esgotar e o Final Boss ainda estiver ativo no mapa (Estouro de Tempo).
* **Progressão de Dificuldade Escalável:**
  * *Estágio 1 (0s - 50s):* Cadência padrão de inimigos comuns para gerenciamento de recursos.
  * *Estágio 2 (50s - 110s):* Aceleração do intervalo de spawn e elevação do limite de entidades simultâneas.
  * *Estágio 3 (110s - 170s):* Modo frenético com spawns duplicados e introdução de *MiniBosses* a cada 20 eliminações comuns.
* **Sistema de Recompensas e Suprimentos (Boosts):** Inimigos comuns e chefes dropam itens pelo mapa que são fundamentais para manter a cadência de tiro e a vida do herói. A probabilidade de drop é calculada dinamicamente, englobando quatro tipos:
  * 📦 **Normal Ammo (Munição Comum - Verde):** Restaura 5 cartuchos de munição.
  * 🔋 **Mega Ammo (Munição Pesada - Amarelo):** Restaura 20 cartuchos de munição de uma só vez.
  * 💊 **Health (Cura - Magenta):** Recupera 50 pontos de vida do herói (respeitando o teto de 100 HP).
  * ⚡ **Power-Up (Sobrecarga - Ciano):** Ativa um estado especial temporário de 10 segundos onde os projéteis disparam com dano triplicado (de 20 para 60 de dano por acerto).
* **Inteligência do Final Boss e Drops de Suporte:** O Boss surge aos 170s, bloqueia o spawn de hordas comuns e executa **teleportes táticos aleatórios** a cada 150 HP perdidos. Ele dropa suprimentos críticos (`MegaAmmo` e `Health`) em marcos específicos de sua vida para manter o dinamismo técnico do combate.
* **Leaderboard Persistente em Ordem Ascendente:** O sistema lê e salva dados em arquivo de texto. Ajustado para o modelo *Time Attack*, o placar filtra exclusivamente as vitórias do jogador e **ordena de forma crescente** (menor tempo de conclusão no topo do ranking).
* **Polimento de UI e UX:**
  * *Contorno Dinâmico:* A espessura do muro da base reage fisicamente à sua vida restante.
  * *Danger Border:* Borda vermelha pulsante translúcida sintonizada via função seno matemática durante a fase do chefe.
  * *Instruções de Jogo Integradas:* Textos guias posicionados nativamente e de forma exclusiva no Menu Inicial para manter a área de combate limpa.

## ⌨️ Controles
* **Botão Direito do Mouse:** Move o herói para o local clicado.
* **Tecla Q:** Dispara um projétil na direção do cursor do mouse.

## 🚀 Como Compilar e Rodar Localmente

1. Certifique-se de ter o MinGW e o SFML 3.0 instalados e configurados nas variáveis de ambiente.
2. Abra o terminal na pasta raiz do projeto.
3. Compile o código executando o comando:
   ```powershell
   mingw32-make