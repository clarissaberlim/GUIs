# Objetivo
O programa implementa um detector de eventos de múltiplos cliques no SDL utilizando uma máquina de estados e um evento de usuário (SDL_USEREVENT).

# Lógica
A lógica é dividida em três estados:
- IDLE, quando o sistema está aguardando um primeiro clique;
- WAIT_CLICKS, quando o sistema está dentro da janela de 250 ms esperando cliques adicionais;
- CANCELLED, quando há movimento do mouse, invalidando o múltiplo clique.

A cada clique dentro do intervalo de 250 ms, o contador é incrementado e o tempo reiniciado. Caso o tempo expire sem novos cliques, o programa gera um evento personalizado (SDL_USEREVENT) cujo campo code contém o número total de cliques detectados (por exemplo, 3 para um triplo clique).

Dessa forma, o programa identifica corretamente cliques múltiplos sucessivos no mesmo ponto, ignorando casos em que o usuário move o mouse entre os cliques.
