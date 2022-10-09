Lucas Guimarães Miranda, RA: 11070916

# REACTION GAME

Para jogar clique em play, acerte o mais rápido possível os alvos marcados com X e veja suas estatíscas!

Foi criada uma janela inicial onde aparece as estatísticas da ultima rodada e um slider de configuração para escolher a quantidade de alvos.

Foi criada uma tabela 6x6 com botões para alocar os alvos (Através de um vetor de char, ' ' posição vazia e 'X' posição com alvo), quando o jogador clica em PLAY é sorteado uma posição da tabela para alocar os alvos, ao alvo ser acertado pelo jogador é sorteado novamente para determinar uma posição para o alvo, o jogo só para quando o jogador clica em Stop.

Usando abcg::Timer foi possivel guardar o tempo de reação do jogador (tempo entre o sorteio da posiç) e fazer as estatisticas mostradas ao pausar, clicando em STOP a rodada é encerrada e o jogador pode ver as estatísticas de sua rodada como média de tempo de reação, tempo máximo, tempo mínimo, etc ...
