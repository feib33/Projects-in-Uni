/* This program is written by Fei BAO(ID:804522) and designed for reading and analysing 
    data about temperatures and finding the evidence of there is a general trend for 
    temperatures to be rising with time*/
/* by the way */
/* programming is fun!!! */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIV "-------" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4

#define MAX_DAYS 50000

void stage_one(int days[], int mons[], int years[], double mins[], 
	double maxs[], int tot_days);
void stage_two(int days[], int mons[], int years[], double mins[], 
	double maxs[], int tot_days);
void stage_three(int mons[], double mins[], double maxs[], int tot_days,
	double long_term_aver_min[], double long_term_aver_max[]);
void stage_four(int mons[], int years[], double mins[], double maxs[], int tot_days, 
	double long_term_aver_min[],  double long_term_aver_max[]);

void print_stage_heading(int stage);
void store_data(int locations[], int days[], int mons[], int years[], double mins[],
	double maxs[], int *tot_days);
void add_valid_day(double min, double max, double *tot_mins, double *tot_maxs, 
	int *min_valid_days, int *max_valid_days);
void return_aver(int month, int tot_days, int mons[], double mins[], double maxs[],
	int *min_valid_days, int *max_valid_days, double *tot_mins, double *tot_maxs);
void print_star(int min, int max);
double fround(double num);
/* main program binds it all together */
int
main(int argc, char* argv[]){
	/* storage for locations, dates, minimum temperatures and maximum temperatures  */
	int  locations[MAX_DAYS];
	int days[MAX_DAYS];
	int mons[MAX_DAYS];
	int years[MAX_DAYS];
	double mins[MAX_DAYS];
	double maxs[MAX_DAYS];
	/* define the array which containing long-term average temperature(min and max) */
	double long_term_aver_min[12];
	double long_term_aver_max[12];
	
	char c;
	int tot_days=0;
	/* skip the first line */
	while((c=getchar())!='\n');
	/* read in and store the data */
	store_data(locations, days, mons, years, mins, maxs, &tot_days);
	
	/* stage 1 */
	stage_one(days, mons, years, mins, maxs, tot_days);
	
	/* stage 2 */
	stage_two(days, mons, years, mins, maxs, tot_days);
	
	/* stage 3 */
	stage_three(mons, mins, maxs, tot_days, long_term_aver_min, long_term_aver_max);
	
	/* stage 4 */
	stage_four(mons, years, mins, maxs, tot_days, 
		long_term_aver_min,  long_term_aver_max);
	
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

/* stage 1 - print the number of input and the first and last of the input records
*/
void
stage_one(int days[], int mons[], int years[], double mins[], double maxs[], int tot_days){
	/* print stage heading for stage 1 */
	print_stage_heading(STAGE_NUM_ONE);
	
	printf("Input has %d records\n", tot_days);
	printf("First record in data file:\n");
	printf("  date: %.2d/%.2d/%d\n", days[0], mons[0], years[0]); 
	printf("  min : %4.1f degrees C\n", mins[0]);
	printf("  max : %.1f degrees C\n", maxs[0]);
	printf("Last record in data file:\n");
	printf("  date: %.2d/%.2d", days[tot_days-1], mons[tot_days-1]); 
	printf("/%d\n", years[tot_days-1]);
	printf("  min : %4.1f degrees C\n", mins[tot_days-1]);
	printf("  max : %.1f degrees C\n", maxs[tot_days-1]);
}

/* compute the average minimum temperature and the average maximum temperature
    of each year
*/
void
stage_two(int days[], int mons[], int years[], double mins[], double maxs[], int tot_days){
	/* print stage heading for stage 2 */
	print_stage_heading(STAGE_NUM_TWO);
	
	int i, basic_year, min_valid_days=0, max_valid_days=0; 
	double tot_mins=0.0, tot_maxs=0.0;
	basic_year = years[0];
	for(i=0;i<tot_days;i++){
		/* computer the avergae temperature of one year */
		if(years[i]==basic_year){
			add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, &min_valid_days,
				&max_valid_days);
			/* print the information about average temperature of last year */
			if(i==tot_days-1){
				printf("%d: average min: %5.2f ", years[i], tot_mins/min_valid_days);
				printf("degrees C (%d days)\n", min_valid_days);
				printf("      average max: %5.2f ", tot_maxs/max_valid_days);
				printf("degrees C (%d days)\n\n", max_valid_days);
			}
		}else{
			/* print the information about average temperature of  the year and reset
			    some variables to 0*/
			printf("%d: average min: %5.2f ", years[i-1], tot_mins/min_valid_days);
			printf("degrees C (%d days)\n", min_valid_days);
			printf("      average max: %5.2f ", tot_maxs/max_valid_days);
			printf("degrees C (%d days)\n\n", max_valid_days);
			basic_year = years[i];
			tot_mins=0;
			tot_maxs=0;
			min_valid_days=0;
			max_valid_days=0;
			add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, &min_valid_days,
				&max_valid_days);
		}
	}
}
/* visualisation each month of all years 
*/
void
stage_three(int mons[], double mins[], double maxs[], int tot_days, 
	double long_term_aver_min[], double long_term_aver_max[]){
	/* print stage heading for stage 3 */
	print_stage_heading(STAGE_NUM_THREE);
	/* compare the average temperatre(minimum and maximum) of each month with 
	long-term average temperature(minimum and maximum) and count them*/
	int i, min_valid_days=0, max_valid_days=0;
	double tot_mins=0.0, tot_maxs=0.0;
	char *mons_name[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
	"Sep", "Oct", "Nov", "Dec"};
	for(i=0;i<12;i++){
		return_aver(i+1, tot_days, mons, mins, maxs, &min_valid_days, 
				&max_valid_days, &tot_mins, &tot_maxs);
		/* storage for long-term average temperature */
		long_term_aver_min[i]=tot_mins/min_valid_days;
		long_term_aver_max[i]=tot_maxs/max_valid_days;
		printf("%s (%4d, %4d) |", mons_name[i], min_valid_days, max_valid_days);
		if(min_valid_days==0 && max_valid_days==0){
			print_star(min_valid_days, max_valid_days);
		}else{
			print_star(2.0*fround(tot_mins/min_valid_days),
					2.0*fround(tot_maxs/max_valid_days));
		}
		min_valid_days=0;
		max_valid_days=0;
		tot_mins=0;
		tot_maxs=0;	
	}
	printf("                 +---------+---------+---------+---------+---------+---------+\n");
	printf("                 0         5        10        15        20        25        30\n");
}

