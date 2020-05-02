/*
Dependency : dataset folder on same directory with categories and txt files in it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <locale.h>

typedef struct occurenceAndIndexes oa;
typedef struct textLengths tl;
typedef struct representiveNode repNode;
typedef struct masterNode exNode;
typedef struct innerNode inNode;
typedef struct stopWords sw;

struct innerNode{// inner list of master
	char docID[20];
	int occurrence;
	int length;
	char category[20];
	struct innerNode *next;
};

struct masterNode{//master nodes
	char *term;
	int numberOfDocs;
	inNode *head;
	struct masterNode *next;
};

struct representiveNode{//my temp nodes
	char category[15];
	char id[15];
	char *term;
	struct representiveNode *next;
};

struct discriminates{//representing discriminating words
	char magazine[5][20];
	char health[5][20];
	char econ[5][20];
};

struct textLengths{//to keep text lengths 
	char *textName;
	char *category;
	int length;
	struct textLengths *next;
};

struct occurenceAndIndexes{//to find stop words
	int index;
	int occurence;
	struct occurenceAndIndexes *next;
};

struct stopWords{
	char *term;
	struct stopWords *next;
};

exNode *mhead;
repNode *fhead;
tl *thead;
oa *ohead;
sw *shead;

sw* insertStopWords(char *p){//insert the stopwords comes from stopWords() function by ascending order.
	if(shead==NULL){
		shead=(sw*)malloc(sizeof(sw));
		shead->term=(char*)malloc(sizeof(char)*strlen(p));
		strcpy(shead->term,p);
		shead->next=NULL;
		return shead;
	}else{	
		sw *temp=shead;
		sw *temp2=(sw*)malloc(sizeof(sw));
		temp2->term=(char*)malloc(sizeof(char)*strlen(p));
		strcpy(temp2->term,p);
		if ( strcmp( shead->term,p )>0  ){
				temp2->next=shead;
				shead=temp2;
				return temp2;
		}
		while(temp->next!=NULL && strcmp(temp->next->term,p) < 0 ){
			temp=temp->next;
		}
		temp2->next=temp->next;
		temp->next=temp2;
		return temp2;
	}
}

void insertIndex(int index,int occurence){//insert indexes and occurences to list.
	if(ohead==NULL){
		ohead=(oa*)malloc(sizeof(oa));
		ohead->index=index;
		ohead->occurence=occurence;
		ohead->next=NULL;
	}else{
		oa *temp=ohead;
		oa *temp2=(oa*)malloc(sizeof(oa));
		temp2->index=index;
		temp2->occurence=occurence;
		while(temp->next != NULL )
			temp = temp->next;
		temp->next=temp2;
		temp2->next=NULL;
	}
}

void printEx(){
	repNode *temp=fhead;
	FILE *fp;
	fp = fopen("heyo.txt","a");
	while(temp!=NULL){
		fprintf(fp,"%s  -  %s  -  %s\n",temp->term,temp->id,temp->category);
		temp=temp->next;
	}
	fclose(fp);
}

void insertTextDetail(char *id,char *category,int length){//text file details
	if(thead==NULL){
		thead=(tl*)malloc(sizeof(tl));
		thead->category=(char*)malloc(sizeof(char)*strlen(category));
		thead->textName=(char*)malloc(sizeof(char)*strlen(id));
		strcpy(thead->category,category);
		strcpy(thead->textName,id);
		thead->length=length;
		thead->next=NULL;
	}else{
		tl *temp=thead;
		tl *temp2=(tl*)malloc(sizeof(tl));
		temp2->length=length;
		temp2->category=(char*)malloc(sizeof(char)*strlen(category));
		temp2->textName=(char*)malloc(sizeof(char)*strlen(id));
		strcpy(temp2->category,category);
		strcpy(temp2->textName,id);
		
		while(temp->next != NULL )
			temp = temp->next;
		
		temp->next=temp2;
		temp2->next=NULL;
	}
}

void insertToTempList(char *p,char *category,char *id){
	if(fhead==NULL){
		fhead=(repNode*)malloc(sizeof(repNode));
		int length=strlen(p);
		fhead->term = (char*)malloc(sizeof(char)* (length+1) );
		strcpy(fhead->term,p);
		strcpy(fhead->category,category);
		strcpy(fhead->id,id);
		fhead->next=NULL;
	}else{
		repNode *temp=fhead;
		repNode *temp2=(repNode*)malloc(sizeof(repNode));
		int length=strlen(p);
		temp2->term = (char*)malloc(sizeof(char)* (length+1) );
		strcpy(temp2->term,p);
		strcpy(temp2->category,category);
		strcpy(temp2->id,id);
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp2->next=temp->next;
		temp->next=temp2;
	}	
}

exNode* insertToMaster(char *p){
	if(mhead==NULL){
		mhead=(exNode*)malloc(sizeof(exNode));
		int length=strlen(p);
		mhead->term = (char*)malloc(sizeof(char)* (length+1) );
		strcpy(mhead->term,p);
		mhead->numberOfDocs=1;
		mhead->next=NULL;
		return mhead;
	}else{
		exNode *temp=mhead;
		exNode *temp2=(exNode*)malloc(sizeof(exNode));
		int length=strlen(p);
		temp2->term = (char*)malloc(sizeof(char)* (length+1) );
		strcpy(temp2->term,p);
		temp2->numberOfDocs=1;
		
		if ( strcmp( mhead->term,p )>0  ){
				temp2->next=mhead;
				mhead=temp2;
				return temp2;
		}
		
		while(temp->next!=NULL && strcmp(temp->next->term,p) < 0 ){
			temp=temp->next;
		}
		temp2->next=temp->next;
		temp->next=temp2;	
		return temp2;
	}
}

int ifExistsMaster(char *p){
	exNode *temp=mhead;
	while(temp!=NULL){
		if(!strcmp(temp->term,p)){
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

void searchAndInsertToMaster(char *p,char *category,char *id){//search whether the term exists in master linked list.
	repNode *temp=fhead;
	exNode *temp2;
	if(!ifExistsMaster(p)){
		temp2 = insertToMaster(p);
		temp2->head=NULL;
		while(temp!=NULL){
			if( !strcmp(temp->term,p) && (strcmp(category,temp->category) || strcmp(id,temp->id)) ){
				temp2->numberOfDocs++;
			}
			temp=temp->next;
		}
	}
}

void transformToMaster(){//inserts the terms by their occurences into master linked list by reading my temporary linked list.
	repNode *temp=fhead;
	while(temp!=NULL){
		if( strcmp(temp->term," "))
			searchAndInsertToMaster(temp->term,temp->category,temp->id);
		temp=temp->next;
	}
}

void recordToTempList(char *p,char *category,char *id){//in order to count the words for master linked list and its inner list
													   //without reading the text files again and again, i put them into a temporary linked list
													   //that i created.
	insertToTempList(p,category,id);
}

void readText(char *p,char *category,char *id){//reading the text comes from traverse directory
	FILE *fp;
    char str[50];
    fp = fopen(p, "r");
    if (fp == NULL){
        return;
    }
    char a;
    int wordCount=0;
    int length=0;
    while ( !feof(fp)){
    	fscanf(fp,"%s",str);
    	wordCount++;
    	length+=strlen(str);
        recordToTempList(str,category,id);
	}
	insertTextDetail(id,category,(wordCount));
    fclose(fp);
}

void traverseDirectory(){//getting text names under /dataset and dataset/econ dataset/magazine etc.
   	
	struct dirent *de;
	char dataset[30]="./dataset";
    DIR *dr = opendir(dataset); 
    if(dr==NULL){
    	printf("directory can not be read");
    	exit(0);
	}
	readdir(dr);
	readdir(dr);

	DIR *in;
	struct dirent *de2;
	char inner[30];
	char text[50];
	while ((de = readdir(dr)) != NULL){
		strcpy(inner,"./dataset/");
		strcat(inner,de->d_name);
		in = opendir(inner); 
		while ((de2 = readdir(in)) != NULL){
			strcpy(text,inner);
			strcat(text,"/");
			strcat(text,de2->d_name);
			readText(text,de->d_name,de2->d_name);
		}
	}
    closedir(dr);     
}

void printFirstList(){//print temp list
	repNode *temp=fhead;
	while(temp!=NULL){
		printf("%s - %s - %s\n",temp->term,temp->id,temp->category);
		temp=temp->next;
	}
}

void printMl(){//prints only master linked list
	exNode *temp=mhead;
	while(temp!=NULL){
		printf("%d %s\n",temp->numberOfDocs,temp->term);
		temp=temp->next;
	}
}

int ifExistsInner(exNode *temp,repNode *temp2){//if text id and category is inserted in a term, no need to insert again.
	inNode *temp3=temp->head;
	while(temp3!=NULL){
		if( !strcmp(temp3->category,temp2->category) && !strcmp(temp3->docID,temp2->id) ){
			return 1;
		}
		temp3=temp3->next;
	}
	return 0;
}

int findTextLength(char *category,char *p){//find text file length
	tl *temp=thead;
	while(temp!=NULL){
		if( !strcmp(temp->category,category) && !strcmp(temp->textName,p) ){
			return temp->length;
		}
		temp=temp->next;
	}
}

void insertInnerNode(int occurence,char *id,char *category,char *p,exNode *temp){//inserts word occurences, text ids, category namely, inner nodes of master.
	if(temp->head==NULL){
		temp->head=(inNode*)malloc(sizeof(inNode));
		temp->head->occurrence=occurence;
		strcpy(temp->head->category,category);
		strcpy(temp->head->docID,id);
		temp->head->length=findTextLength(category,id);
		temp->head->next=NULL;
	}else{
		inNode *temp2 = temp->head;
		inNode *temp3 = (inNode*)malloc(sizeof(inNode));
		while(temp2->next!=NULL){
			temp2=temp2->next;
		}
		temp2->next=temp3;
		strcpy(temp3->category,category);
		strcpy(temp3->docID,id);
		temp3->length=findTextLength(category,id);
		temp3->occurrence=occurence;
		temp3->next=NULL;	
	}	
}

int findOccurenceInText(char *category,char *id,char *p,exNode *temp2){//find the word occurence in a spesific text and category.
	repNode *temp = fhead;
	int occurence=0;
	while(temp!=NULL){
		if( !strcmp(p,temp->term) && (!strcmp(category,temp->category) && !strcmp(id,temp->id))  ){
			occurence++;
		}
		temp=temp->next;
	}
	return occurence;
}

void recordInner(exNode *temp){
	repNode *temp2=fhead;
	while(temp2!=NULL){
		if( !strcmp(temp->term,temp2->term) ){
			if( !ifExistsInner(temp,temp2) ){//if document id and category is not in the term's inner node then add.
				insertInnerNode(findOccurenceInText(temp2->category,temp2->id,temp2->term,temp),temp2->id,temp2->category,temp2->term,temp);
			}
		}
		temp2=temp2->next;
	}
}

void addInnerNodes(){//adding inner nodes to the terms ( master )
	exNode *temp=mhead;
	while(temp!=NULL){
		recordInner(temp);
		temp=temp->next;
	}
}

void swapNodes(oa *a,oa *b){
	int tempIndex=a->index;
	int tempOccurence=a->occurence;
	
	a->index=b->index;
	a->occurence=b->occurence;
	
	b->index=tempIndex;
	b->occurence=tempOccurence;		
}

void fullLinkedListTraverse(){//prints the master linked and its inner nodes
	exNode *temp=mhead;
	inNode *temp3;
	//FILE *fp;
//	fp=fopen("outputsofml.txt","w");
	while(temp!=NULL){
		temp3=temp->head;
		printf("\n%s - %d | ",temp->term,temp->numberOfDocs);
	//	fprintf(fp,"\n%s-%d | ",temp->term,temp->numberOfDocs);
		while(temp3!=NULL){
		//	fprintf(fp,"%s %s %d",temp3->docID,temp3->category,temp3->occurrence);
			printf("(%s) (%s) (%d) (%d) || ",temp3->docID,temp3->category,temp3->occurrence,temp3->length);
			temp3=temp3->next;
		}
		printf("\n");
		temp=temp->next;
	}
}

void discriminatingWords(){//find and writes discriminating words
	struct discriminates discs;
	exNode *temp=mhead;
	char category[20],term[20];
	int flag=1;
	inNode *temp2;
	int econCounter=0,magazineCounter=0,healthCounter=0;
	while(temp!=NULL){
		temp2=temp->head;
		strcpy(term,temp->term);
		strcpy(category,temp2->category);
		temp2=temp2->next;
		flag=1;
		while(temp2!=NULL){	
			if( strcmp(category,temp2->category) ){
				flag=0;
				break;
			}
			temp2=temp2->next;
		}
		
		if(flag){
			if(!strcmp(category,"magazin") && magazineCounter<5 ){
				strcpy(discs.magazine[magazineCounter],term);
				magazineCounter++;
			}else if(!strcmp(category,"econ") && econCounter<5 ){
				strcpy(discs.econ[econCounter],term);
				econCounter++;
			}else if(!strcmp(category,"health") && healthCounter<5){
				strcpy(discs.health[healthCounter],term);
				healthCounter++;
			}	
		}
		temp=temp->next;
	}
	
	printf("\n\nmagazine: ");
	for(int i=0;i<5;i++){
		printf("%s  ",discs.magazine[i]);
		if(strlen(discs.magazine[i])==0){
			break;			
		}
	}
	printf("\necon    : ");
	for(int i=0;i<5;i++){
		printf("%s  ",discs.econ[i]);
		if(strlen(discs.econ[i])==0){
			break;			
		}
	}
	printf("\nhealth  : ");
	for(int i=0;i<5;i++){
		printf("%s  ",discs.health[i]);
		if(strlen(discs.health[i])==0){
			break;			
		}
	}
}

void stopWords(){
	
	exNode *exTemp=mhead;//insert index and occurence nodes
	int index=0;
	while(exTemp!=NULL){
		insertIndex(index,exTemp->numberOfDocs);
		index++;
		exTemp=exTemp->next;
	}
	
	int swapped = 0;//sort indexes and occurences
	oa *temp=ohead;
	oa *lptr=NULL;
	do{
		swapped=0;
		temp=ohead;
		while(temp->next!=lptr){
			if( temp->occurence < temp->next->occurence ){
				swapNodes(temp,temp->next);
				swapped=1;
			}
			temp=temp->next;
		}
		lptr = temp;
	}while(swapped);
	
	
	oa *temp3=ohead;//print first 5
	exNode *toIterate;
	int firstfive=0;
	while(firstfive<5){
		toIterate=mhead;
		for(int i=0;i<temp3->index;i++){
			toIterate=toIterate->next;
		}
		insertStopWords(toIterate->term);
		firstfive++;
		temp3=temp3->next;
	}
}

void printStopWords(){
	sw *temp=shead;
	int i=1;
	while(temp!=NULL){
		printf("Term %d : %s\n",i++,temp->term);
		temp=temp->next;
	}
}


int main(){
traverseDirectory();
transformToMaster();
addInnerNodes();
fullLinkedListTraverse();
printf("\n\nDisciminating word");
discriminatingWords();
printf("\n\nStop words\n");

stopWords();
printStopWords();
getchar();
}
