#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livre.h"

#define delimiter "#"


int recuperer_last_id(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    int lastId = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, delimiter);
        if (token) {
            int id = atoi(token);
            if (id > lastId) lastId = id;
        }
    }
    
    fclose(file);
    return lastId;
}

GestionnaireId* initialiser_GestionnaireId() 
{
    GestionnaireId* gestionnaire = malloc(sizeof(GestionnaireId));
    if (!gestionnaire) return NULL;
    
    gestionnaire->last_livre_id = recuperer_last_id(fichier_livres);
    gestionnaire->last_editeur_id = recuperer_last_id(fichier_editeurs);
    gestionnaire->last_categorie_id = recuperer_last_id(fichier_categories);
    gestionnaire->last_auteur_id = recuperer_last_id(fichier_categories);
    gestionnaire->last_langue_id = recuperer_last_id(fichier_langues);
    
    return gestionnaire;
}

int generer_nouvel_id(GestionnaireId* gestionnaire, const char* typeEntite) 
{
    if (!gestionnaire) return -1;
    
    if (strcmp(typeEntite, "livre") == 0) 
    {
        return ++gestionnaire->last_livre_id;
    }
    else if (strcmp(typeEntite, "editeur") == 0) 
    {
        return ++gestionnaire->last_editeur_id;
    } 
    else if (strcmp(typeEntite, "categorie") == 0) 
    {
        return ++gestionnaire->last_categorie_id;
    } 
    else if (strcmp(typeEntite, "auteur") == 0) 
    {
        return ++gestionnaire->last_auteur_id;
    } 
    else if (strcmp(typeEntite, "langue") == 0) 
    {
        return ++gestionnaire->last_langue_id;
    }
    
    return -1;
}

void liberer_GestionnaireId(GestionnaireId* gestionnaire) 
{
    free(gestionnaire);
}

Livre* creer_livre(const char* titre, const char* isbn, Langue* langue, GestionnaireId* gestionnaire) 
{
    Livre* livre = malloc(sizeof(Livre));
    if (!livre) return NULL;
    
    livre->id = generer_nouvel_id(gestionnaire, "livre");
    livre->titre = strdup(titre);
    livre->isbn = strdup(isbn);
    livre->langue = langue;
    
    livre->auteurs = NULL;
    livre->nombreAuteurs = 0;
    
    livre->categories = NULL;
    livre->nombreCategories = 0;
    
    livre->editeurs = NULL;
    livre->nombreEditeurs = 0;
    
    return livre;
}

Auteur* creer_auteur(const char* prenom, const char* nom, GestionnaireId* gestionnaire) 
{
    Auteur* auteur = malloc(sizeof(Auteur));
    if (!auteur) return NULL;
    
    auteur->id = generer_nouvel_id(gestionnaire, "auteur");
    auteur->nom_auteur = strdup(nom);
    auteur->prenom_auteur = strdup(prenom);
    
    return auteur;
}

Langue* creer_langue(const char* nom, GestionnaireId* gestionnaire) 
{
    Langue* langue = malloc(sizeof(Langue));
    if (!langue) return NULL;
    
    langue->id = generer_nouvel_id(gestionnaire, "langue");
    langue->nom_langue = strdup(nom);
    
    return langue;
}

Categorie* creer_categorie(const char* nom, GestionnaireId* gestionnaire) 
{
    Categorie* categorie = malloc(sizeof(Categorie));
    if (!categorie) return NULL;
    
    categorie->id = generer_nouvel_id(gestionnaire, "categorie");
    categorie->nom_categorie = strdup(nom);
    
    return categorie;
}

Editeur* creer_editeur(const char* nom, GestionnaireId* gestionnaire) 
{
    Editeur* editeur = malloc(sizeof(Editeur));
    if (!editeur) return NULL;
    
    editeur->id = generer_nouvel_id(gestionnaire, "editeur");
    editeur->nom_editeur = strdup(nom);
    
    return editeur;
}

void ajouter_auteur_to_livre(Livre* livre, Auteur* auteur) 
{
    livre->nombreAuteurs++;
    livre->auteurs = realloc(livre->auteurs, livre->nombreAuteurs * sizeof(Auteur*));
    livre->auteurs[livre->nombreAuteurs - 1] = auteur;
}