/* count the number of months in each year in which the average temperature for that month
    is greater than the long-term average temperature for the same month
*/
void
stage_four(int mons[], int years[], double mins[], double maxs[], int tot_days, 
	double long_term_aver_min[],  double long_term_aver_max[]){
	/* print stage heading for stage 4 */
	print_stage_heading(STAGE_NUM_FOUR);
	
	int i, basic_year, basic_mon, count=0, times=0, min_valid_days=0, max_valid_days=0;
	double tot_mins=0.0, tot_maxs=0.0, aver_min=0.0, aver_max=0.0;
	basic_year=years[0];
	basic_mon=mons[0];
	/* count the number of different years of input file */
	int year_long=0;
	year_long=years[tot_days-1]-years[0]+1;
	
	for(i=0;i<tot_days;i++){
		if(years[i]==basic_year){
			/* compute the average temperature(min and max) for one month */
			if(mons[i]!=basic_mon){
				aver_min=tot_mins/min_valid_days;
				aver_max=tot_maxs/max_valid_days;
				times++;
				if(aver_min>long_term_aver_min[mons[i]-2]){
					count++;
				}
				if(aver_max>long_term_aver_max[mons[i]-2]){
					count++;
				}
				tot_mins=0.0;
				tot_maxs=0.0;
				min_valid_days=0;
				max_valid_days=0;
				add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, 
					&min_valid_days, &max_valid_days);
				basic_mon=mons[i];
			}else{
				add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, 
					&min_valid_days, &max_valid_days);
			}
			/*  print out the score of the last year of input file */
			if(i==tot_days-1){
				add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, 
					&min_valid_days, &max_valid_days);
				aver_min=tot_mins/min_valid_days;
				aver_max=tot_maxs/max_valid_days;
				times++;
				if(aver_min>long_term_aver_min[mons[i]-1]){
					count++;
				}
				if(aver_max>long_term_aver_max[mons[i]-1]){
					count++;
				}
				if(times<6){
					times=times*2;
				}else{
					times=24;
				}
				printf("  %d: score is %2d/%2d\n", years[i-1], count, times);
			}
		}else if(years[i]!=basic_year){
			/* print out the score of last year and reset some variables to 0 */
			aver_min=tot_mins/min_valid_days;
			aver_max=tot_maxs/max_valid_days;
			times++;
			if(aver_min>long_term_aver_min[mons[i]+10]){
				count++;
			}
			if(aver_max>long_term_aver_max[mons[i]+10]){
				count++;
			}
			if(times<6){
				times=times*2;
			}else{
				times=24;
			}
			if(years[i-1]<(years[0]+5) || years[i-1]>(years[tot_days-1]-5)){
				printf("  %d: score is %2d/%2d\n", years[i-1], count, times);
				if(year_long>=10 && years[i-1]==years[0]+4){
					printf("  --\n");
				}
			}
			count=0;
			tot_mins=0.0;
			tot_maxs=0.0;
			min_valid_days=0;
			max_valid_days=0;
			add_valid_day(mins[i], maxs[i], &tot_mins, &tot_maxs, 
					&min_valid_days, &max_valid_days);
			basic_year=years[i];
			basic_mon=mons[i];
		}
	}
}
	

