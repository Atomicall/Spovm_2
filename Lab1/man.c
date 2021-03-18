//Человек. Выбирает товар, вносит деньги. Вызывает аппарат
#include "man_main.h"
#include "man_f.h"
#define machine_patch "./machine"

bool leavethisshit = 0;

void pr_time()
{
   struct tm *tm_p;
   time_t t;
   t = time(NULL);
   tm_p = localtime(&t);
   printf(" %02d:%02d:%02d\n", tm_p->tm_hour, tm_p->tm_min, tm_p->tm_sec);
   return;
}

int main()
{
   int ch;
   int j ;
   struct man User;
   manInit(&User);
   char *args;
   printf("В случае неправильного ввода список появится заново\n");
   int prices[4] = {15, 10, 30, 20};
   do
   {
      leavethisshit=0;
      args = (char *)calloc(50, sizeof(char));
      j=0;
      sprintf(args, "%d", User.money); // <=> cin >> args
      
   
      do
      {
         printf("1. Товар_1\n");
         printf("2. Товар_2\n");
         printf("3. Товар_3\n");
         printf("4. Товар_4\n");
         printf("5. Выйти\n");
         printf(">");
         scanf("%d", &ch);
         
      } while (ch < 1 || ch > 5);
      if (ch == 5)
      {
         leavethisshit = 1;
         break;
        
      }
      while (args[j] != '\0')
         j++;
      args[j++] = ' ';
      args[j++] = ch + 48;
      args[j++] = ' ';
      args[j++] = '\n';
      puts("Будут переданы аргументы");
      puts(args);
      printf("%s", "Ожидаем ввода символа для завершения...");
      int status = start_proc(args);
      User.money = show_status(status, User.money, User.money - prices[ch - 1], ch);
      free(args);
      
   } while (!leavethisshit);
}


int start_proc(char *args)
{
   int status;
   int ext;
   int exitcode;
   pid_t childp = fork();
   switch (childp)
   {
   case 0:
   {
      int err = execlp(machine_patch, machine_patch, args, NULL);
      if (err == -1)
      {
         puts("Exec не отработал");
         exit(-1);
      }
      break;
   }

   case -1:
   {
      printf("Проблема при создании процесса. Попробуйте снова позже\n");
      break;
   }

   default:
   {
      printf("Родительский процесс. fork() вернул:\n>");
      printf("%d\n", (int)childp);

      do
      {

         waitpid(childp, &status, 1);
         //system("clear");
         pr_time();
         ext = WIFEXITED(status);
         if (ext)
         {
            printf("S> Exit code = %d\n", (exitcode = WEXITSTATUS(status)));
         }
         sleep(1);
      } while (!ext);
      return exitcode;
      break;
   }
   }
   return 0;
}

int show_status(int stat, int oldmoney, int newmoney, int ch)
{
   int left = oldmoney;
   switch (stat)
   {
   default:
   {
      printf("Продали позицию  %d Остаток шт:%d\n", ch, stat);
      left = newmoney;
      break;
   }
   case 200:
   {
      puts("Похоже, товара нет ¯|_(ツ)_/¯");
      break;
   }
   case 199:
   {
      puts("Денег не хватает, но вы держитесь\n");
      break;
   }
   }
   printf("Осталось денег - %d\n", left);
   return left;
}