void ajouter_categorie_to_livre(Livre* livre, Categorie* categorie) 
{
    livre->nombreCategories++;
    livre->categories = realloc(livre->categories, livre->nombreCategories * sizeof(Categorie*));
    livre->categories[livre->nombreCategories - 1] = categorie;
}

void ajouter_editeur_to_livre(Livre* livre, Editeur* editeur) 
{
    livre->nombreEditeurs++;
    livre->editeurs = realloc(livre->editeurs, livre->nombreEditeurs * sizeof(Editeur*));
    livre->editeurs[livre->nombreEditeurs - 1] = editeur;
}

void liberer_livre(Livre* livre) 
{
    if (!livre) return;
    

    free(livre->titre);
    free(livre->isbn);
    
    if (livre->auteurs) 
    {
        free(livre->auteurs);
    }
    
    if (livre->categories) 
    {
        free(livre->categories);
    }
    
    if (livre->editeurs) 
    {
        free(livre->editeurs);
    }

    free(livre);
}

int supprimer_livre(Livre** livres, int nombre_livres, int id_supprimer) 
{
    for (int i = 0; i < nombre_livres; i++) 
    {
        if (livres[i]->id == id_supprimer) 
        {
            liberer_livre(livres[i]);
            
            for (int j = i; j < nombre_livres - 1; j++) 
            {
                livres[j] = livres[j + 1];
            }
            
            return 1;
        }
    }
    return 0;
}

void modifier_livre(Livre* livre, const char* nouveauTitre, const char* nouveauIsbn, Langue* nouvelleLangue) 
{
    if (nouveauTitre) 
    {
        free(livre->titre);
        livre->titre = strdup(nouveauTitre);
    }
    
    if (nouveauIsbn) 
    {
        free(livre->isbn);
        livre->isbn = strdup(nouveauIsbn);
    }
    
    if (nouvelleLangue) 
    {
        free(livre->langue->nom_langue);
        livre->langue->nom_langue = strdup(nouvelleLangue->nom_langue);
    }
}

Langue* string_to_langue(char* str) 
{
    if (!str) return NULL;

    char* token;
    char* rest = str;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    int id = atoi(token);

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;

    Langue* langue = malloc(sizeof(Langue));
    if (!langue) return NULL;

    langue->id = id;
    langue->nom_langue = strdup(token);

    return langue;
}

char* langue_to_string(Langue* langue) 
{
    if (!langue || !langue->nom_langue) return NULL;

    char* str = malloc(DEFAULT_ALLOCATION_SIZE * sizeof(char));
    if (!str) 
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    sprintf(str, "%d%s%s", langue->id, delimiter, langue->nom_langue);
    return str;
}

Auteur* string_to_auteur(char* str) 
{
    if (!str) return NULL;

    char* token;
    char* rest = str;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    int id = atoi(token);

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;

    char* lastName = strtok_r(rest, delimiter, &rest);
    if (!lastName) return NULL;

    Auteur* auteur = malloc(sizeof(Auteur));
    if (!auteur)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    auteur->id = id;
    auteur->prenom_auteur = strdup(token);
    auteur->nom_auteur = strdup(lastName);

    return auteur;
}

