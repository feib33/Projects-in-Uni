/* This program is written by Fei Bao (804522) and designed for 
	named-entity recognition(NER) which is label the words in text 
	that refer to real-world objects with proper names. 
*/
/* Algorithms is fun!
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define DIV "=========================" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define MAX_UNIQUE_NAMES 100
#define MAX_LETTERS_IN_NAME 30
#define MAX_WORDS 50
#define FIRST_NAME 0

typedef struct node node_t;

struct node{
	char word[MAX_LETTERS_IN_NAME];
	node_t *next;
};

typedef struct{
	char name[MAX_LETTERS_IN_NAME];
	int f_name;
	int l_name;
	int n_name; 
}name_t;

node_t *head;
void  print_stage_heading(int stage);
void store_data(name_t names[], int *nword_n, double *tot_char);
int getword(char word[], int limit);
node_t * creat_head();
void Print(node_t *head);
int bineary_resarch(name_t names[], char word[], int end);
int check_equal(char a[], char b[]);

void stage_one(name_t names[]);
void stage_two(int nword_n, double tot_char);
void stage_three();
void stage_four(name_t names[], int nword_n);
void stage_five(name_t names[], int nword_n);

                            


/* main program binds it all together */
int
main(int argc, char** argv){
	int nword_n = 0;
	double tot_char = 0.0;
	/* define an array which containing the name 
		and its probabilities as first name, last name and non-name */
	name_t names[MAX_UNIQUE_NAMES];
	store_data(names, &nword_n, &tot_char);


	/* stage 1 */
	stage_one(names);
	/* stage 2 */
	stage_two(nword_n, tot_char);
	/*stage 3 */
	stage_three();
	/*stage 4 */
	stage_four(names, nword_n);
	/*stage 5 */
	stage_five(names,nword_n);
	/* all done! */
	return 0;
}

/* function from COMP10002 Semester 1 2017 Assignment 1
	skeleton code by Jianzhong Qi
*/
/* print out a stage heading, with a prior blank line once the 
	stage number goes plus one 
*/
void 
print_stage_heading(int stage) {
	/* main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE) {
		/* separate the stages */
		printf("\n");
	}
	printf("%sStage %d%s\n", DIV, stage, DIV);
}
/* print out the first name and its probabilities of input file
*/
void
stage_one(name_t names[]){
	/* print heading */
	print_stage_heading(STAGE_NUM_ONE);	
	printf("Word 0: ");
	printf("%s", names[FIRST_NAME].name);
	printf("Label probabilities: %d%% %d%% %d%%\n",names[FIRST_NAME].f_name, 
													names[FIRST_NAME].l_name,
													names[FIRST_NAME].n_name);
}
/* print out the total number of names and the average number 
	of characters per name
*/
void
stage_two(int nword_n, double tot_char){
	/* print heading */
	print_stage_heading(STAGE_NUM_TWO);	
	printf("Number of words: %d\n", nword_n);
	printf("Average number of characters per word: %.2f\n", 
											tot_char/nword_n);
}
/* read in and store words of given sentence of input file into a linked list
	and print out every words.
*/
void
stage_three(){
	/* print heading */
	print_stage_heading(STAGE_NUM_THREE);

	head = creat_head();
	Print(head);
}
/* label the names where in the input sentence
	according to the probabilities in the dictionary
*/
void
stage_four(name_t names[], int nword_n){
	/* print heading */
	int n;

	print_stage_heading(STAGE_NUM_FOUR);
	node_t *tmp = head;
	while(tmp!=NULL){
		printf("%-32s", tmp->word);
		n = bineary_resarch(names, tmp->word, nword_n-1);
		if(n==-1){
			printf("NOT_NAME\n");
		}else{
			if(names[n].f_name>0 && names[n].l_name>0){
				printf("FIRST_NAME, LAST_NAME\n");
			}else{
				if(names[n].f_name>0){
					printf("FIRST_NAME\n");
				}
				if(names[n].l_name>0){
					printf("LAST_NAME\n");
				}
			}
		}
		tmp = tmp->next;
	}
}
/* refine the word lables of the given sentence
	based on the probabilities given in the dictionary and some common sense */
