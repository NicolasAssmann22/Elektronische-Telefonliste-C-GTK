#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"beleg.h"

void writeDat(tList * pList)
{
        FILE* file = fopen("phonedirectory.txt","w");
        if(file == NULL)
        {
                printf("could not open file\n");
                exit(1);
        }
        for(tDat *pDat=getFirst(pList);pDat;pDat=getNext(pList))
    {
        fprintf(file, "%s\n%s\n%s\n",pDat->lname, pDat->fname, pDat->tel);
    }
        fclose(file);
}

void readDat(tList * pList)
{
        FILE* file = fopen("phonedirectory.txt","a+");
        if(file == NULL)
        {
                printf("could not open file\n");
                exit(2);

        }
        char templname[128]={};
        char tempfname[128]={};
        char temptel[128]={};

        while(fscanf(file, "%[^\n] %[^\n] %[^\n] ", templname,tempfname,temptel)>0)
        {
                tDat * pDat=malloc(sizeof (tDat));
                pDat->lname=malloc(sizeof(char)* strlen(templname));
                pDat->fname=malloc(sizeof(char)* strlen(tempfname));
                pDat->tel=malloc(sizeof(char)* strlen(temptel));

                strcpy(pDat->lname,templname);
                strcpy(pDat->fname,tempfname);
                strcpy(pDat->tel,temptel);

                int ret=insertTail(pList,pDat);
                if(ret==0) printf("error, could not insert in list!\n");
        }
}