char* auteur_to_string(Auteur* auteur) 
{
    if (!auteur || !auteur->prenom_auteur || !auteur->nom_auteur) return NULL;

    char* str = malloc(DEFAULT_ALLOCATION_SIZE * sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    sprintf(str, "%d%s%s%s%s", auteur->id, delimiter, auteur->prenom_auteur, delimiter, auteur->nom_auteur);
    return str;
}

Categorie* string_to_categorie(char* str) 
{
    if (!str) return NULL;

    char* token;
    char* rest = str;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    int id = atoi(token);

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;

    Categorie* categorie = malloc(sizeof(Categorie));
    if (!categorie)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    categorie->id = id;
    categorie->nom_categorie = strdup(token);

    return categorie;
}

char* categorie_to_string(Categorie* categorie) 
{
    if (!categorie || !categorie->nom_categorie) return NULL;

    char* str = malloc(DEFAULT_ALLOCATION_SIZE * sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    sprintf(str, "%d%s%s", categorie->id, delimiter, categorie->nom_categorie);
    return str;
}

Editeur* string_to_editeur(char* str) 
{
    if (!str) return NULL;

    char* token;
    char* rest = str;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    int id = atoi(token);

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;

    Editeur* editeur = malloc(sizeof(Editeur));
    if (!editeur)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    editeur->id = id;
    editeur->nom_editeur = strdup(token);

    return editeur;
}

char* editeur_to_string(Editeur* editeur) 
{
    if (!editeur || !editeur->nom_editeur) return NULL;

    char* str = malloc(DEFAULT_ALLOCATION_SIZE * sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    sprintf(str, "%d%s%s", editeur->id, delimiter, editeur->nom_editeur);
    return str;
}

Livre* string_to_livre(char* str, Langue **langues, Auteur **authors, Categorie **categories, Editeur **editeurs) // id#title#isbn#language#author1,author2#category1,category2#publisher1,publisher2
{
    if (!str) return NULL;

    char* token;
    char* rest = str;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    int id = atoi(token);

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    char* titre = token;


    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    char* isbn = token;

    token = strtok_r(rest, delimiter, &rest);
    if (!token) return NULL;
    id = atoi(token);
    
    Langue* langue = NULL;
    for (int i = 0; langues[i] != NULL; i++) 
    {
        if (langues[i]->id == id) 
        {
            langue = langues[i];
            break;
        }
    }
    if (!langue) return NULL;

    Livre* livre = malloc(sizeof(Livre));
    if (!livre)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }
    // TODO: verifier cette ligne !
    livre->id = id;
    livre->titre = strdup(titre);
    livre->isbn = strdup(isbn);
    livre->langue = langue;


    livre->auteurs = NULL;
    livre->nombreAuteurs = 0;


    token = strtok_r(rest, delimiter, &rest);
    if (token && strcmp(token, "") != 0) 
    {
        char* authorToken = strtok(token, ",");
        while (authorToken) 
        {
            Auteur* auteur = NULL;
            for (int i = 0; authors[i] != NULL; i++) 
            {
                if (strcmp(authors[i]->prenom_auteur, strtok(authorToken, " ")) == 0 &&
                    strcmp(authors[i]->nom_auteur, strtok(NULL, " ")) == 0) 
                    {
                    auteur = authors[i];
                    break;
                }
            }
            
            if (auteur) 
            {
                ajouter_auteur_to_livre(livre, auteur);
            }
            
            authorToken = strtok(NULL, ",");
        }
    }


    livre->categories = NULL;
    livre->nombreCategories = 0;
    token = strtok_r(rest, delimiter, &rest);
    if (token && strcmp(token, "") != 0) 
    {
        char* categoryToken = strtok(token, ",");
        while (categoryToken) 
        {
            Categorie* categorie = NULL;
            for (int i = 0; categories[i] != NULL; i++) 
            {
                if (strcmp(categories[i]->nom_categorie, categoryToken) == 0) 
                {
                    categorie = categories[i];
                    break;
                }
            }
            
            if (categorie) 
            {
                ajouter_categorie_to_livre(livre, categorie);
            }
            
            categoryToken = strtok(NULL, ",");
        }
    }


    livre->editeurs = NULL;
    livre->nombreEditeurs = 0;
    token = strtok_r(rest, delimiter, &rest);
    if (token && strcmp(token, "") != 0) 
    {
        char* publisherToken = strtok(token, ",");
        while (publisherToken) 
        {
            Editeur* editeur = NULL;
            for (int i = 0; editeurs[i] != NULL; i++) 
            {
                if (strcmp(editeurs[i]->nom_editeur, publisherToken) == 0) 
                {
                    editeur = editeurs[i];
                    break;
                }
            }
            
            if (editeur) 
            {
                ajouter_editeur_to_livre(livre, editeur);
            }
            
            publisherToken = strtok(NULL, ",");
        }
    }

    return livre;
}

char* livre_to_string(Livre livre) 
{
    if (!livre.titre || !livre.isbn || !livre.langue) return NULL;


    size_t bufferSize = DEFAULT_ALLOCATION_SIZE;
    bufferSize += strlen(livre.titre) + strlen(livre.isbn);
    bufferSize += strlen(livre.langue->nom_langue);


    for (int i = 0; i < livre.nombreAuteurs; i++) 
    {
        bufferSize += strlen(livre.auteurs[i]->prenom_auteur) + 
                      strlen(livre.auteurs[i]->nom_auteur) + 10;
    }


    for (int i = 0; i < livre.nombreCategories; i++) 
    {
        bufferSize += strlen(livre.categories[i]->nom_categorie) + 10;
    }


    for (int i = 0; i < livre.nombreEditeurs; i++) 
    {
        bufferSize += strlen(livre.editeurs[i]->nom_editeur) + 10;
    }

    char* str = malloc(bufferSize * sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    sprintf(str, "%d%s%s%s%s%s", livre.id, delimiter, livre.titre, delimiter, livre.isbn, delimiter);

    strcat(str, livre.langue->nom_langue);
    strcat(str, delimiter);

    if (livre.nombreAuteurs > 0) 
    {
        for (int i = 0; i < livre.nombreAuteurs; i++) 
        {
            strcat(str, livre.auteurs[i]->prenom_auteur);
            strcat(str, " ");
            strcat(str, livre.auteurs[i]->nom_auteur);
            if (i < livre.nombreAuteurs - 1) strcat(str, ",");
        }
    }
    strcat(str, delimiter);


    if (livre.nombreCategories > 0) 
    {
        for (int i = 0; i < livre.nombreCategories; i++) 
        {
            strcat(str, livre.categories[i]->nom_categorie);
            if (i < livre.nombreCategories - 1) strcat(str, ",");
        }
    }
    strcat(str, delimiter);


    if (livre.nombreEditeurs > 0) 
    {
        for (int i = 0; i < livre.nombreEditeurs; i++) 
        {
            strcat(str, livre.editeurs[i]->nom_editeur);
            if (i < livre.nombreEditeurs - 1) strcat(str, ",");
        }
    }
    return str;
}

Langue** load_langues(const char* filename, int* nombre_langues)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        *nombre_langues = 0;
        return NULL;
    }

    Langue** langues = NULL;
    *nombre_langues = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = 0;

        Langue** temp = realloc(langues, (*nombre_langues + 2) * sizeof(Langue*));
        if (!temp) 
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            fclose(file);
            free(langues);
            return NULL;
        }
        langues = temp;

        Langue* langue = string_to_langue(line);
        if (langue) 
        {
            langues[(*nombre_langues)++] = langue;
            langues[*nombre_langues] = NULL;
        }
    }

    fclose(file);
    return langues;
}