void
stage_five(name_t names[], int nword_n){
	/* print heading */
	int n;
	node_t *tmp = head;
	print_stage_heading(STAGE_NUM_FIVE);
	while(tmp!=NULL){
		printf("%-32s", tmp->word);
		n = bineary_resarch(names, tmp->word, nword_n-1);
		if(n==-1){
			printf("NOT_NAME\n");
		}else{
			/* if the probability of non_name is over than 50% or 
				length of name is less than or equal to 2, it 
				would prefer to be not a name*/
			if(names[n].n_name>50 || strlen(names[n].name)<=2){
				printf("NOT_NAME\n");
			}else{
				/* if the probabilities of first name and last name are equivalent, 
					or the word is the first word in one sentence
					this word is a first name mostly */
				if(names[n].f_name>0 && names[n].l_name>0){
					if(names[n].f_name>=names[n].l_name || head->word){
						printf("FIRST_NAME\n");
					}else{
						printf("LAST_NAME\n");
					}
				}else{
					if(names[n].f_name>0){
						printf("FIRST_NAME\n");
					}
					if(names[n].l_name>0){
						printf("LAST_NAME\n");
					}
				}
			}
		}	
		tmp = tmp->next;
	}

}
/* store the data of input file into names[] array
*/
void
store_data(name_t names[], int *nword_n, double *tot_char){
	int i, j=0;
	char c;
	for(i=0;i<MAX_UNIQUE_NAMES;i++){
		if((c=getchar())=='#'){
			j=0;
			while((names[i].name[j]= getchar())!='\n'){
				j++;
				*tot_char+=1.0;
			}
			scanf("%d %d %d", &names[i].f_name, 
								&names[i].l_name, &names[i].n_name);
			*nword_n+=1;
		}
		/* store all the data where are over the %%%%%%%% */
		if((c=getchar())=='%'){
			break;
		}
	}
}
/* function from the book "Programming, Problem Solving, 
	and Abstraction with C" code by Alistair Moffat
*/
/* break one sentences into several words and store each those words
*/
int
getword(char word[], int limit){
	int c, len=0;
	while((c=getchar())!=EOF && !isalpha(c)){}
	if(c==EOF){
		return EOF;
	}
	word[len++] = c;
	while(len<limit && (c=getchar())!=EOF && isalpha(c)){
		word[len++]=c;
	}
	word[len]='\0';
	return 0;
}
/* creat a head node to save nodes as a linked list
*/
node_t *
creat_head(){
	node_t* head = NULL;
	node_t* foot, *new;
	int count = 0;
	char word[MAX_LETTERS_IN_NAME];
	/* store the address of first node */
	foot=new=(node_t*)malloc(sizeof(struct node));
	getword(word, MAX_LETTERS_IN_NAME);
	strcpy(new->word, word);
	/* append each node in the linked list */
	while(*new->word){
		count++;
		if(count==1){
			new->next=head;
			foot=head=new;
		}else{
			new->next=NULL;
			foot->next=new;
			foot=new;
		}
		new=(node_t*)malloc(sizeof(struct node));
		if(getword(word, MAX_LETTERS_IN_NAME)==EOF){
			break;
		}
		strcpy(new->word, word);
	}
	return head;
}

/* print all words of nodes
*/
void
Print(node_t *head){
	node_t *tmp;
	tmp=head;
	while(tmp!=NULL){
		printf("%s\n", tmp->word);
		tmp=tmp->next;
	}
}
/* search a word and check whether the word is in the dictionary(names[]) */
int
bineary_resarch(name_t names[], char word[], int end){
	int start = 0;
	int mid;
	if(check_equal(word, names[start].name)==0){
			return start;
	}
	if(check_equal(word, names[end].name)==0){
			return end;
	}
	while(start<end){
		mid = (start+end)/2;
		if(check_equal(word, names[mid].name)==0){
			return mid;
		}
		if(check_equal(word, names[mid].name)<0){
			end=mid;
		}
		if(check_equal(word, names[mid].name)>0){
			start = mid +1;
		}
		
	}
	/* this word is not in the dictionary */
	return -1;
}
/* compare whether two strings are equivalent*/
int
check_equal(char a[], char b[]){
	int i;
	int res;
	for(i=0;a[i]!='\0';i++){
		if(a[i]==b[i] ){
			res = 0;
		}
		if(a[i]<b[i]){
			res = -1;
			break;
		}
		if(a[i]>b[i]){
			res =  1;
			break;
		}
	}
	return res;
}