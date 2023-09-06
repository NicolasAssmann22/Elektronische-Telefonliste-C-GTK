// Nicolas Aßmann
// s85475
// 22/041/61


#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include"list.h"
#include"beleg.h"


typedef struct Cnctr
{
	struct Cnctr *pNxt;
	struct Cnctr *pPrvs;
	void	     *pItem;

}tCnct;

typedef struct tlst
{
	tCnct head;
	tCnct*pCurr;

}tList;

int InsCnctBehCnct(tCnct*pBef, void*pI)
{
	tCnct* pCnct=malloc(sizeof(tCnct));
	if(pCnct)
	{
		pCnct->pItem		=pI;
		pCnct->pNxt		=pBef->pNxt;
		pCnct->pPrvs		=pBef;
		pCnct->pNxt->pPrvs	=pCnct;
		pBef->pNxt		=pCnct;
		return 1;
	}
	else printf("could not allocate memory for connector!\n");
	return 0;

}

tList* CreateList()
{
	tList* pList;
	pList= (tList*)malloc(sizeof(tList));


	if(pList)
	{
		pList->pCurr=NULL;	
		pList->head.pNxt = (tCnct*)pList;
		pList->head.pPrvs = (tCnct*)pList;
		pList->head.pItem=NULL;
	}
	else printf("could not allocate memory for list!\n");
	return pList;
}

int insertHead(tList* pList, void *pDataIns)
{
	return InsCnctBehCnct(&pList->head, pDataIns);
}

int insertTail(tList* pList, void *pDataIns)
{
	return InsCnctBehCnct(pList->head.pPrvs, pDataIns);
}

int insertBehind(tList *pList, void *pDataIns)
{
	return InsCnctBehCnct(pList->pCurr, pDataIns);
}

void removeCnct(tCnct * pCnct)
{
	
	pCnct->pNxt->pPrvs	=pCnct->pPrvs;
	pCnct->pPrvs->pNxt	=pCnct->pNxt;
	free (pCnct);
}



void* getCurrent(tList * pList)
{
	void* ptmp=NULL;
	ptmp=pList->pCurr;
	return ptmp;
}

void removeNext(tList *pList)
{
	removeCnct(pList->pCurr->pNxt);
}

void removeCurrent(tList *pList)
{
	removeCnct(pList->pCurr);
	pList->pCurr=NULL;
}

void removeFirst(tList *pList)
{	
	removeCnct(pList->head.pNxt);
}

void* getFirst(tList * pList)
{
	void* ptmp=NULL;
	ptmp=pList->head.pNxt->pItem;
	pList->pCurr=pList->head.pNxt;
	return ptmp;

}

void* getNext(tList* pList)
{
	void* ptmp=NULL;
	ptmp=pList->pCurr->pNxt->pItem;
	pList->pCurr=pList->pCurr->pNxt;
	return ptmp;
}
void* getPrevious(tList* pList)
{
	void* ptmp=NULL;
	ptmp=pList->pCurr->pPrvs->pItem;
	pList->pCurr=pList->pCurr->pPrvs;
	return ptmp;
}

void* getLast(tList* pList)
{
	void* ptmp=NULL;
	ptmp=pList->head.pPrvs->pItem;
	pList->pCurr=pList->head.pPrvs;
	return ptmp;
}

void swapItem(tCnct * c1, tCnct * c2)
{
	tCnct * temp=malloc(sizeof(tCnct));
	temp->pItem=c2->pItem;
	c2->pItem=c1->pItem;
	c1->pItem=temp->pItem;
	free (temp);
}

tCnct * part(tList * pList, tCnct * low, tCnct * high, int (*compar)(const void *, const void *) )
{ 
	tCnct * piv=low;
	void * pive=piv->pItem;
	tCnct * j=low->pNxt;
	
	for(tCnct * i=low->pNxt; i->pPrvs !=high;i=i->pNxt)
	{
		void * tempi=i->pItem;
		if(compar(tempi,pive)<=0)
		{
			swapItem(i,j);
			j=j->pNxt;	
		}

	}
	swapItem(low,j->pPrvs);
	return j->pPrvs;
}


void  quicksortRec(tList * pList, tCnct * low, tCnct * high, int (*compar)(const void *, const void *))
{
	
	if(high && low && low != high )
	{
		tCnct * pivi=part(pList, low, high, compar);
		if(low != pivi)	quicksortRec(pList, low, pivi->pPrvs, compar);
		if(high != pivi) quicksortRec(pList, pivi->pNxt, high, compar);
	}
}


void quicksort(tList * pList, int (*compar)(const void *, const void *))
{
	quicksortRec(pList,pList->head.pNxt, pList->head.pPrvs, compar);


}

void mergesort(tList *pList, int (*compar)(const void *, const void *))
{
	tList * lList=CreateList();
	tList * rList=CreateList();

	while(getFirst(pList))
	{
		insertHead(lList, getFirst(pList));
		removeCnct(pList->pCurr);
		if(getFirst(pList)) 
		{
			insertHead(rList, getFirst(pList));
			removeCnct(pList->pCurr);
		}

		else break;
	}
	if(lList->head.pNxt->pNxt->pItem) mergesort(lList, compar);
	if(rList->head.pNxt->pNxt->pItem) mergesort(rList, compar);

	while(getFirst(pList)) removeCnct(pList->head.pNxt);

	while(getFirst(lList) || getFirst(rList))
	{
		if(getFirst(lList) && (getFirst(rList)==NULL))
		{
			insertTail(pList, getFirst(lList));
			removeCnct(lList->head.pNxt);
		}
		
		else if(getFirst(rList) && (getFirst(lList)==NULL))
		{
			insertTail(pList, getFirst(rList));
			removeCnct(rList->head.pNxt);
		}
		else if(compar(getFirst(lList), getFirst(rList))<0) 
		{
			insertTail(pList, getFirst(lList));
			removeCnct(lList->head.pNxt);
		}
		else
		{
			insertTail(pList, getFirst(rList));
			removeCnct(rList->head.pNxt);
		}

	}
	

}



































