Auteur** load_auteurs(const char* filename, int* nombre_auteurs)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        *nombre_auteurs = 0;
        return NULL;
    }

    Auteur** auteurs = NULL;
    *nombre_auteurs = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) 
    {

        line[strcspn(line, "\n")] = 0;

        Auteur** temp = realloc(auteurs, (*nombre_auteurs + 2) * sizeof(Auteur*));
        if (!temp) 
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            fclose(file);
            free(auteurs);
            return NULL;
        }
        auteurs = temp;

        Auteur* auteur = string_to_auteur(line);
        if (auteur) 
        {
            auteurs[(*nombre_auteurs)++] = auteur;
            auteurs[*nombre_auteurs] = NULL;
        }
    }

    fclose(file);
    return auteurs;
}

Categorie** load_categories(const char* filename, int* nombre_categories)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        *nombre_categories = 0;
        return NULL;
    }

    Categorie** categories = NULL;
    *nombre_categories = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) 
    {

        line[strcspn(line, "\n")] = 0;

        Categorie** temp = realloc(categories, (*nombre_categories + 2) * sizeof(Categorie*));
        if (!temp) 
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            fclose(file);
            free(categories);
            return NULL;
        }
        categories = temp;

        Categorie* categorie = string_to_categorie(line);
        if (categorie) 
        {
            categories[(*nombre_categories)++] = categorie;
            categories[*nombre_categories] = NULL;
        }
    }

    fclose(file);
    return categories;
}

