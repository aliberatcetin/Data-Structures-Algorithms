/*
Dependency : dataset folder on same directory with categories and txt files in it.

*/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <math.h>

struct pathNode{
    char *term1;
    char *term2;
    struct pathNode *next;
};

struct innerNode{
    char *category;
    char *document;
    int frequency;
    struct innerNode *next;
};

struct masterNode{
    char *term;
    struct masterNode *next;
    struct innerNode *head;
};

struct wordNode{
    char *term;
    struct wordNode *next;
};

struct documentNode{
    char *textid;
    struct wordNode *head;
    struct documentNode *next;
};

struct tempNode{
    char *term;
    char *category;
    char *document;
    struct tempNode *next;
};

struct frequencyNode{
    char *term;
    char *category;
    int freq;
    struct frequencyNode *next;
};

struct singleDocNode{
    char *term;
    char *category;
    char *document;
    int freq;
    struct singleDocNode *next;
};

struct categoryNode{
    char *name;
    int documentCount;
    struct categoryNode *next;
};

typedef struct categoryNode cn;
typedef struct frequencyNode fn;
typedef struct tempNode tn;
typedef struct singleDocNode sdn;
typedef struct wordNode wn;
typedef struct documentNode dn;
typedef struct pathNode pn;


fn *fHead=NULL;
tn *tHead=NULL;
sdn *sdHead=NULL;
dn *dHead=NULL;
cn *cHeadGlobal;

pn *firstOrderHead=NULL;
pn *secondOrderHead=NULL;
pn *thirdOrderHead=NULL;

