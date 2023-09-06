/*list.h*/
// Nicolas Aßmann
// s85475
// 22/041/61


struct tlst;
typedef struct tlst tList;

tList* CreateList();

int insertHead(tList* pList, void *pDataIns);
int insertTail(tList* pList, void *pDataIns);
int insertBehind(tList *pList, void *pDataIns);

void* getFirst(tList *pList);
void* getLast(tList *pList);
void* getNext(tList *pList);
void* getPrevious(tList *pList);

void removeNext(tList *pList);
void removeCurrent(tList *pList);
void removeFirst(tList *pList);

void quicksort(tList * pList, int (*compar)(const void *, const void *));
void mergesort(tList * pList, int (*compar)(const void *, const void *));

void writeDat(tList * pList);
void readDat(tList * pList);


