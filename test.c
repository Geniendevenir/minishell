#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define TAILLE_BUFFER 1024

void copier_fichier(const char *source, const char *destination) {
    // Ouvrir le fichier source en lecture seule
    int fichier_source = open(source, O_RDONLY);
    if (fichier_source == -1) {
        perror("Erreur lors de l'ouverture du fichier source");
        return;
    }

    // Ouvrir le fichier destination en écriture (créer ou écraser)
    int fichier_dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fichier_dest == -1) {
        perror("Erreur lors de l'ouverture du fichier destination");
        close(fichier_source);
        return;
    }

    char buffer[TAILLE_BUFFER];
    ssize_t taille_lue;

    // Lire le contenu du fichier source et l'écrire dans le fichier destination
    while ((taille_lue = read(fichier_source, buffer, TAILLE_BUFFER)) > 0) {
        if (write(fichier_dest, buffer, taille_lue) == -1) {
            perror("Erreur lors de l'écriture dans le fichier destination");
            close(fichier_source);
            close(fichier_dest);
            return;
        }
    }

    // Vérifier si une erreur est survenue lors de la lecture
    if (taille_lue == -1) {
        perror("Erreur lors de la lecture du fichier source");
    }

    // Fermer les fichiers
    close(fichier_source);
    close(fichier_dest);
}

int main()
{
    copier_fichier("input", "output");
    return 0;
}
