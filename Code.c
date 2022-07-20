/*DSA Innovative Assignment Group Members :
    20BCE031 - Bhargav Patel
    20BCE055 - Devansh Shah
    20BCE062 - Dhruvin Varsani

    Project Title - Banking Facility
    Project Description :
    - This program allows user to create new account by giving initial amount they want to maintain
    - And by login they can make money transfer in different account, check account's previous transactions And check current balance in Account
    - User can also Check all his Previous transactions in accounts respective text file
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>                //Header file to store System's current date and time while performing transactions
#define size 10                 //Fixed defined size of array of pointers for Hashing

int g_id = 10000;               //System starts providing user id from 10000
struct hash{                    //structure for hashing
    int id;
    struct hash *next;
    struct User *add;
};

struct hash *arr[size] = {0};  //array to store pointers which points first node of respective linked lists
struct hash *last[size] = {0}; //array to store pointers which points last node of respective linked lists

void display_hash()            //function to display arrangement of all ids in Hashing
{
    for (int i = 0 ;i < size; i++)
    {
        struct hash *p = arr[i];
        printf("%d  -> ", i);
        if(p==NULL)
        {
            printf("There are no node in Hash..");
        }
        else
        {
            while(p != NULL)
        {
            printf("%d ", p-> id);
            p = p -> next;
        }
        }

        printf("\n");
    }
}

int hashing(int key)        //function to provide location of linked list in Hashing for given key by performing modulo operation
{
    return (key % size);
}
struct Time                 //Structure to store all Time component for given Transaction
{
    int date;
    int month;
    int year;
    int hour;
    int minute;
};
struct Trans                //structure for Transaction which stores different attributes for any Transaction
{
    char type;
    struct Time* t;
    float amount;
    int account;
    struct Trans* next;
};
struct User                 //Structure for user which stores ID, Password, Current amount and pointer to first and last node of respective linked list of Transaction
{
    int ID;
    char pass[50];
    float Amount;
    struct Trans *head;
    struct Trans *Tran_last;
    struct User* next;
};
struct User *first = NULL;
struct User *Last = NULL;

void insert(int Id,char *Password,float amount,int conf)        //function to add new user in system
{
    struct User *New = (struct User*)malloc(sizeof(struct User));
    New->ID = Id;
    strcpy(New->pass,Password);
    New->Amount = amount;
    New->head=NULL;
    New->next=NULL;
    if(first==NULL)
    {
        first=New;
    }
    else
    {
        Last->next=New;
    }
    Last = New;

    struct hash * temp = (struct hash *)malloc(sizeof(struct hash));
    temp -> next = NULL;
    temp-> add = New;
    temp -> id = Id;
    int pos = hashing(Id);
    if(arr[pos] == NULL)
    {
        arr[pos] = temp;
    }
    else
    {
        (last[pos]) -> next = temp;
    }
    last[pos] = temp;
    if(conf==1)
    {
        FILE *New_user;
        char File_name[50];
        sprintf(File_name,"Transactions\\%d.txt",Id);
        New_user=(fopen(File_name,"w+"));                       //new text file is created in Transactions folder named [User's id] (Ex: 10002.txt) to store its transactions
        fprintf(New_user,"-------------------------------------------------------\nAccount\t\tDate\t\tTime\t\tAmount\n-------------------------------------------------------\n");
        fclose(New_user);
        printf("\n------------------------------------------------------\nYour New Account with ID : %d is Successfully Created..\n------------------------------------------------------\n",New->ID);
    }
    g_id=Id+1;
}
struct User *find(int key)                                      //function to find given id (key) from Hashing
{
    int pos = hashing(key);
    struct User *ans = NULL;
    struct hash *p = NULL;

    if (arr[pos] != NULL)
    {
         p = arr[pos];
         while((p -> id) <= key && p != NULL)
         {
             if((p -> id) == key)
             {
                 ans = p -> add;
                 break;
             }
         }
    }
    return ans;

}
void display_all()                                              //function to display all accounts and their respective amounts at present
{
    printf("\n\t\t\t\t\t\t\t*********************************");
    printf("\n\t\t\t\t\t\t\t*   ID      *\tAmount\t\t*");
    printf("\n\t\t\t\t\t\t\t*********************************");
    if(first==NULL)
    {
        printf("\nEmpty List\n");
    }
    struct User *t;
    t=first;
    while(t != NULL)
    {
        printf("\n\t\t\t\t\t\t\t*   %d   *\t%.2f\t*",t->ID,t->Amount);
        t=t->next;
    }
    printf("\n\t\t\t\t\t\t\t*********************************\n\n");
}
void f_write(int ID,char *Password,float amount)            //function to store data of newly created account in text files for further Login process
{
    FILE *User_id;
    FILE *User_Password;
    FILE *User_amount;
    User_id = fopen("User_Details\\User_id.txt","a+");
    User_Password= fopen("User_Details\\User_Password.txt","a+");
    User_amount= fopen("User_Details\\User_amount.txt","a+");
    fprintf(User_id,"\n%d",ID);
    fprintf(User_Password,"\n%s",Password);
    fprintf(User_amount,"\n%.2f",amount);
    fclose(User_id);
    fclose(User_Password);
    fclose(User_amount);
}
void f_read()                                               //function to read previously added accounts and add them in user Linked list
{
    int id;
    char P[50];
    float amount;
    FILE *User_id;
    FILE *User_Password;
    FILE *User_amount;
    User_id = fopen("User_Details\\User_id.txt","r");
    User_Password= fopen("User_Details\\User_Password.txt","r");
    User_amount= fopen("User_Details\\User_amount.txt","r");
    while(!feof(User_id))
    {
        fscanf(User_id,"%d",&id);
        fscanf(User_Password,"%s",P);
        fscanf(User_amount,"%f",&amount);
        insert(id,P,amount,2);
    }
    fclose(User_id);
    fclose(User_Password);
    fclose(User_amount);
}

void add_trans(struct User* Sen,struct User* Rec)       //function to add Transaction nodes in both Sender and Receiver's Transaction Linked list and Write that data in both's text files
{
    float amount;
    int flag=0;
    time_t s;
    s = time(NULL);
    struct tm tm = *localtime(&s);
    struct Time *temp=(struct Time*)malloc(sizeof(struct Time));
    while(flag==0)
    {
        printf("\n\tEnter Amount of Money you want to transfer : ");
        scanf("%f",&amount);
        if(amount > Sen->Amount)
            printf("\n\tYou Don't have Sufficient balance for this Transaction..!\n");
        else if(amount <=0)
            printf("\n\tInvalid amount entered! Please Try again.");
        else
            flag=1;
    }
    char File_name[50];
    sprintf(File_name,"Transactions\\%d.txt",Sen->ID);
    char File_name2[50];
    sprintf(File_name2,"Transactions\\%d.txt",Rec->ID);

    FILE *User1;
    User1=fopen(File_name,"a+");
    fprintf(User1,"   %d\t%d/%d/%d\t%d:%d\t-%.2f\n",Rec->ID,tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min,amount);
    fclose(User1);

    FILE *User2;
    User2=fopen(File_name2,"a+");
    fprintf(User2,"   %d\t%d/%d/%d\t%d:%d\t+%.2f\n",Sen->ID,tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min,amount);
    fclose(User2);

    (Sen->Amount)-=amount;
    (Rec->Amount)+=amount;
    struct Trans *sender = (struct Trans*)malloc(sizeof(struct Trans));
    sender->type = '-';
    sender->amount = amount;
    sender->account=Rec->ID;
    sender->next=NULL;
    sender->t=temp;

    struct Trans *receiver = (struct Trans*)malloc(sizeof(struct Trans));
    receiver->type = '+';
    receiver->account=Sen->ID;
    receiver->next=NULL;
    receiver->amount = amount;
    receiver->t=temp;

    (sender->t)->hour = tm.tm_hour;(receiver->t)->hour = tm.tm_hour;
    (sender->t)->minute = tm.tm_min;(receiver->t)->minute = tm.tm_min;
    (sender->t)->date =  tm.tm_mday;(receiver->t)->date = tm.tm_mday;
    (sender->t)->month = tm.tm_mon+1;(receiver->t)->month = tm.tm_mon+1;
    (sender->t)->year = tm.tm_year+1900;(receiver->t)->year = tm.tm_year+1900;

    if(Sen->head==NULL){Sen->head=sender;}
    else {(Sen->Tran_last)->next=sender;}
    Sen->Tran_last=sender;

    if(Rec->head==NULL){Rec->head=receiver;}
    else {(Rec->Tran_last)->next=receiver;}
    Rec->Tran_last=receiver;

    printf("\n\t\t\t\t\t\t\t*****************************************");
    printf("\n\t\t\t\t\t\t\t*     Money Transfered Successfully     *");
    printf("\n\t\t\t\t\t\t\t*****************************************");
    printf("\n\t\t\t\t\t\t\t*\tSender ID : %d\t\t*",Sen->ID);
    printf("\n\t\t\t\t\t\t\t*\tReceiver ID : %d\t\t*",Rec->ID);
    printf("\n\t\t\t\t\t\t\t*\tAmount  : %.2f\t\t*",amount);
    printf("\n\t\t\t\t\t\t\t*\tDate  : %d/%d/%d\t\t*",sender->t->date,sender->t->month,sender->t->year);
    printf("\n\t\t\t\t\t\t\t*\tTime  : %d:%d\t\t\t*",sender->t->hour,sender->t->minute);
    printf("\n\t\t\t\t\t\t\t*****************************************\n\n");

}

void Disp_Trans(struct User* user)                      //function to display previously made transactions by user
{
    printf("\n\n\t\t\t\t\t\t-------------------------------------------------------\n\t\t\t\t\t\tAccount\t\tDate\t\tTime\t\tAmount\n\t\t\t\t\t\t-------------------------------------------------------\n");
    struct Trans* t;
    t=user->head;
    while(t!=NULL)
    {
        printf("\t\t\t\t\t\t  %d\t\t%d/%d/%d\t%d:%d\t\t%c%.2f\n",t->account,t->t->date,t->t->month,t->t->year,t->t->hour,t->t->minute,t->type,t->amount);
        t=t->next;
    }
}
int Login(int id)                           //function for taking User id and password from user and allow them to Login
{
    int flag=0,x=0;
    struct User* U = find(id);
    if(U!=NULL)
    {
        char p[50];
        printf("\tEnter Password : ");
        scanf("%s",p);
        if(strcmp(p,U->pass)!=0)
        {
            do{
                    printf("\n\tError! You have Entered Incorrect Password..\n\n\tPlease Try Again .: ");
                    scanf("%s",p);
                    if (strcmp(p,U->pass)==0)
                    {
                        flag=1;
                        break;
                    }
                    x++;
                }while (x<2);
                if (strcmp(p,U->pass)!=0)
                    printf("\n\tSorry - you have exhausted the maximum trials ..");
            }
            else
            {
                flag=1;
            }
            if(flag==1)
            {
               printf("\n\t\t\t\t\t\t\t---------------------------------\n\t\t\t\t\t\t\tYou Have Successfully Logged in..\n\t\t\t\t\t\t\t---------------------------------\n");
                printf("\n\n\t\t\t\t\t\t\t\tCurrent Amount : %.2f\n",U->Amount);
                return 1;
            }
            else
            {
                return 0;
            }
    }
    else
    {
        printf("\n\tUser does not Exist");
        return 0;
    }
}
int main()
{
    f_read();
    struct node *t;
    int i,j,flag;
    int x,y;
    float m;
    for(;;)
    {
        printf("\n\t\t\t\t\t\t\t*****************************************");
        printf("\n\t\t\t\t\t\t\t*\t1 : Create a New Account\t*");
        printf("\n\t\t\t\t\t\t\t*\t2 : Display All Accounts\t*");
        printf("\n\t\t\t\t\t\t\t*\t3 : Login To Existing Account\t*");
        printf("\n\t\t\t\t\t\t\t*\t4 : Exit\t\t\t*");
        printf("\n\t\t\t\t\t\t\t*****************************************");
        printf("\n\t\t\t\t\t\t\tEnter Your Choice ::  ");
        scanf("%d",&i);
        switch(i)
        {
        case 1:
            {
            int x,flag=0;
            float amount;
            char p[50];
            printf("\n\tThis is Your User ID %d: ",g_id);
            x=0;
            printf("\n\tPlease Set your password of length 8 - 32 characters.: ");
            scanf("%s",p);
            int length = strlen(p);
            if (!(length>=8 && length<=32))
            {
                do{
                    printf("\n\tError! Length of your password should be of length 8 - 32 characters.: ");
                    scanf("%s",p);
                    length = strlen(p);
                    if (length>=8 && length<=32)
                    {
                        flag=1;
                        break;
                    }
                    x++;
                }while (x<2);
                if (!(length>=8 && length<=32))
                    printf("\n\tSorry - you have exhausted the maximum trials ..");
            }
            else
            {
                flag=1;
            }
            if(flag==1)
            {
               printf("\n\tEnter Value of First Deposit in Rs.: ");
                scanf("%f",&amount);
                f_write(g_id,p,amount);
                insert(g_id,p,amount,1);
            }

            }
            break;
        case 2:
            display_all();
            break;
        case 3:
            {
                if(first==NULL)printf("\n\tCurrently There are no users..\n");
            else
            {
                printf("\n\tEnter Your User-ID : ");
                scanf("%d",&x);
                struct User *P=find(x);
                if(Login(x)==1)
                {
                    do{
                            printf("\n\t\t\t\t\t\t\t*****************************************");
                            printf("\n\t\t\t\t\t\t\t*\t1 : Transfer Money\t\t*");
                            printf("\n\t\t\t\t\t\t\t*\t2 : Display Your Transactions   *");
                            printf("\n\t\t\t\t\t\t\t*\t3 : Check current Balance\t*");
                            printf("\n\t\t\t\t\t\t\t*\t4 : Log out\t\t\t*");
                            printf("\n\t\t\t\t\t\t\t*****************************************");
                            printf("\n\t\t\t\t\t\t\tEnter Your Choice ::  ");
                            scanf("%d",&j);
                    switch(j)
                    {
                    case 1:
                        flag=0;
                        while(flag==0)
                        {
                        printf("\n\tEnter User-ID of Receiver : ");
                        scanf("%d",&y);
                        struct User *Q=find(y);
                        if(Q!=NULL && P!=Q)
                        {
                        flag=1;
                        add_trans(P,Q);
                        }
                        else if(Q==NULL) printf("\n\t********************************\n\tMentioned User does not Exist...\n\t********************************\n");
                        else if(P==Q) printf("\n\t*******************************************\n\tSender and Receiver Id s can not be same...\n\t*******************************************\n");
                        }
                        break;
                    case 2:
                        Disp_Trans(P);
                        break;
                    case 3:
                        printf("\n\n\t\t\t\t\t\t\t\t-------------------------\n");
                        printf("\t\t\t\t\t\t\t\tCurrent Amount : %.2f\n",P->Amount);
                        printf("\t\t\t\t\t\t\t\t-------------------------\n\n");
                    }
                    }
                    while(j!=4);
                }
            }
            break;
            }
        }
        if(i==4)break;
    }
}
