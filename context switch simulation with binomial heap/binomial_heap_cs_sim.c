#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*
Dependencies : input.txt file on same location.
*/
typedef struct node{
    int degree,original_execution_time,arrival_time,remaining;
    double value;
    int flag;
    char *pid;
    struct node *sibling,*child,*parent;
}node;

typedef struct heap{
    node *head;
}heap;

typedef struct process_list_node{
    char *pid;
    int arrival_time;
    double execution_time;
    int waiting_time;
    struct process_list_node *next;
}list_node;


list_node *new_node(char *pid,int execution_time,int arrival_time){
    list_node *temp=(list_node*)malloc(sizeof(list_node));
    temp->arrival_time=arrival_time;
    temp->execution_time=execution_time;
    temp->pid=strdup(pid);
    temp->waiting_time=0;
    temp->next=NULL;
    return temp;
};

void insert_to_process_list(list_node **head,char *pid,int execution_time,int arrival_time){
    list_node *temp = new_node(pid,execution_time,arrival_time);
    if(*head==NULL){
        *head=temp;
        return;
    }
    list_node *iterator = *head;
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next=temp;
}

void print_process_list(list_node *head){
    while(head!=NULL){
        printf("%s %lf %d %d\n",head->pid,head->execution_time,head->arrival_time,head->waiting_time);
        head=head->next;
    }
}

void read_text_then_insert_to_process_list(char *filename,list_node **head){//reading the text comes from traverse directory
    FILE *fp;
    char str[3][50];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("file is not found");
        return;
    }
    int cell_counter=0;
    char h[30];
    while ( !feof(fp)){

        fscanf(fp,"%s",str[cell_counter]);

        cell_counter++;
        if(cell_counter==3){
            insert_to_process_list(head,str[0],(double) strtol(str[1], (char **)NULL, 10),(int) strtol(str[2], (char **)NULL, 10) );
            cell_counter=0;
        }
    }
    fclose(fp);
}



node *node_init(double execution_time,int arrival_time,char *pid,int remaining){
    node *temp = (node*)malloc(sizeof(node));
    temp->value=execution_time;
    temp->flag=0;
    temp->remaining=remaining;
    temp->original_execution_time=execution_time;
    temp->arrival_time=arrival_time;
    temp->degree=0;
    temp->pid=strdup(pid);
    temp->child=temp->parent=temp->sibling=NULL;
    return temp;
}

heap *heap_init(){
    heap *heap = malloc( sizeof ( heap) );
    heap->head = NULL;
    return heap;
}

node *heap_merge(heap *h1,heap*h2){
    if(h1->head==NULL)
        return h2->head;
    if(h2->head==NULL)
        return h1->head;

    node *head,*tail;
    node *h1iterate = h1->head;
    node *h2iterate = h2->head;


    if(h1->head->degree <= h2->head->degree){
        head=h1->head;
        h1iterate=h1iterate->sibling;
    }else{
        head=h2->head;
        h2iterate=h2iterate->sibling;
    }

    tail=head;

    while( h1iterate != NULL && h2iterate != NULL )
    {
        if( h1iterate->degree <= h2iterate->degree ){
            tail->sibling = h1iterate;
            h1iterate = h1iterate->sibling;
        }
        else{
            tail->sibling = h2iterate;
            h2iterate = h2iterate->sibling;
        }

        tail = tail->sibling;
    }

    if (h1iterate != NULL) {
        tail->sibling = h1iterate;
    } else {
        tail->sibling = h2iterate;
    }

    return head;
}

node *heap_union(heap *h1,heap *h2){
    node *newHead = heap_merge(h1,h2);

    h1->head=NULL;
    h2->head=NULL;

    if(newHead==NULL){
        return NULL;
    }

    node *prev=NULL;
    node *current=newHead;
    node *next=newHead->sibling;

    while(next!=NULL){
        if(current->degree != next->degree || (next->sibling!=NULL && next->sibling->degree == current->degree)){
            prev=current;
            current=next;
        }else{
            if(current->value < next->value){
                current->sibling=next->sibling;
                next->parent=current;
                next->sibling=current->child;
                current->child=next;
                current->degree++;
            }else{
                if(current->value == next->value && current->arrival_time < next->arrival_time){
                    current->sibling=next->sibling;
                    next->parent=current;
                    next->sibling=current->child;
                    current->child=next;
                    current->degree++;
                }else{
                    if(prev==NULL){
                        newHead = next;
                    }else{
                        prev->sibling=next;
                    }

                    current->parent=next;
                    current->sibling=next->child;
                    next->child=current;
                    next->degree++;

                    current=next;
                }
            }
        }
        next=current->sibling;
    }
    return newHead;
}

void heap_remove(heap *heap2,node *min,node *min_prev){
    if(min == heap2->head)
        heap2->head=min->sibling;
    else
        min_prev->sibling = min->sibling;

    node *new_head = NULL;
    node *child = min->child;

    node *next;
    while(child!=NULL){
        next = child->sibling;
        child->sibling = new_head;
        child->parent = NULL;
        new_head = child;
        child = next;
    }

    heap *temp = heap_init();
    temp->head = new_head;
    heap2->head = heap_union( heap2, temp );
    free( temp );

}

