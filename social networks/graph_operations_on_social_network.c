/*
Dependency : input.txt on same direction
Computes centrality values for each node. 
Degree Centrality
Closeness Centrality
Betweenness Centrality
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>
#include <time.h>

struct node{
	int data;
	struct node *next;
};

typedef struct node Node;

struct AdjListNode 
{ 
    char name[50];
    struct AdjListNode* next;
};

typedef struct AdjListNode AdjListNode;

struct AdjList 
{
	char name[50];
    struct AdjListNode *head;
};
typedef struct AdjList AdjList;

struct Graph
{
    int V;
    struct AdjList* array;
}; 

typedef struct Graph Graph;

struct onePathInfo{
	int src,dest;
	Node *head;
	struct onePathInfo *next;
};

typedef struct onePathInfo onePathInfo;

Node *head=NULL;
onePathInfo *ohead=NULL;

struct Graph* createGraph(int V) 
{ 
    Graph* graph = ( Graph*) malloc(sizeof( Graph)); 
    graph->V = V;
    graph->array =  ( AdjList*) malloc(V * sizeof( AdjList));
  
    int i; 
    for (i = 0; i < V; ++i){
    	graph->array[i].head = NULL;
    	strcpy(graph->array[i].name,"");
	}
    return graph; 
}

void addSource(Graph *graph,int position,char *vertex){
	strcpy(graph->array[position].name,vertex);
}
  
int findSource(Graph *graph,char *source){
	for(int i=0;i<graph->V;i++){
		if(strlen(graph->array[i].name)<1){
			return i;
		}
	}
}

void addEdge( Graph* graph, int src, char *dest) 
{
    if(graph->array[src].head==NULL){
   	    graph->array[src].head = (AdjListNode*)malloc(sizeof(AdjListNode));
   	 	strcpy(graph->array[src].head->name,dest);
   	 	graph->array[src].head->next=NULL;
	}else{
		AdjListNode *temp=graph->array[src].head;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		AdjListNode *temp2 = (AdjListNode*)malloc(sizeof(AdjListNode));
		strcpy(temp2->name,dest);
		temp2->next=NULL;
		temp->next=temp2;
	}
}

void printGraph(Graph* graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; ++v) 
    { 
        AdjListNode* pCrawl = graph->array[v].head; 
        printf("\n%s",graph->array[v].name ); 
        while (pCrawl!=NULL) 
        {
            printf(" -> %s", pCrawl->name); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
}

int ifAdjacent(AdjList adj,char *dest){
	AdjListNode *temp=adj.head;
	while(temp!=NULL){
		if( !strcmp(temp->name,dest) ){
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

void printAdjacencyMatrix(Graph *graph,int **adjacencyMatrix){
	for(int i=0;i<=45;i++){
		printf("-");
	}
	printf("\nAdjacency Matrix\n");
	for(int i=0;i<=45;i++){
		printf("-");
	}
	printf("\n");
	printf("Characters are representing names C:Cem, A:Ayse ..\n");
	printf("\n         ");
	for(int i=0;i<graph->V;i++){
		printf("%c  ",graph->array[i].name[0]);
	}
	printf("\n");
	for(int i=0;i<graph->V;i++){
		printf("%s",graph->array[i].name);
		for(int j=0;j<10-(strlen(graph->array[i].name))-1;j++){
			printf(" ");
		}
		for(int k=0;k<graph->V;k++){
			if(ifAdjacent(graph->array[i],graph->array[k].name) ){
				printf("1");
				adjacencyMatrix[i][k]=1;
			}else{
				printf("0");
				adjacencyMatrix[i][k]=0;
			}
			printf("  ");			
		}
		printf("\n");
	}
}

int findVerticeCount(char *file){
	FILE *f;
	f=fopen(file,"r");
	if(f==NULL){
		printf("file is not found");
		exit(0);
	}
	char word[20],ch;
	int lines=1;
	while(!feof(f)){
		ch = fgetc(f);
		if(ch == '\n'){
			lines++;
		}
	}
	return lines;
}

char * deblank(char *str){
  char *out = str, *put = str;
  for(; *str != '\0'; ++str){
    if(*str != ' '){
    	*put++ = *str;
	}
  }
  *put = '\0';
  return out;
}

void parseLineThenAdd(Graph *graph,char *line){
	line = deblank(line);
	char vertice[30]="",adjacent[30];
	int indexV=0,indexA=0,i=0;
	for(i;line[i]!=';';i++){
		vertice[i]=line[i];
	}
	i++;
	int whereToAdd =findSource(graph,vertice);
	addSource(graph,whereToAdd,vertice);
	char *o = &line[i];
	char *token;
    const char s[2] = ",";
    token = strtok(o, s);
    while( token != NULL ) {
    	
    	addEdge(graph,whereToAdd,token);
    	token = strtok(NULL, s);
    }	
}

void addVerticesFromInput(Graph *graph,char *file){
	FILE *fp;
	fp=fopen(file,"r");
	
	char buffer[512];
	char tempBuff[512];
	char vertice[30],tempWord[30],ch,fullText[200];
	int index=0,indexForTempWord=0;
	while(fgets(buffer, 512, (FILE*) fp)) {
		if(buffer[strlen(buffer)-1]=='\n'){
			buffer[strlen(buffer)-1]='\0';	
		}
		parseLineThenAdd(graph,buffer);
	}
}

void append(Node** head_ref, int new_data) { 
    Node* new_node = (Node*) malloc(sizeof(Node)); 
  
    Node *last = *head_ref;  
   
    new_node->data  = new_data; 
 
    new_node->next = NULL; 
  
    if (*head_ref == NULL) 
    { 
       *head_ref = new_node; 
       return; 
    }   
       
    while (last->next != NULL) 
        last = last->next; 
   
    last->next = new_node; 
    return;     
} 

int queueSize(Node *head){
	Node *temp=head;
	int counter=0;
	while(temp!=NULL){
		counter++;
		temp=temp->next;
	}
	return counter;
}

void popFront(Node **head){
	Node *temp=(*head);
	(*head)=(*head)->next;
	free(temp);
}

int adjacentLength(AdjListNode *head){
	AdjListNode *temp=head;
	int counter=0;
	while(temp!=NULL){
		counter++;
		temp=temp->next;
	}
	return counter;
}

int geti(Graph *graph,AdjListNode *head,int j){
	AdjListNode *temp=head;
	for(int i=0;i<j;i++){
		temp=temp->next;
	}
	for(int i=0;i<graph->V;i++){
		if(!strcmp(temp->name,graph->array[i].name)){
			return i;
		}
	}
}

void deleteAllQueue(Node **queue){
	Node *temp;
	if(*queue==NULL){
		return;
	}
	while (*queue != NULL) {
	    temp = *queue;
	    *queue = temp->next;
	    free(temp);
	}
}


int BFS(Graph *graph, int src, int dest, int v, 
                            int pred[], int dist[]) 
{

    Node *queue = NULL;
   
    int visited[v]; 
  
    for (int i = 0; i < v; i++) { 
        visited[i] = 0; 
        dist[i] = INT_MAX; 
        pred[i] = -1; 
    } 

    visited[src] = 1; 
    dist[src] = 0; 
    append(&queue,src);
  	
    // standard BFS algorithm 
    while (queue!=NULL) { 
        int u = queue->data;
        popFront(&queue);
        for (int i = 0; i < adjacentLength(graph->array[u].head); i++) {
        	int l = geti(graph,graph->array[u].head,i);
            if (visited[l] == 0) { 
                visited[l] = 1; 
                dist[l] = dist[u] + 1; 
                pred[l] = u; 
                append(&queue,l);
  
                // We stop BFS when we find 
                // destination. 
                if (l == dest){
                Node *temp;
				while (queue != NULL) {
				    temp = queue;
				    queue = temp->next;
				    free(temp);
				}
				//deleteAllQueue(&queue);
                   return 1;
                }
            } 
        } 
    } 
    return 0; 
} 

void printList(Node *head){
	Node *temp=head;
	while(temp!=NULL){
		printf("%d ",temp->data);
		temp=temp->next;
	}
	printf("\n");
}

int findShortestDistance(Graph *graph, int s,  
                                    int dest, int v) 
{ 

    int pred[v], dist[v]; 
  
    if (BFS(graph, s, dest, v, pred, dist) == 0) 
    { 
        //printf("fail");
        return 0;
    } 
  	
    Node *path=NULL;
    int crawl = dest; 
    append(&path,crawl); 
    while (pred[crawl] != -1) { 
       	append(&path,pred[crawl]); 
        crawl = pred[crawl]; 
    } 

  	int f = dist[dest];
	Node *temp15;
	while (path != NULL) {
	    temp15 = path;
	    path = temp15->next;
	    free(temp15);
	}
  	return f;
}

void printScoreTable(){
	for(int i=0;i<=38;i++){
		printf("-");
	}
	printf("\n\n\nSource      DegreeCentrality      ClosenessCentrality    BetweennessCentrality \n");
}

void printDegreeCentrality(Graph *graph){
	for(int i=0;i<=38;i++){
		printf("-");
	}
	printf("\nDegree Centrality\n");
	
	int tempLength;
	for(int i=0;i<graph->V;i++){
		tempLength=adjacentLength(graph->array[i].head);
		
		printf("%d/%d\n",tempLength,graph->V-1);
	}
	for(int i=0;i<=38;i++){
		printf("-");
	}
	printf("\n\n");
}


void addForDfs(int data){
	if(head==NULL){
		head = (Node*)malloc(sizeof(Node));
		head->data=data;
		head->next=NULL;
		return;
	}
	
	Node *temp=head;
	while(temp->next!=NULL){
		temp=temp->next;
	}
	Node *temp2 = (Node*)malloc(sizeof(Node));
	temp2->data=data;
	temp->next=temp2;
	temp2->next=NULL;
}

void deleteAll(){
	Node *temp;
	while (head != NULL) {
	    temp = head;
	    head = temp->next;
	    free(temp);
	}
}

void removeIntervalNode(int index){
	Node *temp = head; 
	int i=0;
	if(index==0){
		head = temp->next;   // Changed head 
        free(temp);               // free old head 
        return; 
	}
	while(i<index-1){
		temp=temp->next;
		i++;
	}
	Node *temp2=temp->next;
	temp->next=temp->next->next;
	free(temp2);
}

void removeIntervalFromPaths(int index){
	onePathInfo *temp = ohead; 
	int i=0;
	if(index==0){
		ohead = temp->next;   // Changed head 
        free(temp);               // free old head 
        return;
	}
	while(i<index-1){
		temp=temp->next;
		i++;
	}
	onePathInfo *temp2=temp->next;
	temp->next=temp->next->next;
	free(temp2);
}

int color[10]={0,0,0,0,0,0,0,0,0,0};
int size=0;

int findGivenPathLength(){
	Node *temp=head;
	int counter=0;
	while(temp!=NULL){
		counter++;
		temp=temp->next;
	}
	return counter;
}

void addInfo(Node **k,int data){
	//printf("%d ",data);
	if(*k==NULL){
		(*k)=(Node*)malloc(sizeof(Node));
		(*k)->data=data;
		(*k)->next=NULL;
		return;
	}
	
	Node *temp2 = (Node*)malloc(sizeof(Node));
	Node *temp=(*k);
	temp2->data=data;
	while(temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=temp2;
	temp2->next=NULL;
}

void add(Node *head){
	Node *temp=head;
	int src = temp->data;
	
	while(temp->next!=NULL){
		temp=temp->next;
	}
	int dest=temp->data;
	
	if(ohead==NULL){
		ohead=(onePathInfo*)malloc(sizeof(onePathInfo));
		ohead->src=src;
		ohead->dest=dest;
		ohead->next=NULL;
		ohead->head=NULL;
		temp=head;
		while(temp!=NULL){
			//printf("%d",temp->data);
			addInfo(&(ohead->head),temp->data);
			temp=temp->next; 
		}
	}else{
		onePathInfo *temp2=ohead;
		while(temp2->next!=NULL){
			temp2=temp2->next;
		}
		onePathInfo *temp3=(onePathInfo*)malloc(sizeof(onePathInfo));
		temp3->head=NULL;
		temp3->src=src;
		temp2->next=temp3;
		temp3->dest=dest;
		temp3->next=NULL;
		temp=head;
		while(temp!=NULL){
			addInfo(&(temp3->head),temp->data);
			temp=temp->next; 
		}	
	}
	
}

void removeToArrange(Node *head){
	
	Node *temp=head;
	int i=0;
	while(temp!=NULL){
		removeIntervalFromPaths(temp->data-i);
		temp=temp->next;
		i+=1;
	}
	printf("\n");
}

void arrangePaths(int v){
	
	onePathInfo *temp=ohead;
	Node *temp2;
	Node *listToBeDeleted=NULL;
	Node *lol;
	int i=0,k=0,j=0;
	int path1[v];
	int path2[v];
	onePathInfo *tmp = ohead;
	while(temp!=NULL){
		
		if(temp->src==temp->dest){
			append(&listToBeDeleted,i);
		}else{
			if( temp->dest < temp->src ){
				append(&listToBeDeleted,i);
			}		
		}
		temp=temp->next;
		i+=1;
	}	
	removeToArrange(listToBeDeleted);
}


void printInfos(){
	onePathInfo *temp=ohead;
	Node *temp2;
	while(temp!=NULL){
		temp2=temp->head;
		printf("\n%d %d ",temp->src,temp->dest);
		while(temp2!=NULL){
			printf("%d",temp2->data);
			temp2=temp2->next;
		}
		temp=temp->next;
	}
}

void dfs(Graph *graph,int src,int dest,int length){
	
	addForDfs(src);
	size++;
	color[src]=1;
	if(src==dest){
		int i = findGivenPathLength();
		if(length==i-1){
			add(head);
		}
		return;
	}
	
	for(int i=0;i<adjacentLength(graph->array[src].head);i++){
		int l = geti(graph,graph->array[src].head,i);
		if(color[l]==0){
			dfs(graph,l,dest,length);
			color[l]=0;
			size--;
			removeIntervalNode(size);
		}
	}
}

int listLength(){
	Node *temp=head;
	int counter=0;
	while(temp!=NULL){
		counter++;
		temp=temp->next;
	}
	return counter;
}

int lengthOfASpesificList(Node *k){
	Node *temp=k;
	int counter=0;
	while(temp!=NULL){
		counter++;
		temp=temp->next;
	}
	return counter;
}

void printClosenessCentrality(Graph *graph){
	
	printf("\nCloseness Centrality\n");
	int tempShortestLength=0;
	for(int i=0;i<graph->V;i++){
		tempShortestLength=0;
		for(int j=0;j<graph->V;j++){
			tempShortestLength+= findShortestDistance(graph, i, j, graph->V); 
		}
		
		printf("%d/%d\n",graph->V-1,tempShortestLength);
	}
	for(int i=0;i<=38;i++){
		printf("-");
	}
	printf("\n\n");
}

int findOccurenceInIntermediaPath(int vertex){
	onePathInfo *temp=ohead;
	Node *temp2;
	int counter=0;
	while(temp!=NULL){
		temp2=temp->head;
		if(lengthOfASpesificList(temp2)>2){
			while(temp2!=NULL){
				if(temp2!=temp->head && temp2->next!=NULL){
					if(temp2->data==vertex){
						counter++;
					}
				}
				temp2=temp2->next;
			}
		}
		temp=temp->next;
	}
	return counter;
}
int findOccurenceInPath(int k){
	onePathInfo *temp=ohead;
	int counter=0;
	Node *temp2;
	while(temp!=NULL){
		temp2=temp->head;
		while(temp2!=NULL){
			if(k==temp2->data){
				counter++;
			}
			temp2=temp2->next;
		}
		temp=temp->next;
	}
	return counter;
}

void calculateBts(Graph *graph,int arrayOfBts[],int arrayOfBts2[]){
	for(int i=0;i<graph->V;i++){
		arrayOfBts[i] = findOccurenceInIntermediaPath(i);
	}
	for(int i=0;i<graph->V;i++){
		arrayOfBts2[i]=findOccurenceInPath(i); 
	}
}

int numCount(int a){
	int counter=0;
	while(a!=0){
		a/=10;
		counter++;
	}
	return counter;
}

void printBtsMatrix(Graph *graph){
	printf("\n");
	for(int i=0;i<=45;i++){
		printf("-");
	}
	printf("\nBetweenness Centrality\n");
	for(int i=0;i<=45;i++){
		printf("-");
	}

	onePathInfo *temp=ohead;
	onePathInfo *temp3 = ohead;
	Node *temp2;
	int cnt;
	char  tempPathLength[200];
	printf("\nSource   Target        Intermadia-Nodes                   Path\n");
	while(temp!=NULL){
		strcpy(tempPathLength,"");
		cnt=0;
		printf("%s",graph->array[temp->src].name);
		for(int r=0;r<10-strlen(graph->array[temp->src].name) && r>=0 ;r++){
			printf(" ");
		}
		
		printf("%s",graph->array[temp->dest].name);
		for(int r=0;r<16-strlen(graph->array[temp->dest].name) && r>=0 ;r++){
			printf(" ");
		}
		temp2=temp->head;
		if(lengthOfASpesificList(temp2)>2){
			while(temp2!=NULL){
				if(temp2!=temp->head && temp2->next!=NULL){
					printf("%s,",graph->array[temp2->data].name);
					cnt+=1;
					strcat(tempPathLength,graph->array[temp2->data].name );
				}
				temp2=temp2->next;
			}
		for(int q=0;q<30-strlen(tempPathLength)-cnt;q++){
			printf(" ");
			
		}
		}else{
			printf("-");
			for(int q=0;q<30-1;q++){
				printf(" ");
			}
		}
		temp2=temp3->head;
		while(temp2!=NULL){
			printf("%s-",graph->array[temp2->data]);
			temp2=temp2->next;
		}
		temp3=temp3->next;
		
		printf("\n");
		temp=temp->next;
	}
	
}

void printBtsValues(int array[],int size,Graph *graph,int arrayOfOccurenceInPath[]){
	printf("\n\n");
	int n=(size-1)*(size-2) /2;
	float standartBtsValue;

	for(int i=0;i<size;i++){
		if(array[i]==0){
			printf("Cbetwennes (%s) = 0 \n",graph->array[i].name);
		}else{
			 
			printf("Cbetwennes (%s) = %d/%d \n",graph->array[i].name,array[i],arrayOfOccurenceInPath[i]);
		}
	}
	printf("\nAfter standardization (n-1)(n-2)/2=%d\n",n);
	for(int i=0;i<size;i++){
		if(array[i]==0){
			printf("Cbetwennes (%s) = 0 \n",graph->array[i].name);
		}else{
			standartBtsValue=(float) array[i]/arrayOfOccurenceInPath[i]/n;
			printf("Cbetwennes (%s) = %d/%d = %f \n",graph->array[i].name,array[i],n*arrayOfOccurenceInPath[i],standartBtsValue);
		}	
	}
}

void printBtsCentrality(Graph *graph,int arrayOfBts[],int arrayOfNonIntermedia[]){
	
	int a,l;
	for(int i=0;i<graph->V;i++){
		for(int j=0;j<graph->V;j++){
				a=findShortestDistance(graph, i, j, graph->V);
				dfs(graph,i,j,a);
				for(int i=0;i<graph->V;i++){
					color[i]=0;
				}
				l = listLength();
				while(l){
					removeIntervalNode(l-1);
					l-=1;
				}
				size=0;	
		}
	}
	arrangePaths(graph->V);
	//printInfos();
	
	calculateBts(graph,arrayOfBts,arrayOfNonIntermedia);
	printf("\n");
}
void printFirstTable(int array[],int size,Graph *graph,int arrayOfOccurenceInPath[]){
	printScoreTable();
	int tempLength,spaceCoefficent=15,tempShortestLength=0;
	float dcv;
	int n=(size-1)*(size-2) /2;
	float standartBtsValue;
	for(int i=0;i<graph->V;i++){
		printf("%s",graph->array[i].name);
		for(int j=0;j<spaceCoefficent-strlen(graph->array[i].name); j++){
			printf(" ");
		}
		
		tempLength=adjacentLength(graph->array[i].head);
		printf("%f",(float)tempLength/(graph->V-1) );

		for(int i=0;i<spaceCoefficent;i++){
			printf(" ");
		}
		tempShortestLength=0;
		for(int j=0;j<graph->V;j++){
			tempShortestLength+= findShortestDistance(graph, i, j, graph->V); 
		}
		printf("%f",(float)(graph->V-1)/tempShortestLength);
		for(int i=0;i<spaceCoefficent;i++){
			printf(" ");
		}
		if(array[i]==0){
			printf("0.00000\n",graph->array[i].name);
		}else{
			standartBtsValue=(float) array[i]/arrayOfOccurenceInPath[i]/n;
			printf("%f \n",standartBtsValue);
		}	
	}
}

int main() 
{
	int verticeCount = findVerticeCount("input.txt");
    Graph* graph = createGraph(verticeCount);
	addVerticesFromInput(graph,"input.txt");
    //printGraph(graph); 
	int **adjacencyMatrix;
	adjacencyMatrix = (int**)malloc(graph->V * sizeof(*adjacencyMatrix) );
	for (int i=0; i<graph->V; i++)
	{
		adjacencyMatrix[i] = (int*)malloc(graph->V * sizeof(int) );
	}
  	printAdjacencyMatrix(graph,adjacencyMatrix);
  	int arrayOfBts[graph->V];
	int arrayOfNonIntermedia[graph->V];
	
  	printBtsCentrality(graph,arrayOfBts,arrayOfNonIntermedia);
  	printFirstTable(arrayOfBts,graph->V,graph,arrayOfNonIntermedia);
    
    printBtsMatrix(graph);
	printBtsValues(arrayOfBts,graph->V,graph,arrayOfNonIntermedia);
    return 0; 
} 
