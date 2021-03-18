//Аппарат. Имеет товары, с опр стоимостью

#define POS_AM 4
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <stdbool.h>
#include <stdlib.h>

struct position
{
    char title[50];
    int price;
    int amount;
    int left;
};

bool parceArgs(char *args, int *money, int *choise);

FILE *fileload(const char *path, const char *permissions);
void to_file(FILE *f, struct position *menu, int amount);
void from_file(FILE *fi, struct position *menu);
void parse(char *ch, char *title, int *price, int *amount, int *left);

void structout(struct position *men);

int main(int argc, char *argv[])
{
    FILE *f = fileload("temp.txt", "a+");
    char *args = calloc(50, sizeof(char));
    strcpy(args, argv[1]);
    int choise;
    int origin_money, current_money = 0;
    int exstat;
    parceArgs(args, &origin_money, &choise);
    printf("%d %d\n", origin_money, choise);
    current_money = origin_money;
    struct position *menu = (struct position *)calloc(POS_AM, sizeof(struct position));
    from_file(f, menu);
    fclose(f);
    structout(menu);
    if (current_money >= menu[choise - 1].price && menu[choise - 1].left > 0)
    {
        current_money -= menu[choise - 1].price;
        exstat = --(menu[choise - 1].left);
        //printf("Продали %sОстаток шт:%d\n", menu[choise - 1].title, --(menu[choise - 1].left));
    }
    else if (current_money < menu[choise - 1].price)
    {
        //puts("Денег нет, но вы держитесь\n");
        exstat = 199;
    }
    else
    {
        //puts("Похоже, товара нет ¯\_(ツ)_/¯");
        exstat = 200;
    }
    f = fileload("temp.txt", "w");
    to_file(f, menu, POS_AM);
    
    while (true)
    {
        rewind(stdin);
        getchar();
        exit(exstat);
    }
    if (argc) argc=0;

    return 0;
}

void parse(char *ch, char *title, int *price, int *amount, int *left)
{
    int i = 0, j = 0;
    char buff[20];
    char buff1[20];
    char buff2[20];
    char buff3[20];
    while (ch[i] == ' ')
        i++;
    while (ch[i] != '!')
    {
        buff[j++] = ch[i];
        i++;
    }
    buff[i++] = '\n';
    strcpy(title, buff);

    j = 0;
    while (ch[i] != '!')
    {
        buff1[j++] = ch[i];
        i++;
    }
    *price = atoi(buff1);
    j = 0;
    i++;
    while (ch[i] != '!')
    {
        buff2[j++] = ch[i];
        i++;
    }
    *amount = atoi(buff2);

    j = 0;
    i++;
    while (ch[i] != '\n' && ch[i] != '\0')
    {
        buff3[j++] = ch[i];
        i++;
    }
    *left = atoi(buff3);
}

bool parceArgs(char *args, int *money, int *choise)
{
    int i = 0, j = 0;
    char substr[5];
    char substr1[5];
    while (args[i] == ' ')
        i++;
    while (args[i] != ' ')
    {
        substr[j++] = args[i++];
    }
    *money = atoi(substr);
    j = 0;
    while (args[i] == ' ')
        i++;
    while (args[i] != ' ' && args[i] != '\0')
    {
        substr1[j++] = args[i++];
    }
    *choise = atoi(substr1);
    return true;
}

char *del_n(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == '\n')
            str[i] = '\0';
        i++;
    }
    return str;
}

FILE *fileload(const char *path, const char *permissions)
{
    FILE *fi = fopen(path, permissions);
    if (fi == NULL)
    {
        // printf("Проверь файл");
        return NULL;
    }
    else
    {
        // printf("OK с файлом\n");
        return fi;
    }
}

void to_file(FILE *f, struct position *menu, int amount)
{
    int i;

    rewind(f);
    for (i = 0; i < amount; i++)
    {
        char *temps = (char *)calloc(50, sizeof(char));
        menu[i].title[strlen(menu[i].title) - 1] = ' ';
        int from = strlen(menu[i].title) - 1;
        sprintf(temps, "%s!%d!%d!%d\n", menu[i].title, menu[i].price, menu[i].amount, menu[i].left);
        while (temps[from + 1] != '\n')
        {
            temps[from] = temps[from + 1];
            temps[from + 1] = ' ';
            from++;
        }
        printf("%s", temps);
        fwrite(temps, 1, strlen(temps), f);
    }
}

void from_file(FILE *fi, struct position *menu)
{

    if (fi == NULL)
    {
        // printf("Проверь файл1");
        return;
    }
    else
    {
        // printf("OK с файлом11\n");
    }
    fseek(fi, 0, 0);
    int i = 0;
    char ch[255];
    int j = 0;
    while ((ch[i] = fgetc(fi)) != EOF)
    {
        if (ch[i] == '\n')
        {

            ch[i] = '\0';
            printf("%s\n", ch);
            parse(ch, menu[j].title, &menu[j].price, &menu[j].amount, &menu[j].left);
            i = 0;
            j++;
        }
        else
            i++;
    }
    ch[i] = '\0';
    printf("%s\n", ch);
}

void structout(struct position *men)
{
    int i;
    for (i = 0; i < POS_AM; i++)
    {
        printf("%d. %s - Цена:%d Всего:%d Осталось:%d\n", i + 1, men[i].title, men[i].price, men[i].amount, men[i].left);
    }
}