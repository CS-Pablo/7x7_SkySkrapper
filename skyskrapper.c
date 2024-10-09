/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skyskrapper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sauron <sauron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 00:55:05 by sauron            #+#    #+#             */
/*   Updated: 2024/10/10 00:55:12 by sauron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 7

// Fonction pour vérifier combien de gratte-ciels sont visibles depuis une direction
int count_visible_buildings(int *line) {
    int max_height = 0;
    int visible_count = 0;
    for (int i = 0; i < SIZE; i++) {
        if (line[i] > max_height) {
            visible_count++;
            max_height = line[i];
        }
    }
    return visible_count;
}

// Fonction pour vérifier si une ligne respecte les indices
int is_valid_line(int *line, int clue_left, int clue_right) {
    if (clue_left && count_visible_buildings(line) != clue_left) {
        return 0;
    }
    
    int reversed_line[SIZE];
    for (int i = 0; i < SIZE; i++) {
        reversed_line[i] = line[SIZE - 1 - i];
    }
    
    if (clue_right && count_visible_buildings(reversed_line) != clue_right) {
        return 0;
    }
    
    return 1;
}

// Fonction récursive pour résoudre la grille
int solve(int **grid, int *clues, int row, int col) {
    if (row == SIZE) return 1; // Si on a rempli toutes les lignes, solution trouvée
    if (col == SIZE) return solve(grid, clues, row + 1, 0); // Si on a rempli la ligne actuelle, passer à la suivante
    
    for (int height = 1; height <= SIZE; height++) {
        int is_valid = 1;
        
        // Vérifier si cette hauteur est déjà utilisée dans la ligne ou la colonne
        for (int i = 0; i < SIZE; i++) {
            if (grid[row][i] == height || grid[i][col] == height) {
                is_valid = 0;
                break;
            }
        }
        
        if (!is_valid) continue;
        
        // Placer la hauteur
        grid[row][col] = height;
        
        // Vérifier les indices après placement
        if (col == SIZE - 1 && !is_valid_line(grid[row], clues[row], clues[SIZE + row])) {
            grid[row][col] = 0;
            continue;
        }
        
        if (row == SIZE - 1) {
            int column[SIZE];
            for (int i = 0; i < SIZE; i++) {
                column[i] = grid[i][col];
            }
            
            if (!is_valid_line(column, clues[2 * SIZE + col], clues[3 * SIZE + col])) {
                grid[row][col] = 0;
                continue;
            }
        }
        
        // Continuer la résolution
        if (solve(grid, clues, row, col + 1)) {
            return 1;
        }
        
        // Si non valide, réinitialiser la cellule
        grid[row][col] = 0;
    }
    
    return 0;
}

int **SolvePuzzle(int *clues) {
    // Initialiser la grille
    int **grid = (int **)malloc(SIZE * sizeof(int *));
    for (int i = 0; i < SIZE; i++) {
        grid[i] = (int *)calloc(SIZE, sizeof(int));
    }
    
    // Résoudre la grille
    solve(grid, clues, 0, 0);
    
    return grid;
}

void print_grid(int **grid) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int clues[28] = {
        0, 0, 1, 2, 3, 3, 4, // Haut
        4, 4, 3, 2, 2, 1, 0, // Bas
        2, 2, 2, 1, 0, 0, 4, // Gauche
        3, 2, 1, 2, 2, 3, 4  // Droite
    };
    
    int **solution = SolvePuzzle(clues);
    
    // Afficher la solution
    print_grid(solution);
    
    // Libérer la mémoire
    for (int i = 0; i < SIZE; i++) {
        free(solution[i]);
    }
    free(solution);
    
    return 0;
}
