/* This program is written by Fei Bao (804522) and designed for getting the 
	path planning such as the item "pickers" (which may be a robotic advices)
	in Amazon's warehouse follow the designed routies to get items from bins
	in the minimize time.
*/
/* Programming is fun! */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define DIV "-------" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define MAX_ITEM 10
#define MAX_CUSTOMER 100
#define FIRST_VALUE_IN_COMMANDLINE 1
#define SECOND_VALUE_IN_COMMANDLINE 2

typedef struct{
	int item_n;
	int bin_col;
	char bin_row;
}item_t;

typedef struct{
	int customer;
	int nitem;
	item_t items[MAX_ITEM];
}customer_t;

	

void print_stage_heading(int stage);
void store_data(customer_t customers[], int *tot_orders, int *tot_items);
void item_swap(item_t *a, item_t *b);
void print_orders(customer_t customers[], int tot_orders);
void sort_order(customer_t customers[], int i);
double compute_distance(customer_t customers[], int i, 
	double rows, double columns);
void add_one_array(customer_t customers[], customer_t pickers[], 
	int i, int j);
void add_two_arrays(customer_t customers[], customer_t pickers[],
	int i, int n, int j);

void stage_one(customer_t customers[], int tot_orders, int tot_items);
void stage_two(customer_t customers[], int tot_orders, int tot_items);
void stage_three(customer_t customers[], int tot_orders, 
	int rows, int columns);
void stage_four(customer_t customers[], int tot_orders, 
	int rows, int columns);


/* main program binds it all together */
int
main(int argc, char** argv){
	int tot_orders=0, tot_items=0;
	int  rows, columns;
	/* define an array which containing the information of orders of customers
	    and store the data in this array */
	customer_t customers[MAX_CUSTOMER];
	/* scan the number of bin in one column and the number of corridor */
	rows = atoi(argv[FIRST_VALUE_IN_COMMANDLINE]);
	columns = atoi(argv[SECOND_VALUE_IN_COMMANDLINE]);
	/* store the date from input file into "customers[]" array */
	store_data(customers, &tot_orders, &tot_items);
	/* stage 1 */
	stage_one(customers, tot_orders, tot_items);
	/* stage 2 */
	stage_two(customers, tot_orders, tot_items);
	/* stage 3 */
	stage_three(customers, tot_orders, rows, columns);
	/* stage 4 */
	stage_four(customers, tot_orders, rows, columns);
	/* all done! */
	return 0;
}


/* print out a stage heading, with a prior blank line once the stage number
   goes plus one 
*/
void 
print_stage_heading(int stage) {
	/* main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE) {
		/* separate the stages */
		printf("\n");
	}
	printf("Stage %d\n%s\n", stage, DIV);
}


/* compute the number of orders and items and
   show the orders in full of the first and last customers 
*/
void
stage_one(customer_t customers[], int tot_orders, int tot_items){
	/* print heading */
	print_stage_heading(STAGE_NUM_ONE);
	
	printf("  orders: %3d\n", tot_orders);
	printf("  items : %3d\n", tot_items);
	print_orders(customers, tot_orders);
}


/* sorting each costomer's items in "picking" order
*/
void
stage_two(customer_t customers[], int tot_orders, int tot_items){
	/* print heading */
	print_stage_heading(STAGE_NUM_TWO);
	
	int i;
	for(i=0;i<tot_orders;i++){
		sort_order(customers, i);	
	}
	print_orders(customers, tot_orders);
}


/* calculating pick times and average distance per order
*/
void
stage_three(customer_t customers[], int tot_orders, int rows, int columns){
	/* print heading */
	print_stage_heading(STAGE_NUM_THREE);
	int i;
	double distance = 0.0, tot_distance=0.0;
	printf("  warehouse has %1d rows and %1d columns\n", rows, columns);
	/* compute the distance for each customer */
	for(i=0;i<tot_orders;i++){
		distance = compute_distance(customers, i, rows, columns);
		tot_distance+=distance;
		if(i==0 || i==tot_orders-1){
			printf("  customer %d,  %d items, pick distance: %.1f metres\n",
				customers[i].customer, customers[i].nitem, distance);
		}
		distance = 0.0;
	}
	printf("  average distance per order over  %d orders: %.1f metres\n",
		tot_orders, tot_distance/tot_orders);
}


/* compute pickers required and average distance per order picker need to walk
*/
void
stage_four(customer_t customers[], int tot_orders, int rows, int columns){
	/* print heading */
	print_stage_heading(STAGE_NUM_FOUR);
	
	int i=1, j=0, npicker=0;
	double tot_pickers_distance=0.0;
	customer_t pickers[MAX_CUSTOMER];
	/* transfer the items of customer into items of picker */
	while(i<tot_orders){
		/* when there is only one customer */
		if(customers[i].customer==0){
			add_one_array(customers, pickers, i-1, j);
			break;
		}
		/* when the picker cannot hold more than 10 items
			for the last two customers */
		if(i==tot_orders-1 && customers[i].nitem+customers[i-1].nitem>10){
			add_one_array(customers, pickers, i-1, j);
			add_one_array(customers, pickers, i, j+1);
			break;
		}
		/* when the picker can only hold items 
		  for the last second customer and the last third customer*/
		if(i==tot_orders-2 &&  customers[i].nitem+customers[i-1].nitem<=10){
			add_two_arrays(customers, pickers, i-1, i, j);
			add_one_array(customers, pickers, i+1, j+1);
			break;
		}
		/* check whether picker can hold items for two customers */
		if(customers[i].nitem+customers[i-1].nitem<=10){
			add_two_arrays(customers, pickers, i-1, i, j);
			j++;
			i+=2;
		}else{
			add_one_array(customers, pickers, i-1, j);
			j++;
			i++;
		}
	}
	/* sort each piker's items and compute the distance each picker need */
	for(i=0;pickers[i].nitem!=0;i++){
		npicker++;
		sort_order(pickers, i);
		tot_pickers_distance+=compute_distance(pickers, i, rows, columns);
	}
	printf("  pickers required: %2d\n", npicker);
	printf("  average distance per order over %2d orders: %.1f metres\n", 
		tot_orders, tot_pickers_distance/tot_orders);	
}


