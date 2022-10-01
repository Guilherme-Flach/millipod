#include "arquivos.h"

// All of these use the gamestate.fazendeiro.nome for reference on player name

// Save game data on <playername>.bin
// returns 1 on success or 0 on failure;
int saveGame(GAMESTATE gameState) {
    FILE *arq = fopen(TextFormat("%s.bin", gameState.fazendeiro.nome), "w+b");
    if (ferror(arq)){
        fclose(arq);
        return 0;
    }

    // Save the gamestate data
    fwrite(&gameState, sizeof(GAMESTATE), 1, arq);

    fclose(arq);

    if (ferror(arq)) {
        return 0;
    }
    return 1;
}

// Load game data on <playername>.bin
// returns 1 on success or 0 on failure;
int loadGame(GAMESTATE *gameState) {
    FILE *arq = fopen(TextFormat("%s.bin", gameState->fazendeiro.nome), "rb");
    if (ferror(arq)){
        fclose(arq);
        return 0;
    }

    // Load gamestate data
    fread(gameState, sizeof(GAMESTATE), 1, arq);
    fclose(arq);

    if (ferror(arq)) {
        return 0;
    }
    return 1;

}

// Load Rankings from Ranking.txt
// Returns the amount of players read
int loadRankingList (JOGADOR list[]) {
    FILE *arq;
    // Check to see if the ranking file is there:
    if (!FileExists(RANKING_PATH)) {
        // If it doesn't, create it and stop the execution
        arq = fopen(RANKING_PATH, "w+");
        fclose(arq);
        return 0;
    }
    // If it does, load data from it
    arq = fopen(RANKING_PATH, "r");
    JOGADOR temp_player;
    int num_players = 0;
    char linha[LINE_SIZE];

    // Se houve algum erro ao abrir o arquivo, retorna 0;
    if (ferror(arq)) {
        fclose(arq);
        return 0;
    }
    // Le ate chegar no final do arquivo:
    while (!feof(arq)) {
        fgets(linha, LINE_SIZE, arq);
        // Extrai os dados do jogador e salva-o na lista
        strcpy(temp_player.nome, strtok(linha, ";"));
        temp_player.pontuacao = atoi(strtok(NULL, "\0"));
        list[num_players] = temp_player;

        // Atualiza o numero de jogadores
        num_players++;
    }
    fclose(arq);

    // Ao final da leitura, retornar o numero de jogadores lidos
    return num_players;
}

// Verifica se o jogador dado e uma posicao vazia
int isEmptyPlayer (JOGADOR jogador) {
    return ((strcmp(jogador.nome, EMPTY_PLAYER.nome))
            && (jogador.pontuacao == EMPTY_PLAYER.pontuacao));
}

// Insere o jogador na lista dada, ou atualiza-o se ja existir
int insertPlayer(JOGADOR list[], JOGADOR player) {
    int num_elementos = 0,
        duplicata_index;

    // Procura um local vazio na lista ou um local onde o 
    while (num_elementos < NUM_PLAYERS && // nao estourou o limite de jogadores
           !isEmptyPlayer(list[num_elementos])) // nao achou lugar vazio)
        num_elementos++;

    // Procura para ver se o jogador ja esta na lista
    duplicata_index = isInList(list, player);

    // Se achou uma duplicata, atualiza o jogador
    if (duplicata_index != NOT_FOUND) {
        list[duplicata_index] = player;
        // Retorna a quantidade 
        return num_elementos;
    } else { // Senao, insere no final da lista (primeira posicao vazia)
        list[num_elementos] = player;

        // Retorna a posicao do jogador inserido + 1
        // (equivalente a quantidade de jogadores na lista)
        return num_elementos + 1;
    }
    
}

// Verifica se dois jogadores possuem o mesmo nome
int equalName (JOGADOR player1, JOGADOR player2) {
    return !strcmp(player1.nome, player2.nome);
}

// Passa por uma dada lista e retorna o indice da primeira instancia do jogador, ou NOT_FOUND se o jogador nao foi encontrado
int isInList(JOGADOR lista[], JOGADOR jog) {
    int duplicata_index = 0;

    // Procura por um jogador de mesmo nome na lista 
    while (duplicata_index < NUM_PLAYERS &&
           !equalName(jog, lista[duplicata_index]))
           duplicata_index++;
    
    // Se achou uma duplicata, retornar seu indice
    if (duplicata_index != NUM_PLAYERS)
        return duplicata_index;

    // Senao, retornar NOT_FOUND
    return NOT_FOUND;
}

// Ordena os jogadores em ordem decrescente de pontuacao
// (Gnome sort)
void sort(JOGADOR list[]) {
    JOGADOR player_aux;
    int player_index = 1;
    
    while(player_index < NUM_PLAYERS+1) // Account for the temp player
    {
        if(list[player_index].pontuacao > list[player_index-1].pontuacao)
        {
            player_aux = list[player_index];
            list[player_index] = list[player_index-1];
            list[player_index-1] = player_aux;
            if(player_index > 1) player_index--;
        } else
            player_index++;
    }
}

// Salva o ranking da lista dada no arquivo com o nome dado.
// Retorna o numero de jogadores salvos ou 0 se houve erro ao abrir o arquivo.
int saveRanking(JOGADOR lista[]) {
    int num_players = 0;
    FILE *arq = fopen(RANKING_PATH, "w+");

    // se houve erro, retorna 0
    if (ferror(arq)) {
        fclose(arq);
        return 0;
    }

    // Senao, passa salva os jogadores validos na lista
    // (Ate chegar no maximo de jogadores ou achar um vazio)
    while(num_players < NUM_PLAYERS && 
          !isEmptyPlayer(lista[num_players])) {
        fprintf(arq, "%s;%d\n", lista[num_players].nome, lista[num_players].pontuacao);
        num_players++;
    }
    fclose(arq);
    return num_players;
}

// Inicializa a lista com jogadores vazios
// (evita que lixo na memoria altere a execucao do programa)
void initializeList(JOGADOR list[]) {
    int player_index;
    for (player_index = 0; player_index < NUM_PLAYERS+1; player_index++)
        list[player_index] = EMPTY_PLAYER;
}