/* add the minimun temperatures and maximum temperatures together if it is valid(not equal 
    to -999) 
*/
void
add_valid_day(double min, double max, double *tot_mins, double *tot_maxs, 
	int *min_valid_days, int *max_valid_days){
	if(min!=-999){
		*tot_mins+=min;
		*min_valid_days+=1;
	}
	if(max!=-999){
		*tot_maxs+=max;
		*max_valid_days+=1;
	}
}

/* read in and store the data into array
*/			
void
store_data(int locations[], int days[], int mons[], int years[], double mins[], double maxs[],
	int *tot_days){
	int i=0, location, yy, mm, dd;
	double max, min;
	while(scanf("IDCJAC0010, %d,%d,%d,%d,%lf,%lf\n",
						&location, &yy, &mm, &dd, &max, &min)==6){
		locations[i]=location;
		days[i]=dd;
		mons[i]=mm;
		years[i]=yy;
		mins[i]=min;
		maxs[i]=max;
		*tot_days+=1;
		i++;
		}
}
/* compute valid days of minimum temperature and maximum temperature */
void
return_aver(int month, int tot_days, int mons[], double mins[], double maxs[],
	int *min_valid_days, int *max_valid_days, double *tot_mins, double *tot_maxs){
	int i;
	for(i=0;i<tot_days;i++){
		if(mons[i]==month){
			if(mins[i]!=-999){
				*min_valid_days+=1;
				*tot_mins+=mins[i];
			}
			if(maxs[i]!=-999){
				*max_valid_days+=1;
				*tot_maxs+=maxs[i];
			}
		}
	}
}
/* print star for stage 3 */
void
print_star(int min, int max){
	int i, j;
	if(min==0 && max==0){
	}else{
		for(i=0;i<min-1;i++){
			printf(" ");
		}
		for(j=0;j<(max-min+1);j++){
			printf("*");
		}
	}
	printf("\n");
}
/* find the closest integral num or integral+0.5 of num */
double
fround(double num){
	int int_num;
	double remainder;
	int_num=floor(num);
	remainder=num-int_num;
	if(remainder<0.25){
		num-=remainder;
	}else if(remainder>0.25 && remainder<0.5){
		num=int_num+0.5;
	}else if(remainder>0.5 && remainder<0.75){
		num=int_num+0.5;
	}else if(remainder>0.75){
		num+=(1.00-remainder);
	}
	return num;
}
		
		
		