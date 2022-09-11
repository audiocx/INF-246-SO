#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

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

int main()
{
    char folder[] = "Archivos de prueba";
    dirReader(folder);
    return 0;
}