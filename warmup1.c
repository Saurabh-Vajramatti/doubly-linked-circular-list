#include "my402list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h> 

typedef struct tagTransactionRecord {
    char transactionType;
    time_t transactionTime;
    double transactionAmount;
    char transactionDescription[1024];
} TransactionRecord;

/* ----------------------- main() ----------------------- */

void displayTransaction(TransactionRecord* transactionRecord)
{
    printf("Transaction:\n");
    printf("%c|",transactionRecord->transactionType);
    printf("%lu|",transactionRecord->transactionTime);
    printf("%f|",transactionRecord->transactionAmount);
    printf("%s",transactionRecord->transactionDescription);
    printf("\n");
}

void PrintTestList(My402List *pList, int num_items)
{
    My402ListElem *elem=NULL;

    if (My402ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in PrintTestList().\n", num_items);
        exit(1);
    }

    printf("LIST:");
    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem)) {
        TransactionRecord* ival=elem->obj;
        printf("%s",ival->transactionDescription);
        displayTransaction(ival);
    }
    fprintf(stdout, "\n");
}

void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

void BubbleSortForwardList(My402List *pList, int num_items)
{
    My402ListElem *elem=NULL;
    int i=0;

    if (My402ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(1);
    }
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
            TransactionRecord* transactionRecord=elem->obj;
            time_t cur_val=transactionRecord->transactionTime;

            next_elem=My402ListNext(pList, elem);
            transactionRecord=next_elem->obj;
            time_t next_val = transactionRecord->transactionTime;

            if (cur_val==next_val)
            {
                fprintf(stderr,"Timestamp is duplicate.\n");
                exit(1);
            }
            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}


void moneyFormat(char balance[15],double currentBalance)
{
    balance[14]='\0';

        double bal=currentBalance;

        if(bal<0)
        {
            balance[0]='(';
            balance[13]=')';
        }
        else
        {
            balance[0]=' ';
            balance[13]=' ';
        }

        if(bal>=10000000 || bal<=-10000000)
        {
            balance[1]='?';
            balance[2]=',';
            balance[3]='?';
            balance[4]='?';
            balance[5]='?';
            balance[6]=',';
            balance[7]='?';
            balance[8]='?';
            balance[9]='?';
            balance[10]='.';
            balance[11]='?';
            balance[12]='?';
        }

        else
        {
            int balMul100=round(bal*100);
            balMul100=abs(balMul100);
            int b1=balMul100%10;
            balMul100=balMul100/10;

            balance[12]='0'+b1;

            int b2=balMul100%10;
            balMul100=balMul100/10;
            balance[11]='0'+b2; 

            balance[10]='.';

            int x=9;

            int tripleCount=0;
            if(balMul100%10==0)
            {
                balance[x]='0';
                x=x-1;
                tripleCount=tripleCount+1;
                balMul100=balMul100/10;
            }

            while(balMul100!=0)
            {
                if(tripleCount==3)
                {
                    tripleCount=0;
                    balance[x]=',';
                    x=x-1;
                }
                int curDig=balMul100%10;
                balance[x]='0'+curDig;

                balMul100=balMul100/10;
                x=x-1;
                tripleCount=tripleCount+1;
            } 

            while(x!=0)
            {
                balance[x]=' ';
                x=x-1;
            }
        }
}