pn *newPathNode(){
    pn *temp ; // declare a node
    temp = (pn*)malloc(sizeof(pn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

wn* newWordNode(){
    wn *temp ; // declare a node
    temp = (wn*)malloc(sizeof(wn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

dn* newDocumentNode(){
    dn *temp ; // declare a node
    temp = (dn*)malloc(sizeof(dn)); // allocate memory using malloc()
    temp->head=NULL;
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

tn* newtempNode(){
    tn *temp ; // declare a node
    temp = (tn*)malloc(sizeof(tn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

fn* newfrequencyNode(){
    fn *temp;
    temp = (fn*)malloc(sizeof(fn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

sdn* newSingleDocNode(){
    sdn *temp;
    temp = (sdn*)malloc(sizeof(sdn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

cn* newCategoryNode(){
    cn *temp;
    temp = (cn*)malloc(sizeof(cn)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}


void insertToCategorylist(char *name,cn **head,int numberOfDocuments){
    cn *temp,*iterator;
    temp=newCategoryNode();
    temp->name=strdup(name);
    temp->documentCount=numberOfDocuments;
    if(*head==NULL){
        *head=temp;
        return;
    }
    iterator=*head;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

void insertToDocumentlist(char *name){
    dn *temp,*iterator;
    temp=newDocumentNode();
    temp->textid=strdup(name);
    if(dHead==NULL){
        dHead=temp;
        return;
    }
    iterator=dHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

void insertToWordlist(char *term,wn **whead){
    wn *temp=newWordNode();
    temp->term=strdup(term);
    if(*whead==NULL){
        *whead=temp;
        return;
    }

    wn *iterator=*whead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

int isInDocumentList(char *textid){
    dn *temp=dHead;
    if(dHead==NULL){
        return 0;
    }
    while(temp!=NULL){
        if(!strcmp(textid,temp->textid)){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void insertToTemplist(char *term,char *category,char *document){
    tn *temp,*iterator;
    temp=newtempNode();
    temp->term=strdup(term);
    temp->category=strdup(category);
    temp->document=strdup(document);
    if(tHead==NULL){
        tHead=temp;
        return;
    }
    if(strcmp(term,tHead->term)>0){
        iterator=tHead;
        while(iterator->next!=NULL){
            if(strcmp(term,iterator->next->term)>0){
                iterator=iterator->next;
            }else{
                break;
            }
        }
        if(iterator->next==NULL){
            iterator->next=temp;
            temp->next=NULL;
        }else{
            temp->next=iterator->next;
            iterator->next=temp;
        }
    }else{
        temp->next=tHead;
        tHead=temp;
    }
}

void printTemplist(){
    tn *iterator = tHead;
    while(iterator!=NULL){
        printf("%s - %s - %s\n",iterator->term,iterator->category,iterator->document);
        iterator=iterator->next;
    }
}

int isInTemplist(char *term){
    tn *iterator = tHead;
    while(iterator!=NULL){
        if(!strcmp(term,iterator->term)){
            return 1;
        }
        iterator=iterator->next;
    }
    return 0;
}

void readText(char *p,char *category,char *document){//reading the text comes from traverse directory
    FILE *fp;
    char str[50];
    fp = fopen(p, "r");
    if (fp == NULL){
        return;
    }
    while ( !feof(fp)){
        fscanf(fp,"%s",str);
        insertToTemplist(str,category,document);
    }
    fclose(fp);
}

int readDataSet(char folder[],cn **cHead){
    int numOfDocuments;
    struct dirent *de,*in;
    char datasetDir[10],categoryDir[30],textDir[40];
    strcpy(datasetDir,folder);
    DIR *datasetFile,* categoryFile= opendir(datasetDir);
    datasetFile = opendir(datasetDir);

    if(datasetFile==NULL){
        printf("directory can not be read");
        exit(0);
    }
    readdir(datasetFile);
    readdir(datasetFile);
    while ((de = readdir(datasetFile)) != NULL){
        strcpy(categoryDir,folder);
        strcat(categoryDir,"/");
        strcat(categoryDir,de->d_name);

        categoryFile = opendir(categoryDir);
        numOfDocuments = 0;
        while((in=readdir(categoryFile))!=NULL){
            if(in->d_namlen>2){
                strcpy(textDir,categoryDir);
                strcat(textDir,"/");
                strcat(textDir,in->d_name);
                numOfDocuments++;
                readText(textDir,de->d_name,in->d_name);
            }
        }
        insertToCategorylist(de->d_name,cHead,numOfDocuments);
    }
    return numOfDocuments;
}

void swap(fn *node1,fn *node2){
    int tempFreq;
    char tempCategory[30],tempTerm[30];
    tempFreq=node1->freq;
    strcpy(tempCategory,node1->category);
    strcpy(tempTerm,node1->term);

    strcpy(node1->term,node2->term);
    strcpy(node1->category,node2->category);
    node1->freq=node2->freq;

    strcpy(node2->term,tempTerm);
    strcpy(node2->category,tempCategory);
    node2->freq=tempFreq;
}

void frequencySort(){
    int swapped, i;
    fn *ptr1;
    fn *lptr = NULL;

    /* Checking for empty list */
    if (fHead == NULL)
        return;
    do
    {
        swapped = 0;
        ptr1 = fHead;

        while (ptr1->next != lptr)
        {
            if (ptr1->freq < ptr1->next->freq)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

int findInFrequencylistThenIncrement(char *term,char *category){
    if(fHead==NULL){
        return 0;
    }
    fn *iterator = fHead;
    while(iterator!=NULL){
        if(!strcmp(iterator->category,category) && !strcmp(iterator->term,term)){
            iterator->freq=iterator->freq+1;
            return 1;
        }
        iterator=iterator->next;
    }
    return 0;
}

void insertToFrequencylist(char *term,char *category){
    fn *temp,*iterator;
    temp=newfrequencyNode();
    temp->term=strdup(term);
    temp->category=strdup(category);
    temp->freq=1;
    if(fHead==NULL){
        fHead=temp;
        return;
    }
    iterator=fHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

void frequencyListFromTemp(){
    tn *temp=tHead;
    while(temp!=NULL){
        if(!findInFrequencylistThenIncrement(temp->term,temp->category)){
            insertToFrequencylist(temp->term,temp->category);
        }
        temp=temp->next;
    }
    frequencySort();
}

void printFrequencylist(){
    fn *iterator = fHead;
    while(iterator!=NULL){
        printf("%s - %s - %d\n",iterator->term,iterator->category,iterator->freq);
        iterator=iterator->next;
    }
}

void addToSingleDoclist(char *term,char *category,char *document){
    sdn *temp,*iterator;
    temp=newSingleDocNode();
    temp->term=strdup(term);
    temp->category=strdup(category);
    temp->document=strdup(document);
    temp->freq=1;
    if(sdHead==NULL){
        sdHead=temp;
        return;
    }
    iterator=sdHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

int isInSinglelist(char *term,char *category,char *document){
    if(sdHead==NULL){
        return 0;
    }
    sdn *temp = sdHead;
    while(temp!=NULL){
        if(!strcmp(temp->document,document) && !strcmp(temp->term,term) && !strcmp(temp->category,category)){
             temp->freq=temp->freq+1;
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void singleDoclistFromTemp(){
    tn *iterator=tHead;
    while(iterator!=NULL){
        if(!isInSinglelist(iterator->term,iterator->category,iterator->document)){
            addToSingleDoclist(iterator->term,iterator->category,iterator->document);
        }
        iterator=iterator->next;
    }
}

void printSingleDoclist(){
    sdn *iterator=sdHead;
    while(iterator!=NULL){
        printf("%s %s %s %d \n",iterator->term,iterator->category,iterator->document,iterator->freq);
        iterator=iterator->next;
    }
}

int isInCategorlist(char *name,cn *head){
    if(head==NULL){
        return 0;
    }
    while(head!=NULL){
        if(!strcmp(head->name,name)){
            return 1;
        }
        head=head->next;
    }
    return 0;
}

void printCategorylist(cn *head){
    while(head!=NULL){
        printf("%s %d",head->name,head->documentCount);
        head=head->next;
    }
}

int getCategoryLength(cn *head){
    int counter=0;
    while(head!=NULL){
        counter++;
        head=head->next;
    }
    return counter;
}

cn getCategoryNodeWithIndex(int index,cn *head){
    int counter=0;
    while(head!=NULL){
        if(index==counter){
            return *head;
        }
        counter++;
        head=head->next;
    }
}

void printMostFrequentWithCategory(char *category,int count){
    int counter=0;
    fn *temp=fHead;
    while(temp!=NULL){
        if(counter==count){
            return;
        }
        if(!strcmp(temp->category,category)){
            printf("%s,%d\n",temp->term,temp->freq);
            counter++;
        }
        temp=temp->next;
    }
}

int getDocumentCountOfATermInACategory(char *term,char *category){
    int counter=0;
    sdn *temp=sdHead;
    while(temp!=NULL){
        if(!strcmp(temp->category,category) && !strcmp(temp->term,term)){
            counter++;
        }
        temp=temp->next;
    }
    return counter;
}

int getDocumentCountByCategory(char *category){
    cn *temp=cHeadGlobal;
    while(temp!=NULL){
        if(!strcmp(category,temp->name)){
            return temp->documentCount;
        }
        temp=temp->next;
    }
}

void printMostFrequentWithCategoryIdf(char *category,int count){
    int counter=0;
    fn *temp=fHead;
    while(temp!=NULL){
        if(counter==count){
            return;
        }
        if(!strcmp(temp->category,category)){
            printf("%s,%lf \n",temp->term,log((double)getDocumentCountByCategory(category)/getDocumentCountOfATermInACategory(temp->term,category))*temp->freq);
            counter++;
        }
        temp=temp->next;
    }
}

void getDocumentCountInACategory(char *category){

}

void printBC(){
    cn *cHead=NULL;
    fn *temp = fHead;
    printf("\n");
    while(temp!=NULL){
        if(!isInCategorlist(temp->category,cHead)){
            insertToCategorylist(temp->category,&cHead,0);
        }
        temp=temp->next;
    }
    int categoryLength = getCategoryLength(cHead);

    int printedWords=0;
    cn tempCategory;
    for(int i=0;i<categoryLength;i++){
        tempCategory=getCategoryNodeWithIndex(i,cHead);
        printf("%s\n",tempCategory.name);
        printMostFrequentWithCategory(tempCategory.name,5);
        printf("\n\n");
    }

    for(int i=0;i<categoryLength;i++){
        tempCategory=getCategoryNodeWithIndex(i,cHead);
        printf("%s\n",tempCategory.name);
        printMostFrequentWithCategoryIdf(tempCategory.name,5);
        printf("\n\n");
    }

}

void documentListFromSingleDocList(){
    sdn *temp=sdHead;
    char currentDocumentId[30];
    while(temp!=NULL){
        strcpy(currentDocumentId,temp->category);
        strcat(currentDocumentId,temp->document);
        if(!isInDocumentList(currentDocumentId)){
            insertToDocumentlist(currentDocumentId);
        }
        temp=temp->next;
    }
}

void printDocumentlist(){
    dn *temp=dHead;
    wn *temp2;
    while(temp!=NULL){
        printf("%s ",temp->textid);
        temp2=temp->head;
        while(temp2!=NULL){
            printf("%s ",temp2->term);
            temp2=temp2->next;
        }
        printf("\n");
        temp=temp->next;
    }
}

void insertWordToADocument(char *term,char *textid){
    if(dHead==NULL){
        return;
    }
    dn *temp=dHead;
    while(temp!=NULL){
        if(!strcmp(temp->textid,textid)){
            break;
        }
        temp=temp->next;
    }

    wn *temp2,*iterator;
    temp2=newWordNode();
    temp2->term=strdup(term);
    if(temp->head==NULL){
        temp->head=temp2;
        return;
    }
    iterator = temp->head;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp2;
}

void addWordsToInnerDocNodes(){
    dn *temp=dHead;
    sdn *temp2;
    char currentText[30];
    while(temp!=NULL){
        temp2=sdHead;
        while(temp2!=NULL){
            strcpy(currentText,temp2->category);
            strcat(currentText,temp2->document);
            if(!strcmp(currentText,temp->textid)){
                insertWordToADocument(temp2->term,currentText);
            }
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

int isInFirstOrderPaths(char *term1,char *term2){
    pn *temp=firstOrderHead;
    if(temp==NULL){
        return 0;
    }
    while(temp!=NULL){
        if(!strcmp(temp->term1,term1) && !strcmp(temp->term2,term2) || !strcmp(temp->term2,term1) && !strcmp(temp->term1,term2)){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void insertToSecondOrderPath(char *term1,char *term2){
    pn *temp,*iterator;
    temp=newPathNode();
    temp->term1=strdup(term1);
    temp->term2=strdup(term2);
    if(secondOrderHead==NULL){
        secondOrderHead=temp;
        return;
    }

    iterator=secondOrderHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;

}



void insertToThirdOrderPath(char *term1,char *term2){
    pn *temp,*iterator;
    temp=newPathNode();
    temp->term1=strdup(term1);
    temp->term2=strdup(term2);
    if(thirdOrderHead==NULL){
        thirdOrderHead=temp;
        return;
    }

    iterator=thirdOrderHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;

}

void insertToFirstOrderPath(char *term1,char *term2){
    pn *temp,*iterator;
    temp=newPathNode();
    temp->term1=strdup(term1);
    temp->term2=strdup(term2);
    if(firstOrderHead==NULL){
        firstOrderHead=temp;
        return;
    }

    iterator=firstOrderHead;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;

}

void findFirstOrderPaths(){
    dn *temp=dHead;
    wn *temp2;
    wn *temp3;
    while(temp!=NULL){
        temp2=temp->head;
        while(temp2!=NULL){
            temp3=temp2->next;
            while(temp3!=NULL){
                if(!isInFirstOrderPaths(temp2->term,temp3->term) && strcmp(temp2->term,temp3->term)){
                    insertToFirstOrderPath(temp2->term,temp3->term);
                }
                temp3=temp3->next;
            }
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

void printThirdOrderPaths(){
    pn *temp=thirdOrderHead;
    int length=0;
    int counter=0;
    printf("\nThird Order Paths\n");
    while(temp!=NULL){
        length++;
        printf("{ %s,%s } ",temp->term1,temp->term2);
        temp=temp->next;
        counter++;
        if(counter%5==0){
            printf("\n");
        }
    }
    printf("\n\n--%d--",counter);

}

void printFirstOrderPaths(){
    pn *temp=firstOrderHead;
    int length=0;
    int counter=0;
    printf("\nFirst Order Paths\n");
    while(temp!=NULL){
        length++;
        printf("{ %s,%s } ",temp->term1,temp->term2);
        temp=temp->next;
        counter++;
        if(counter%7==0){
            printf("\n");
        }
    }
    printf("\n\n--%d--",counter);
}

int isInSecondOrderPath(char *term1,char *term2){
    pn *temp=secondOrderHead;
    if(temp==NULL){
        return 0;
    }
    while(temp!=NULL){
        if(!strcmp(temp->term1,term1) && !strcmp(temp->term2,term2) || !strcmp(temp->term2,term1) && !strcmp(temp->term1,term2)){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

int isInThirdOrderPath(char *term1,char *term2){
    pn *temp=thirdOrderHead;
    if(temp==NULL){
        return 0;
    }
    while(temp!=NULL){
        if(!strcmp(temp->term1,term1) && !strcmp(temp->term2,term2) || !strcmp(temp->term2,term1) && !strcmp(temp->term1,term2)){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

int controlCommonWord(wn *fh,wn *sh){
    wn *actualHead = sh;
    while(fh!=NULL){
        sh=actualHead;
        while(sh!=NULL){
            if(!strcmp(fh->term,sh->term)){
                return 1;
            }
            sh=sh->next;
        }
        fh=fh->next;
    }
    return 0;
}

int getCommonWords(wn *fh,wn *sh,wn **head){
    wn *actualHead = sh;
    int counter=0;
    while(fh!=NULL){
        sh=actualHead;
        while(sh!=NULL){
            if(!strcmp(fh->term,sh->term)){
                insertToWordlist(fh->term,head);
                counter ++;
            }
            sh=sh->next;
        }
        fh=fh->next;
    }
    return counter;
}

void printCommonWords(wn *head){
    while(head!=NULL){
        printf("%s ",head->term);
        head=head->next;
    }
}

void combineDocuments(wn *firstDocHead,wn*secondDocHead){
    wn *actualHead=secondDocHead;
    wn *actualFirst=firstDocHead;
    wn *wHead=NULL;
    if(getCommonWords(firstDocHead,secondDocHead,&wHead)==0){
        return;
    }

    wn *tempwnhead=wHead;
    while(tempwnhead!=NULL){
        if(!strcmp(tempwnhead->term,"ssk")){
            printf("ssk");
        }
        firstDocHead=actualFirst;
        while(firstDocHead!=NULL ){
            secondDocHead=actualHead;
            while(secondDocHead!=NULL){
                if(!isInSecondOrderPath(firstDocHead->term,secondDocHead->term) && strcmp(firstDocHead->term,secondDocHead->term )){
                    if(strcmp(tempwnhead->term,firstDocHead->term) && strcmp(tempwnhead->term,secondDocHead->term)){
                        insertToSecondOrderPath(firstDocHead->term,secondDocHead->term);
                    }
                }
                secondDocHead=secondDocHead->next;
            }
            firstDocHead=firstDocHead->next;
        }
        tempwnhead=tempwnhead->next;
    }

}

int searchTwoTermsInDocs(char *term1, char *term2,dn *doc1,dn *doc2){
    dn *temp=dHead;
    int term1Flag=0,term2Flag=0;
    wn *tempDocHead;
    while(temp!=NULL){

        if(temp!=doc1 && temp!=doc2){
            tempDocHead=temp->head;
            term1Flag=0;
            term2Flag=0;
            while(tempDocHead!=NULL){
                if(!strcmp(tempDocHead->term,term1)){
                        term1Flag=1;
                }else if(!strcmp(tempDocHead->term,term2)){
                        term2Flag=1;
                }
                tempDocHead=tempDocHead->next;
                if(term1Flag && term2Flag){
                    //printf("Found doc : %s , terms : %s %s\n",temp->textid,term1,term2);
                    break;
                }
            }
            if(term1Flag && term2Flag){
                return 1;
            }
        }
        temp=temp->next;
    }
    return 0;
}

void searchAndFindMatchesForThirdOrder(char *term1, char *term2,dn *doc1,dn *doc2){

    wn *sdh;
    wn *fdh=doc1->head;
    while(fdh!=NULL){
        sdh=doc2->head;
        while(sdh!=NULL){
            if(strcmp(sdh->term,term1) && strcmp(sdh->term,term2) && strcmp(fdh->term,term1) && strcmp(fdh->term,term2)){
                if(searchTwoTermsInDocs(fdh->term,sdh->term,doc1,doc2)){
                    if(!isInThirdOrderPath(term1,term2)){
                        //printf("inserted terms : %s %s without docs %s %s\n",term1,term2,doc1->textid,doc2->textid);
                        insertToThirdOrderPath(term1,term2);
                    }

                }
            }
            sdh=sdh->next;
        }
        fdh=fdh->next;
    }
}

void combineTwoDocumentForThirdOrder(dn *temp1,dn *temp2){
    wn *secondDocHead=temp2->head;
    wn *firstDocHead=temp1->head;

    /*if(controlCommonWord(firstDocHead,secondDocHead)){
        return;
    }*/

    while(firstDocHead!=NULL){
        secondDocHead=temp2->head;
        while(secondDocHead!=NULL){
            if(strcmp(firstDocHead->term,secondDocHead->term)){
                searchAndFindMatchesForThirdOrder(firstDocHead->term,secondDocHead->term,temp1,temp2);
            }
            secondDocHead=secondDocHead->next;
        }
        firstDocHead=firstDocHead->next;
    }
}

void findThirdOrderPaths(){
    dn *temp=dHead;
    dn *temp2;
    while(temp!=NULL){
        temp2=temp->next;
        while(temp2!=NULL){
            combineTwoDocumentForThirdOrder(temp,temp2);
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

void findSecondOrderPaths(){
    dn *temp=dHead;
    dn *temp2;
    while(temp!=NULL){
        temp2=temp->next;
        while(temp2!=NULL){
            combineDocuments(temp->head,temp2->head);
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

void printSecondOrderPaths(){
    pn *temp=secondOrderHead;
    int counter=0;
    printf("\nSecond Order Paths\n");
    while(temp!=NULL){
        printf("{ %s,%s } ",temp->term1,temp->term2);
        temp=temp->next;
        counter++;
        if(counter%5==0){
            printf("\n");
        }
    }
    printf("\n\n--%d--",counter);
}

int main(){
    cn *cHead=NULL;
    int numOfDocuments = readDataSet("./dataset",&cHead);
    //printCategorylist(cHead);
    cHeadGlobal=cHead;
    frequencyListFromTemp(); // n^2
    //printFrequencylist();
    printf("\n\n");
    singleDoclistFromTemp(); // n^2
    //printSingleDoclist();
    documentListFromSingleDocList(); // n^2
    addWordsToInnerDocNodes(); // n^3
    //printDocumentlist();

    //printf("\n%d",numOfDocuments);

    findFirstOrderPaths();//n^2
    printFirstOrderPaths(); //n
    printf("\n");

    findSecondOrderPaths();//n^2

    printf("\n");

    printSecondOrderPaths();//n

    findThirdOrderPaths();//n^4
    printThirdOrderPaths();// n

    //printf("\n");
    printBC();

}
