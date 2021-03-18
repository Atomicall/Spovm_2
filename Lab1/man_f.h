struct man
{
    int money;
    char name[128];
};



bool manInit(struct man* Customer)
{
    printf("Заполняем покупателя\n");
    printf("Сколько внес денег?\n>");
    scanf("%d",&(Customer->money));
}

int show_status (int stat, int oldmoney, int newmoney, int ch);
int start_proc(char* args);