int main(int argc, char *argv[])
{
    My402List transactionList;

    memset(&transactionList, 0, sizeof(My402List));
    (void)My402ListInit(&transactionList);

    if(argc==3 || argc==2)
    {
        char recordBuffer[1025];
        const char delim[]="\t";
        char* field;

        FILE* filePointer;

        if(argc==3)
        {
            filePointer=fopen(argv[2],"r");
        }

        if(argc==2)
        {
            filePointer=stdin;
        }

        if(filePointer==NULL)
        {
            fprintf(stderr,"Error while opening the file\n");
            exit(1);
        }

        while(fgets(recordBuffer,1024,filePointer))
        {
            TransactionRecord* transactionRecord;
            transactionRecord=(TransactionRecord*)malloc(sizeof(TransactionRecord));
            
            if(transactionRecord==NULL)
            {
                fprintf(stderr,"Dynamic memory allocation through malloc failed.\n");
                exit(1);
            }
            // printf("%lu",strlen(recordBuffer));
            // printf("%c",recordBuffer[strlen(recordBuffer)-1]);
            // printf("\n");
            // printf("%s",recordBuffer);

            // if(recordBuffer[strlen(recordBuffer)-1]!='\n')
            // {
            //     printf("Record longer than 1024 characters.");
            //     return(-1);
            // }

            field=strtok(recordBuffer,delim);
            if(strlen(field)!=1)
            {
                fprintf(stderr,"Transaction type is not a single character.\n");
                exit(1);
            }

            char transactionType=*field;

            field=strtok(NULL,delim);
            if(strlen(field)>=11)
            {
                fprintf(stderr,"Transaction time is more than or equal to 11 digits.\n");
                exit(1);
            }
            if(field[0]=='0')
            {
                fprintf(stderr,"Transaction time as zero as first digit.\n");
                exit(1);
            }
            if(field[0]=='-')
            {
                fprintf(stderr,"Transaction time is negative.\n");
                exit(1);
            }
            time_t currentTimeSeconds; 
      
            currentTimeSeconds = time(NULL); 
            time_t transactionTimeSeconds=atoi(field);
            if(transactionTimeSeconds>=currentTimeSeconds)
            {
                fprintf(stderr,"Transaction time is greater than or equal to current time.\n");
                exit(1);
            }

            time_t transactionTime=transactionTimeSeconds;

            field=strtok(NULL,delim);

            if(field[0]=='-')
            {
                fprintf(stderr,"Transaction amount is negative.\n");
                exit(1);
            }

            int i=0;
            while(i<7)
            {
                if(field[i]=='.')
                {
                    break;
                }
                i=i+1;
            }

            if(i==7)
            {
                fprintf(stderr,"Transaction amount number to the left of decimel point is more than 7 digits.\n");
                exit(1);
            }

            double transactionAmount=atof(field);
            if((int)transactionAmount!=0 && field[0]=='0')
            {
                fprintf(stderr,"Transaction amount before decimal point is not zero but its first digit is zero.\n");
                exit(1);
            }

            field=strtok(NULL,delim);

            if(strlen(field)==0)
            {
                fprintf(stderr,"Transaction description is empty.\n");
                exit(1);
            }
            
            int j=0;
            while(field[j]==' ')
            {
                j++;
            }

            int k=0;
            if(j!=0)
            {
                while(field[k+j]!='\0')
                {
                    field[k]=field[k+j];
                    k++;
                }
                field[k]='\0';
            }

            if(strlen(field)==0)
            {
                fprintf(stderr,"Transaction description is empty.\n");
                exit(1);
            }
            // printf("%s",field);

            char transactionDescription[1024];
            memset(transactionDescription, '\0', strlen(field));
            strncpy(transactionDescription,field,strlen(field));

            transactionRecord->transactionType=transactionType;
            transactionRecord->transactionTime=transactionTime;
            transactionRecord->transactionAmount=transactionAmount;
            strncpy(transactionRecord->transactionDescription,transactionDescription,strlen(field));

            (void)My402ListAppend(&transactionList, transactionRecord);

            // printf("\n");
            // displayTransaction(transactionRecord);

            // My402ListElem* e=My402ListLast(&transactionList);
            // TransactionRecord* o=e->obj;
            // printf("%s",o->transactionDescription);
            // printf("\n");
        }
        fclose(filePointer);
    }
    // printf("\n");

    // printf("%d",transactionList.num_members);
    // PrintTestList(&transactionList,transactionList.num_members);

    BubbleSortForwardList(&transactionList,transactionList.num_members);

    // printf("After SORT");
    // PrintTestList(&transactionList,transactionList.num_members);


    printf("+-----------------+--------------------------+----------------+----------------+\n");
    printf("|       Date      | Description              |         Amount |        Balance |\n");
    printf("+-----------------+--------------------------+----------------+----------------+\n");

    My402ListElem *printElem=NULL;

    // if (My402ListLength(pList) != num_items) {
    //     fprintf(stderr, "List length is not %1d in PrintTestList().\n", num_items);
    //     exit(1);
    // }

    double currentBalance=0;
    for (printElem=My402ListFirst(&transactionList); printElem != NULL; printElem=My402ListNext(&transactionList, printElem)) {
        TransactionRecord* ival=printElem->obj;
        // printf("%s",ival->transactionDescription);
        time_t ival_date=ival->transactionTime;
        char date[16];
        char buf[26];
        strncpy(buf, ctime(&ival_date), sizeof(buf)); 
        date[0] = buf[0];
        date[1] = buf[1];
        date[2] = buf[2];
        date[3] = buf[3];
        date[4] = buf[4];
        date[5] = buf[5];
        date[6] = buf[6];
        date[7] = buf[7];
        date[8] = buf[8];
        date[9] = buf[9];
        date[10] = buf[19];
        date[11] = buf[20];
        date[12] = buf[21];
        date[13] = buf[22];
        date[14] = buf[23];
        date[15] = '\0';

        printf("| %s |",date);

        char description[25];
        int descriptionDisplayLength=0;
        int j;
        for(j=0;j<24;j++)
        {
            if(ival->transactionDescription[j]=='\n' || ival->transactionDescription[j]=='\0')
            {
                break;
            }
            description[j]=ival->transactionDescription[j];
            descriptionDisplayLength=descriptionDisplayLength+1;
        }
        description[j]='\0';
        printf(" %s",description);

        for(int k=1;k<=24-descriptionDisplayLength+1;k++)
        {
            printf(" ");
        }
        printf("| ");

        char amount[15];
        amount[14]='\0';

        double amt=ival->transactionAmount;

        if(ival->transactionType=='-')
        {
            amount[0]='(';
            amount[13]=')';
            currentBalance=currentBalance-amt;
        }
        else
        {
            amount[0]=' ';
            amount[13]=' ';
            currentBalance=currentBalance+amt;
        }

        if(amt>=10000000 || amt<=-10000000)
        {
            amount[1]='?';
            amount[2]=',';
            amount[3]='?';
            amount[4]='?';
            amount[5]='?';
            amount[6]=',';
            amount[7]='?';
            amount[8]='?';
            amount[9]='?';
            amount[10]='.';
            amount[11]='?';
            amount[12]='?';
        }

        else
        {
            int amtMul100=round(amt*100);

            int b1=amtMul100%10;
            amtMul100=amtMul100/10;

            amount[12]='0'+b1;

            int b2=amtMul100%10;
            amtMul100=amtMul100/10; 
            amount[11]='0'+b2; 

            amount[10]='.';

            int x=9;

            int tripleCount=0;
            if(amtMul100%10==0)
            {
                amount[x]='0';
                x=x-1;
                tripleCount=tripleCount+1;
                amtMul100=amtMul100/10;
            }

            while(amtMul100!=0)
            {
                if(tripleCount==3)
                {
                    tripleCount=0;
                    amount[x]=',';
                    x=x-1;
                }
                int curDig=amtMul100%10;
                amount[x]='0'+curDig;

                amtMul100=amtMul100/10;
                x=x-1;
                tripleCount=tripleCount+1;
            } 

            while(x!=0)
            {
                amount[x]=' ';
                x=x-1;
            }
        }

        printf("%s | ",amount);

        char balance[15];

        moneyFormat(balance, currentBalance);
        
        printf("%s |",balance);
        printf("\n");

        // displayTransaction(ival);
    }

    printf("+-----------------+--------------------------+----------------+----------------+\n");
    return(0);
}