#include <stdio.h>
#include <string.h>
#include <dirent.h>

// Variables globales para conteo del total
int total_prio_3000 = 0;
int total_prio_6000 = 0;
int total_prio_9000 = 0;

/*
viewDataFolder: se le entrega el anno y nombre de carpeta, para luego revisar su contenido y entregar los resultados
*/
void viewDataFolder(int year, char *folder)
{
    struct dirent *entry;
    struct dirent *aux;

    DIR *directory = opendir(folder);

    if (directory == NULL)
    {
        printf("Error opening directory.\n");
        return;
    }

    int regCounter = 0;
    int prio_3000 = 0;
    int prio_6000 = 0;
    int prio_9000 = 0;

    char subPath[256];

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            strcpy(subPath, folder);
            strcat(subPath, "/");
            strcat(subPath, entry->d_name);

            DIR *subDir = opendir(subPath);
            if (subDir == NULL)
            {
                printf("Error opening subdirectory.\n");
                return;
            }

            while ((aux = readdir(subDir)) != NULL)
            {
                if (strcmp(aux->d_name, ".") != 0 && strcmp(aux->d_name, "..") != 0)
                {
                    if (strcmp(entry->d_name, "3000+") == 0)
                    {
                        prio_3000++;
                        total_prio_3000++;
                    }
                    if (strcmp(entry->d_name, "6000+") == 0)
                    {
                        prio_6000++;
                        total_prio_6000++;
                    }
                    if (strcmp(entry->d_name, "9000+") == 0)
                    {
                        prio_9000++;
                        total_prio_9000++;
                    }
                    regCounter++;
                }
            }

            if (closedir(subDir) == -1)
            {
                printf("Error closing subdirectory.\n");
                return;
            }
        }
    }

    if (closedir(directory) == -1)
    {
        printf("Error closing directory.\n");
        return;
    }

    printf("La cantidad de estudiantes de %i son: %i\n", year, regCounter);
    printf("    La cantidad de estudiantes de %i con prioridad 3000-5999 son: %i\n", year, prio_3000);
    printf("    La cantidad de estudiantes de %i con prioridad 6000-9999 son: %i\n", year, prio_6000);
    printf("    La cantidad de estudiantes de %i con prioridad 9000+ son: %i\n", year, prio_9000);
}


/*
main: ejecuta las operaciones para visualizacion de datos
*/
int main()
{
    char f_2019[] = "./2019";
    char f_2020[] = "./2020";
    char f_2021[] = "./2021";

    viewDataFolder(2019, f_2019);
    viewDataFolder(2020, f_2020);
    viewDataFolder(2021, f_2021);

    printf("Del total de estudiantes:\n");
    printf("    Existen %i con prioridad 3000-5999.\n", total_prio_3000);
    printf("    Existen %i con prioridad 6000-8999.\n", total_prio_6000);
    printf("    Existen %i con prioridad 9000+.\n", total_prio_9000);

    return 0;
}