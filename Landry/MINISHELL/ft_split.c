/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <larakoto@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:55:10 by larakoto          #+#    #+#             */
/*   Updated: 2024/07/31 17:49:16 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Fonction pour vérifier si un caractère est un des séparateurs
bool is_separator(char c) {
    return (c == '|' || c == ' ' || c == '<' || c == '>');
}

// Fonction pour compter le nombre de mots + séparateurs
static size_t count_words(char const *s) {
    size_t count = 0;
    size_t a = 0;

    while (s[a]) {
        // Ignorer les espaces successifs
        while (s[a] == ' ')
            a++;
        if (s[a]) {
            // Si c'est un séparateur, il compte comme un token
            if (is_separator(s[a]) && s[a] != ' ') {
                count++;
                a++;
            }
            // Sinon, c'est un mot
            else {
                count++;
                while (s[a] && !is_separator(s[a]))
                    a++;
            }
        }
    }
    return count;
}

// Fonction pour remplir chaque token dans le tableau
static void fill_word(char *new, char const *s, size_t start, size_t len) {
    size_t a = 0;
    while (a < len) {
        new[a] = s[start + a];
        a++;
    }
    new[a] = '\0';
}

// Fonction pour réaliser le découpage de la chaîne
static void split_words(char **tab, char const *s) {
    size_t index = 0;
    size_t i = 0;
    size_t word_start = 0;

    while (s[index])
     {
        while (s[index] == ' ')
            index++;
        word_start = index;

        if (is_separator(s[index]) && s[index] != ' ') {
            tab[i] = malloc(2 * sizeof(char));  // 1 caractère + '\0'
            tab[i][0] = s[index];
            tab[i][1] = '\0';
            i++;
            index++;
        }
        else {
            while (s[index] && !is_separator(s[index]))
                index++;
            if (index > word_start) {
                tab[i] = malloc(sizeof(char) * (index - word_start + 1));
                fill_word(tab[i], s, word_start, index - word_start);
                i++;
            }
        }
    }
    tab[i] = NULL;
}

char **ft_split(char *s) {
    size_t w;
    char **tab;

    w = count_words(s);
    tab = malloc(sizeof(char *) * (w + 1));
    if (!tab)
        return NULL;
    split_words(tab, s);
}

 
