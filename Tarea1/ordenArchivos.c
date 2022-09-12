#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
fileSorter: se le entrega una carpeta y archivo de origen, lee el archivo y lo mueve a la carpeta correspondiente dependiendo
    de donde se deba ubicar
*/
void fileSorter(char *folder, char *fileName)
{
    char filePath[128];
    strcpy(filePath, fileName);
    strcat(filePath, "/");
    strcat(filePath, folder);

    FILE *f = fopen(filePath, "r");

    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int year, prio;
    fscanf(f, "%i\n%i", &year, &prio);
    fclose(f);

    char call[256] = "mv ";
    strcat(call, filePath);

    if (year == 2019)
    {
        if (prio >= 9000)
            strcat(call, " ./2019/9000+");
        else if (prio >= 6000)
            strcat(call, " ./2019/6000+");
        else if (prio >= 3000)
            strcat(call, " ./2019/3000+");
    }
    if (year == 2020)
    {
        if (prio >= 9000)
            strcat(call, " ./2020/9000+");
        else if (prio >= 6000)
            strcat(call, " ./2020/6000+");
        else if (prio >= 3000)
            strcat(call, " ./2020/3000+");
    }

    if (year == 2021)
    {
        if (prio >= 9000)
            strcat(call, " ./2021/9000+");
        else if (prio >= 6000)
            strcat(call, " ./2021/6000+");
        else if (prio >= 3000)
            strcat(call, " ./2021/3000+");
    }

    system(call);
}

/*
dirReader: lee el directorio donde se encuentran los archivos de los alumnos ejecutando fileSorter a cada uno
*/
void dirReader(char *folder)
{
    struct dirent *entry;

    char temp[128];
    strcpy(temp, "./");
    strcat(temp, folder);
    DIR *directory = opendir(temp);

    if (directory == NULL)
    {
        printf("Error opening directory.\n");
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            fileSorter(entry->d_name, temp);
        }
    }

    if (closedir(directory) == -1)
    {
        printf("Error closing directory.\n");
        return;
    }
}


/*
makeDirs: crea los directorios donde se moveran los archivos correspondientes
*/
void makeDirs()
{
    mkdir("2019", 0777);
    mkdir("2019/3000+", 0777);
    mkdir("2019/6000+", 0777);
    mkdir("2019/9000+", 0777);

    mkdir("2020", 0777);
    mkdir("2020/3000+", 0777);
    mkdir("2020/6000+", 0777);
    mkdir("2020/9000+", 0777);

    mkdir("2021", 0777);
    mkdir("2021/3000+", 0777);
    mkdir("2021/6000+", 0777);
    mkdir("2021/9000+", 0777);
}


/*
main: ejecuta las operaciones para el orden de archivos
*/
int main()
{
    char folder[] = "Archivos"; //Cambiar nombre en caso de ser necesario (solo nombre con caracteres contiguos)
    
    makeDirs();
    dirReader(folder);
    return 0;
}