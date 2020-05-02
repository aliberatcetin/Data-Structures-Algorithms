#include <stdio.h>
#include <stdlib.h>
/*
Implements a ternary search tree which obeys that rule : Key(LCA) < Key(A) < Key(MCA) ≤ (Key(A))^2 < Key(RCA)

*/
struct node{
	int data;
	struct node *left;
	struct node *right;
	struct node *mid;	
};

typedef struct node Node;

void insert(Node **root,int key){//insert the nodes into list.
	if( (*root)==NULL ){
		(*root)=(Node*)malloc(sizeof(Node));
		(*root)->data=key;
		(*root)->left=NULL;
		(*root)->right=NULL;
		(*root)->mid=NULL;
		return;
	}
	
	if( (*root)->data > key )
		insert(&(*root)->left,key);
	else if( key > (*root)->data * (*root)->data )
		insert(&(*root)->right,key);
	else
		insert(&(*root)->mid,key);
}


Node *search( Node *root, int key ){
	if(root==NULL){
		return NULL;
	}
	
	if( key < root->data )
		search(root->left,key);
	else if( key > ( (root->data) * (root->data) ))
		search(root->right,key);
	else if (  key > root->data )
		search(root->mid,key);
	else
		return root;
}

Node * biggest(Node * node){ //finds the biggest element in a given "root" 
	if(node==NULL)
		return NULL;
    while(1){
    	if( node->right!=NULL )
    		node=node->right;
    	else if( node->mid!=NULL )
    		node=node->mid;
    	else
    		return node;
	} 
} 

Node * smallest(Node *node){ //finds the smallest element in a given "root" 
	if(node==NULL)
		return NULL;
    while(1){
    	if( node->left!=NULL )
    		node=node->left;
   		return node;	
	} 
}

Node* removeNode(Node *root, int key){
	if ( root == NULL) return root;


	if (key < root->data) 
        root->left=removeNode( root->left, key);
    else if ( key > root->data * root->data )
    	root->right=removeNode( root->right,key );
    else if( key > root->data )
    	root->mid = removeNode( root->mid, key );
    
    else{
    	if( root->left==NULL && root->right==NULL  ){
    		Node *temp = root->mid;
    		free(root);
    		return temp;
		}else if( root->left==NULL && root->mid==NULL  ){
			Node *temp = root->right;
    		free(root);
    		return temp;
		}else if ( root->right==NULL && root->mid==NULL ){
			Node *temp = root->left;
    		free(root);
    		return temp;
		}
		if ( root->left==NULL ){
			Node *temp = smallest( root->mid );
			root->data=temp->data;
			root->mid = removeNode( root->mid, temp->data);
		}else{
			Node *temp = biggest( root->left );
			root->data=temp->data;
			root->left =  removeNode( root->left, temp->data);
		}
	}  
	return root; 
}

void printTreeLMRN(Node *root){
	if(root==NULL){
		return;
	}
		
	
	printTreeLMRN( root->left);
	printTreeLMRN( root->mid);
	printTreeLMRN( root->right);
	
	printf("%d ",root->data);
	
}

void  findBrokenRoot(Node **root,Node ***root3){ //finds the broken node by post order traversal.
	if((*root)==NULL)
		return ;
	
	findBrokenRoot( &(*root)->left,root3);
	findBrokenRoot( &(*root)->mid,root3);
	findBrokenRoot( &(*root)->right,root3);
	
	Node **mid = &(*root)->mid;
	Node **right = &(*root)->right;
	
	if( (*mid) !=NULL && ((*root)->mid->data > (*root)->data*(*root)->data) ){
		(*root3) = mid;
	}
	
	if( (*right)!=NULL && ((*root)->right->data < (*root)->data*(*root)->data) ){
		(*root3) = right; 
	}
	
}

void rebuildTheBrokenTree(Node **root,Node **root2,Node **root3){ //makes a post order traversal to the broken tree and inserts again.
	if((*root)==NULL)
		return ;

	rebuildTheBrokenTree( &(*root)->left,root2,root3);
	rebuildTheBrokenTree( &(*root)->mid,root2,root3);
	rebuildTheBrokenTree( &(*root)->right,root2,root3);

	int tempData = (*root)->data;
	(*root3)  = removeNode( (*root3) , tempData );
	insert( root2,tempData );
}

void addFromInputFile(Node **root){
	FILE *fp;
    int num;
    fp = fopen("input.txt", "r");
    if (fp == NULL){
        return;
    }

    while ( !feof(fp)){
    	fscanf(fp,"%d",&num);
    	insert(root,num);
	}
    fclose(fp);
}

int main(){
	
	Node *root=NULL;
	char input;
	int dataToProcess;
	addFromInputFile(&root);
	printf("----Welcome to Ternary Search Tree Program----\n\n");
	do{
		printf("\n1-Add a Node\n2-Delete a Node\n3-Search a Node\n4-Print(LMRN)\n5-Quit\n");
		scanf(" %c",&input);
		switch(input){
			case '1':{
				printf("\n Enter a number to add: ");
				scanf("%d",&dataToProcess);
				insert(&root,dataToProcess);
				break;
			}
			case '2':{
				printf("\n Enter a number to delete: ");
				scanf("%d",&dataToProcess);
				root = removeNode(root,dataToProcess);
				Node **bb;
				bb=NULL;
				findBrokenRoot(&root,&bb);
				if(bb!=NULL){
					rebuildTheBrokenTree(&(*bb),&root,&(*bb));
				}
				break;
			}
			case '3':{
				printf("\n Enter a number to search: ");
				scanf("%d",&dataToProcess);
				Node *tempSearchNode = search(root,dataToProcess);
				if(tempSearchNode!=NULL)
					printf("\nNode is found : %d\n",tempSearchNode->data);
				else
					printf("\nNot Found\n");
				break;
			}
			case '4':
				printTreeLMRN(root);
				break;
			case '5':
				break;
			default:
				printf("Wrong Input Please Try Again\n");
				break;
		}
	}while(input!='5');	
}
