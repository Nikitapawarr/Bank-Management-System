#include <stdio.h>
#include <string.h>

void create_account();
void deposite_money();
void withdraw_money();
void check_balance();

const char *ACCOUNT_FILE = "info.dat";

typedef struct
{
   char name[10];
   int acc_no;
   float balance;
} Account;

int main()
{
   int choice;
   while (1)
   {
      printf("\n\n***Bank Management System***\n");
      printf("1. Create Account\n");
      printf("2. Deposit Money\n");
      printf("3. Withdraw Money\n");
      printf("4. Check Balance\n");
      printf("5. Exit\n\n");

      printf("Enter your choice ");
      scanf("%d", &choice);

      switch (choice)
      {
      case 1:
         create_account();
         break;
      case 2:
         deposite_money();
         break;
      case 3:
         withdraw_money();
         break;
      case 4:
         check_balance();
         break;
      case 5:
         printf("Bank is closed\n");
         return 0;

      default:
         printf("Invalid Input\n");
         break;
      }
   }
}
void create_account()
{
   Account acc;

   FILE *file = fopen(ACCOUNT_FILE, "ab+"); // ab+ means append binay and we can read it
   if (file == NULL)
   {
      printf("\nUnable to open file !");
      return;
   }

   char c;
   do
   {
      c = getchar();
   } while (c != '\n' && c != EOF);

   printf("\nEnter your name ");
   // fgets() is used for taking string input
   fgets(acc.name, sizeof(acc.name), stdin); // this function by default give \n at end
   int index = strcspn(acc.name, "\n");      // this is used to find the \n in the string
   acc.name[index] = '\0';                   // this assign null in place of \n

   printf("\nEnter your Account Number ");
   scanf("%d", &acc.acc_no);

   acc.balance = 0;

   fwrite(&acc, sizeof(acc), 1, file);
   fclose(file);
   printf("\nAccount Created Successfully !");
}

void deposite_money()
{
   FILE *file = fopen(ACCOUNT_FILE, "rb+");
   if (file == NULL)
   {
      printf("\nunable to open file");
      return;
   }

   int acc_num;
   float money;
   Account acc_r;
   printf("\nEnter Your Account number : ");
   scanf("%d", &acc_num);
   printf("Enter Amount to be deposite : ");
   scanf("%f", &money);

   while (fread(&acc_r, sizeof(acc_r), 1, file))
       // reading the file line by line
   {
      if (acc_r.acc_no == acc_num)
      {                                         // if acc no. matches then
         acc_r.balance += money;                // update the balance
         fseek(file, -sizeof(acc_r), SEEK_CUR); // fseek is used for putting the cursor to the starting of line
         fwrite(&acc_r, sizeof(acc_r), 1, file);
         fclose(file);
         printf("\nDeposited %.2f rs Successfully , Current Balance is %.2f rs", money, acc_r.balance);
         return;
      }
   }
   fclose(file);
   printf("\nAccount number %d not found", acc_num);
}

void withdraw_money()
{
   FILE *file = fopen(ACCOUNT_FILE, "rb+");
   if (file == NULL)
   {
      printf("\nUnable to open file !");
      return;
   }

   int acc_no;
   float money;
   Account acc_r;
   printf("\nEnter your Account number: ");
   scanf("%d", &acc_no);
   printf("\nEnter Withdrawl Amount: ");
   scanf("%f", &money);

   while (fread(&acc_r, sizeof(acc_r), 1, file))
   {
      if (acc_r.acc_no == acc_no)
      {
         if (acc_r.balance >= money)
         {
            acc_r.balance -= money;
            fseek(file , -sizeof(acc_r) ,SEEK_CUR);
            fwrite(&acc_r , sizeof(acc_r) , 1 ,file);

            printf("\nSuccessfully Withdrawn Rs %.2f , Current balance is Rs %.2f", money, acc_r.balance);
            fclose(file);
            return;
         }
         else
         {
            printf("\nInsufficient balance");
         }
         fclose(file);
         return;
      }
      else
      {
         printf("Unable to withdraw money , as account no. %d not found", acc_no);
         fclose(file);
      }
   }
}

void check_balance()
{
   FILE *file = fopen(ACCOUNT_FILE, "rb");
   if (file == NULL)
   {
      printf("\nUnable to open file !");
      return;
   }
   int acc_no;
   Account acc_read;
   printf("\nEnter your Account Number ");
   scanf("%d", &acc_no);

   while (fread(&acc_read, sizeof(acc_read), 1, file))
   {
      if (acc_no == acc_read.acc_no)
      {
         printf("\nYour current balance is Rs %.2f", acc_read.balance);
         fclose(file);
         return;
      }
   }
   fclose(file);
   printf("\nAccount no. %d was not found ", acc_no);
}