/* store the data about the information of orders of customers into array
  and compute the number of total orders and total items
*/
void
store_data(customer_t customers[], int *tot_orders, int *tot_items){
	int i, j;
	int count_item=0;
	for(i=0;scanf("%d %d", &customers[i].customer, &customers[i].nitem)
					==2;i++){
		*tot_orders=i+1;
		for(j=0;j<customers[i].nitem;j++){
			scanf("%d %d%c", &customers[i].items[j].item_n, 
				&customers[i].items[j].bin_col, &customers[i].items[j].bin_row);
			count_item++;
		}
	}
	*tot_items=count_item;
}


/* exchange the values of the two variables in the arguments */
void
item_swap(item_t *a, item_t *b){
	item_t tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}


/* show the bins of items that the first and last customers bought*/
void
print_orders(customer_t customers[], int tot_orders){
	int i, j;
	for(i=0;i<tot_orders;i++){
		if(i==0 || i==tot_orders-1){
			printf("  customer %d, ", customers[i].customer);
			printf("%2d items, bins: ", customers[i].nitem);
			for(j=0;j<customers[i].nitem;j++){
				printf("%2d", customers[i].items[j].bin_col);
				printf("%c ", customers[i].items[j].bin_row);
				if(j==customers[i].nitem-1){
					printf("\n");
				}
			}
		}
	}
}


/* sort the order of items for each person */
void
sort_order(customer_t customers[], int i){
	int j, k;
	char *a, *b;
	/* sorting items of each customer according to the row of bin */
	for(j=1;j<customers[i].nitem;j++){
		for(k=j-1;k>=0;k--){
			a=&customers[i].items[k+1].bin_row;
			b=&customers[i].items[k].bin_row;
			if(strcmp(a, b)<0){
				item_swap(&customers[i].items[k], &customers[i].items[k+1]);
			}
		}
	}
	/* sorting items of each customer according to the column of bin, which
		basing on the previous sorting condition */
	for(j=1;j<customers[i].nitem;j++){
		for(k=j-1;k>=0;k--){
			if(customers[i].items[j].bin_row==customers[i].items[k].bin_row){
				/* the bins is visited in increasing numeric order 
					when in the odd-lettered corridors "a" and "c" and so on */
				if(toascii(customers[i].items[k].bin_row)%2!=0 && 
				    customers[i].items[k+1].bin_col < customers[i].items[k].bin_col){
					item_swap(&customers[i].items[k], &customers[i].items[k+1]);
				}
				/* the bins is visited in decreasing numeric order 
					 when in the even-lettered corridors "a" and "c" and so on */
				if(toascii(customers[i].items[k].bin_row)%2==0 && 
				customers[i].items[k+1].bin_col > customers[i].items[k].bin_col){
					item_swap(&customers[i].items[k], &customers[i].items[k+1]);
				}
			}else{
				break;
			}
		}
	}
}


/* compute the shortest distance for the person holding all items */
double
compute_distance(customer_t customers[], int i, double rows, double columns){
	int j;
	int vertical_travel_times=0;
	double distance;
	/* compute times of vertical travel */
	if((toascii(customers[i].items[0].bin_row)%2)==0 || 
		(toascii(customers[i].items[customers[i].nitem-1].bin_row)%2)!=0){
		vertical_travel_times++;
	}
	for(j=1;j<customers[i].nitem;j++){
		if(customers[i].items[j].bin_row!=customers[i].items[j-1].bin_row){
			vertical_travel_times++;
			if((toascii(customers[i].items[j-1].bin_row)%2)!=0 && 
				(toascii(customers[i].items[j].bin_row))%2!=0){
				vertical_travel_times++;
			}
			if((toascii(customers[i].items[j-1].bin_row)%2)==0 && 
				(toascii(customers[i].items[j].bin_row)%2)==0){
				vertical_travel_times++;
			}
		}
	}
	distance = ((columns-1)*6.4)+((1+(vertical_travel_times+1)*(rows+1))*3.8);
	return distance;
}


/* add one customer's items into the picker's items */
void
add_one_array(customer_t customers[], customer_t pickers[], int i, int j){
	int k;
	pickers[j].nitem = customers[i].nitem;
	for(k=0;k<customers[i].nitem;k++){
		pickers[j].items[k].bin_col = customers[i].items[k].bin_col;
		pickers[j].items[k].bin_row = customers[i].items[k].bin_row;
	}
}


/* add two customers' items into the same picker's items */
void
add_two_arrays(customer_t customers[], customer_t pickers[], int i, int n, int j){
	int k;
	pickers[j].nitem = customers[i].nitem+customers[n].nitem;
	for(k=0;k<customers[i].nitem+customers[n].nitem;k++){
		if(k<customers[i].nitem){
			pickers[j].items[k].bin_col = customers[i].items[k].bin_col;
			pickers[j].items[k].bin_row = customers[i].items[k].bin_row;
		}else{
			pickers[j].items[k].bin_col = 
			customers[n].items[k-customers[i].nitem].bin_col;
			pickers[j].items[k].bin_row = 
			customers[n].items[k-customers[i].nitem].bin_row;
		}
	}
}