Editeur** load_editeurs(const char* filename, int* nombre_editeurs)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        *nombre_editeurs = 0;
        return NULL;
    }

    Editeur** editeurs = NULL;
    *nombre_editeurs = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) 
    {

        line[strcspn(line, "\n")] = 0;

        Editeur** temp = realloc(editeurs, (*nombre_editeurs + 2) * sizeof(Editeur*));
        if (!temp) 
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            fclose(file);
            free(editeurs);
            return NULL;
        }
        editeurs = temp;

        Editeur* editeur = string_to_editeur(line);
        if (editeur) 
        {
            editeurs[(*nombre_editeurs)++] = editeur;
            editeurs[*nombre_editeurs] = NULL;
        }
    }

    fclose(file);
    return editeurs;
}

Livre** load_livres(const char* filename, Langue** langues, Auteur** auteurs, 
                       Categorie** categories, Editeur** editeurs, int* nombre_livres) 
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        *nombre_livres = 0;
        return NULL;
    }

    Livre** livres = NULL;
    *nombre_livres = 0;

    char line[2048];
    while (fgets(line, sizeof(line), file)) 
    {

        line[strcspn(line, "\n")] = 0;

        Livre** temp = realloc(livres, (*nombre_livres + 2) * sizeof(Livre*));
        if (!temp) 
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            fclose(file);
            free(livres);
            return NULL;
        }
        livres = temp;

        Livre* livre = string_to_livre(line, langues, auteurs, categories, editeurs);
        if (livre) 
        {
            livres[(*nombre_livres)++] = livre;
            livres[*nombre_livres] = NULL;
        }
    }

    fclose(file);
    return livres;
}

int save_langues(const char* filename, Langue** langues, int nombre_langues) 
{
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < nombre_langues - 1; i++) 
    {
        char* str = langue_to_string(langues[i]);
        if (str) 
        {
            fprintf(file, "%s\n", str);
            free(str);
        }
    }
    char* str = langue_to_string(langues[nombre_langues - 1]);
        if (str) 
        {
            fprintf(file, "%s", str);
            free(str);
        }

    fclose(file);
    return 1;
}

int save_auteurs(const char* filename, Auteur** auteurs, int nombre_auteurs)
{
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < nombre_auteurs - 1; i++) 
    {
        char* str = auteur_to_string(auteurs[i]);
        if (str) 
        {
            fprintf(file, "%s\n", str);
            free(str);
        }
    }

    char* str = auteur_to_string(auteurs[nombre_auteurs - 1]);
        if (str) 
        {
            fprintf(file, "%s", str);
            free(str);
        }

    fclose(file);
    return 1;
}

int save_categories(const char* filename, Categorie** categories, int nombre_categories)
{
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < nombre_categories - 1; i++) 
    {
        char* str = categorie_to_string(categories[i]);
        if (str) 
        {
            fprintf(file, "%s\n", str);
            free(str);
        }
    }

    char* str = categorie_to_string(categories[nombre_categories - 1]);
        if (str) 
        {
            fprintf(file, "%s", str);
            free(str);
        }

    fclose(file);
    return 1;
}

int save_editeurs(const char* filename, Editeur** editeurs, int nombre_editeurs)
{
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < nombre_editeurs - 1; i++) 
    {
        char* str = editeur_to_string(editeurs[i]);
        if (str) 
        {
            fprintf(file, "%s\n", str);
            free(str);
        }
    }

    char* str = editeur_to_string(editeurs[nombre_editeurs - 1]);
        if (str) 
        {
            fprintf(file, "%s", str);
            free(str);
        }

    fclose(file);
    return 1;
}

int save_livres(const char* filename, Livre** livres, int nombre_livres)
{
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < nombre_livres - 1; i++) 
    {
        char* str = livre_to_string(*livres[i]);
        if (str) 
        {
            fprintf(file, "%s\n", str);
            free(str);
        }
    }

    char* str = livre_to_string(*livres[nombre_livres - 1]);
        if (str) 
        {
            fprintf(file, "%s", str);
            free(str);
        }

    fclose(file);
    return 1;
}
