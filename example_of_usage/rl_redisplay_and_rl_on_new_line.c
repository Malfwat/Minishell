/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_redisplay_and_rl_on_new_line.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:40:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/28 19:15:31 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

/* int main(void)
{
    while (1)
    {
        readline(">>");
        rl_redisplay();
        add_history(rl_line_buffer);
    }
        free(rl_line_buffer);
    return (0);
} */

/*
rl_redisplay will simply 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *line;
    char *prompt;

    // Allouer un buffer pour stocker le prompt
    prompt = malloc(sizeof(char) * 1024);

    // Boucler indéfiniment pour lire l'entrée utilisateur
        // Obtenir le répertoire de travail actuel
        getcwd(prompt, 1024);

        // Concaténer le répertoire de travail au prompt
        strcat(prompt, "> ");
    while (1) {

        // Afficher le prompt et lire une ligne d'entrée utilisateur
        line = readline(prompt);

        // Afficher la ligne d'entrée
        printf("Vous avez entré: %s\n", line);

        // Mettre à jour l'affichage de la ligne d'entrée
        rl_on_new_line();
        rl_redisplay();

        // Ajouter la ligne d'entrée à l'historique
        add_history(line);

        // Libérer la mémoire allouée pour la ligne d'entrée
        free(line);
    }
    rl_clear_history();
    // Libérer la mémoire allouée pour le prompt
    free(prompt);

    return 0;
}