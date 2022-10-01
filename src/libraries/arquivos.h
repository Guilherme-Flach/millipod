/* Helper library for saving and loading files. */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "definitions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 2*PLAYER_NAME_SIZE + 1
#define NUM_PLAYERS 5

#define NOT_FOUND -1
#define PLAYER_SEPARATOR '\n'

#define RANKING_PATH "Ranking.txt"

// Define um jogador vazio para ser usado nas listas
#define EMPTY_PLAYER (JOGADOR) {{0}, 0}

typedef struct {
    char nome[PLAYER_NAME_SIZE];
    int pontuacao;
}JOGADOR;

// All of these use the gamestate.fazendeiro.nome for reference on player name

// Save game data on <playername>.bin
int saveGame(GAMESTATE gameState);

// Load game data on <playername>.bin
int loadGame(GAMESTATE *gameState);

// Load Rankings from Ranking.txt
int loadRankingList(JOGADOR list[]);

// Checks if the player is empty
int isEmptyPlayer(JOGADOR player);

// Inserts a player into a list
int insertPlayer(JOGADOR list[], JOGADOR player);

// Checks if two players have the same name
int equalName(JOGADOR player1, JOGADOR player2);

// Checks if the given player is already on the list
int isInList(JOGADOR list[], JOGADOR player);

// Sorts the 
void sort(JOGADOR list[]);

// Saves the ranking on Ranking.txt
int saveRanking(JOGADOR list[]);

// Initializes the list with empty players 
void initializeList(JOGADOR list[]);
#endif