node *extract_min(heap *heap){
    if(heap->head==NULL)
        return NULL;

    node *min=heap->head;
    node *min_prev = NULL;
    node *next = min->sibling;
    node *next_prev = min;

    while(next!=NULL){
        if(next->value < min->value){
            min=next;
            min_prev=next_prev;
        }else if(next->value == min->value && min->arrival_time > next->arrival_time){
            min=next;
            min_prev=next_prev;
        }
        next_prev=next;
        next=next->sibling;
    }

    heap_remove(heap,min,min_prev);
    return min;
}

node* heap_insert(heap *H,double execution_time,int arrival_time,char *pid,int remaining){
    node *tempNode = node_init( execution_time,arrival_time,pid,remaining);
    heap *tempHeap = heap_init();

    tempHeap->head=tempNode;

    H->head=heap_union(H,tempHeap);
    free(tempHeap);
}


int heap_size(node *root){
    node *iterator = root;
    int size=0;
    while(iterator!=NULL){
        size+=pow(2,iterator->degree);
        iterator=iterator->sibling;
    }
    return size;
}

list_node *get_process_by_time(int time,list_node *head){
    while(head!=NULL){
        if(head->arrival_time==time){
            return head;
        }
        head=head->next;
    }
    return NULL;
}

int process_list_size(list_node *head){
    int counter=0;
    while(head!=NULL){
        counter++;
        head=head->next;
    }
    return counter;
}

int get_emax(list_node *head){
    int max = head->execution_time;
    head=head->next;
    while(head!=NULL){
        if(max < head->execution_time){
            max=head->execution_time;
        }
        head=head->next;
    }
    return max;
}

void print_heap(node *root){
    if(root==NULL){
        return;
    }

    print_heap(root->child);
    print_heap(root->sibling);
    printf("%s\n",root->pid);
}


void search_and_increment_waiting_time(char *pid,list_node *head){
    while(head!=NULL){
        if(!strcmp(pid,head->pid)){
            head->waiting_time+=1;
        }
        head=head->next;
    }
}

void update_waiting_times(node *root,node *node_not_waiting,list_node *head){
    if(root==NULL){
        return;
    }

    update_waiting_times(root->child,node_not_waiting,head);
    update_waiting_times(root->sibling,node_not_waiting,head);
    if(strcmp(root->pid,node_not_waiting->pid)){
        search_and_increment_waiting_time(root->pid,head);
    }
}


void start_to_process(list_node *head,heap *heap,int quantum){
    int time=0,new_flag=1,quantum_counter=0;
    int total_number_of_process=process_list_size(head);
    list_node *current_node;
    int process_history=0;
    int emax = get_emax(head);
    double exp_result,new_value,inner;
    node *current_process;
    while(1){
        update_waiting_times(heap->head,current_process,head);
        current_node=get_process_by_time(time,head);
        if(current_node!=NULL && heap_size(heap->head)!=process_list_size(head)){
            heap_insert(heap,current_node->execution_time,current_node->arrival_time,current_node->pid,current_node->execution_time);
        }
        time++;
        if(new_flag){
            current_process=extract_min(heap);
            if(current_process!=NULL){
                new_flag=0;
                quantum_counter=0;
            }
        }
        if(!new_flag){
            quantum_counter++;
            current_process->remaining=current_process->remaining- 1;
            if(quantum_counter==quantum){
                if(current_process->remaining==0){
                    new_flag=1;
                    process_history+=1;
                    quantum_counter=0;
                }else{
                    inner = -pow( (double)(2*current_process->remaining) /(double) (3 * emax),3);
                    new_value =( 1 / exp(inner)) * (double)current_process->remaining;
                    //printf("\n%lf",new_value);
                    heap_insert(heap,new_value,current_process->arrival_time,current_process->pid,current_process->remaining);
                    quantum_counter=0;
                    new_flag=1;
                }
            }else{
                if(current_process->remaining==0){
                    new_flag=1;
                    process_history+=1;
                    quantum_counter=0;
                }
            }

        }
        if(process_history==total_number_of_process){
            break;
        }
    }
}

double average_waiting_time(list_node *head){
    if(head==NULL){
        return 0;
    }
    int count=0,length=0;
    while(head!=NULL){
        count+=head->waiting_time;
        head=head->next;
        length++;
    }


    return (double)count / (double)length;

}

void make_zero_waitings(list_node *head){
    while(head!=NULL){
        head->waiting_time=0;
        head=head->next;
    }
}

void print_waiting_times(list_node *head){
    while(head!=NULL){
        printf("%s waited %d times\n",head->pid,head->waiting_time);
        head=head->next;
    }
}
int main() {

    list_node *head=NULL;
    read_text_then_insert_to_process_list("./input.txt",&head);

    heap *heap = heap_init();

    for(int i=1;i<11;i++){
        start_to_process(head,heap,i);
        print_waiting_times(head);
        printf("Waiting times of quantum : %d - %lf\n",i,average_waiting_time(head));
        make_zero_waitings(head);
    }

    return 0;
}