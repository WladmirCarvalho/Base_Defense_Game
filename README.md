# Base Defense - Projeto C++ com SFML

Projeto desenvolvido para a disciplina de programação do curso de Bacharelado em Tecnologia da Informação (BTI) da UFRN. 

Trata-se de um jogo de sobrevivência 2D *top-down* onde o jogador controla um herói que deve defender sua base central contra ondas infinitas de inimigos, gerenciando sua vida e recursos de munição.

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

## ⌨️ Controles
* **Botão Direito do Mouse:** Move o herói para o local clicado.
* **Tecla Q:** Dispara um projétil na direção do cursor do mouse.

## 🚀 Como Compilar e Rodar Localmente

1. Certifique-se de ter o MinGW e o SFML 3.0 instalados e configurados nas variáveis de ambiente (`C:\msys64\...`).
2. Abra o terminal na pasta raiz do projeto
3. Compile o código executando o comando:
   ```powershell
   mingw32-make