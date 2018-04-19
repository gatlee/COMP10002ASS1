/* Extended precision integer calculator
 * Implements +, *, and ^ (power of) operations
 *
 * Skeleton code written by Jianzhong Qi, March 2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INT_SIZE	100/* max number of digits per integer value */ 
#define LINE_LEN	103	/* maximum length of any input line */ 
#define NUM_VARS	10	/* number of different huge int "variables" */ 
#define ASN_OP		'='	/* assignment operator */
#define ECH_OP		'?'	/* echo operator */
#define ADD_OP		'+'	/* addition operator */
#define MUL_OP		'*'	/* multiplication operator */
#define POW_OP		'^'	/* power of operator */

#define OPR1_POS	1	/* position of the first operand */
#define OPR2_POS	3	/* position of the second operand */
#define OP_POS		2	/* position of the operator */

#define CH_ZERO		'0'	/* character 0 */

#define EXIT_CMD 	"exit"	/* command to exit */
#define PROMPT		"> "	/* command prompt */
#define CMT_FLAG	'%'		/* indicator for comment line */

typedef int digit_t;			/* a decimal digit */
typedef digit_t huge_t[INT_SIZE];	/* one huge int "variable" */

/* add your constant and type definitions here */


/****************************************************************/ 
/* function prototypes */ 
void read_line(char *line, int max_len); 
void init(huge_t vars[], int lens[]); 
void echo(huge_t vars[], int lens[], int opr1_index); 
void assign(huge_t vars[], int lens[], int opr1_index, char *opr2_str); 
void add(huge_t vars[], int lens[], int opr1_index, char *opr2_str); 
void multiply(huge_t vars[], int lens[], int opr1_index, char *opr2_str); 
void power(huge_t vars[], int lens[], int opr1_index, char *opr2_str); 
/* add your function prototypes here */ 
int get_bigger(int, int); 
int get_opr2_len(char *opr2_str); 
void set_huge_to_str(huge_t *huge ,char *opr2_str);
void add_return_carry(digit_t *digit, int factor, int *carry);
void trim_huge(huge_t huge, int len);
/****************************************************************/ 

int
main(int argc, char *argv[]) {
	char line[LINE_LEN+1];		/* to hold the input line */
	huge_t vars[NUM_VARS];		/* to hold 10 huge integers */
	int lens[NUM_VARS];			/* to hold the length of the 10 vars */
	
	int opr1_index;				/* index of the first operand in command */
	char op;					/* operator in command */
	
	init(vars, lens);
	
	while (1) {
		printf(PROMPT);						/* print prompt */
		read_line(line, LINE_LEN);			/* read one line of command */

		if (line[0] == CMT_FLAG) {			/* print comment in the test data */ 
			printf("%s\n", line);			/* used to simplify marking */
			continue;
		}
		
		if (strcmp(line, EXIT_CMD) == 0) {	/* see if command is "exit" */
			return 0;
		} 
		
		opr1_index = line[OPR1_POS] - CH_ZERO;/* first var number at line[1] */
		op = line[OP_POS];					/* operator at line[2] */

		if (op == ECH_OP) {					/* print out the variable */
			echo(vars, lens, opr1_index);
			continue;
		} 
		
		/* do the calculation, second operand starts at line[3] */
		if (op == ASN_OP) {	
			assign(vars, lens, opr1_index, line+OPR2_POS);
		} else if (op == ADD_OP) {
			add(vars, lens, opr1_index, line+OPR2_POS);
		} else if (op == MUL_OP) {
			multiply(vars, lens, opr1_index, line+OPR2_POS);
		} else if (op == POW_OP) {
			power(vars, lens, opr1_index, line+OPR2_POS);
		}
	}
	
	/* all done; take some rest */
	return 0;
}
/* read a line of input into the array passed as argument */
void
read_line(char *line, int max_len) {
	int i = 0, c;
	while (((c = getchar()) != EOF) && (c != '\n') && (c != '\r')) {
		if (i < max_len) {
			line[i++] = c;
		} else {
			printf("Invalid input line, toooooooo long.\n");
			exit(0);
		}
	}
	line[i] = '\0';
}

/* print out a huge integer */
void echo(huge_t vars[], int lens[], int opr1_index) {
	int i;
	
	/* print the digits in a reverse order */
	for (i = lens[opr1_index]-1; i >= 0; i--) {
		printf("%d", vars[opr1_index][i]);
	}
	
	printf("\n");
}


/****************************************************************/

/* add code below to complete the function bodies,
 * add more functions where appropriate.
 */

/* set the vars array to zeros */
void
init(huge_t vars[], int lens[]) {
	int i;
	for (i=0; i<NUM_VARS; i++){
		lens[i] = 1;
		vars[i][0] = 0;
	}
}

/* process the '=' operator */
void
assign(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	if (opr2_str[0] == 'n'){
		int i;
		int opr2_index = opr2_str[1] - CH_ZERO;
		lens[opr1_index] = lens[opr2_index];
		
		for (i = lens[opr1_index]-1; i >= 0; i--) {
			
			vars[opr1_index][i] =  vars[opr2_index][i];
		}
	} else if(opr2_str[0] >= '0' && opr2_str[0] <= '9'){	
		lens[opr1_index] = get_opr2_len(opr2_str);
		set_huge_to_str(&vars[opr1_index], opr2_str);
		/*
		int i;			
		for (i = lens[opr1_index]-1; i >= 0; i--) {
			vars[opr1_index][i] = opr2_str[(lens[opr1_index]-1)-i] - CH_ZERO;
		*/
	} else {
		;/*Algorithms are fun!*/
	}
}

/* process the '+' operator */
void 
add(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	int *left_val = vars[opr1_index];
	int left_len = lens[opr1_index];
	int *right_val;
	int right_len;
	huge_t temp;
	if(opr2_str[0] == 'n'){
		right_val = vars[opr2_str[1] - CH_ZERO];
		right_len = lens[opr2_str[1] - CH_ZERO];
	}
	else {
		right_len = get_opr2_len(opr2_str);
		set_huge_to_str(&temp, opr2_str);
		printf("temp = %d", temp[0]);
		right_val = temp;

	}
	printf("l:%d, r:%d", left_len, right_len);
	int i;
	int left_dig = 0, right_dig =0, carry = 0, sum_dig= 0;
	int calc_limit = get_bigger(left_len, right_len);
	for(i = 0; i<calc_limit+1 && i<INT_SIZE;i++){  /*INT_SIZE to prevent overflow*/
		if (i < left_len){
			left_dig = left_val[i];
		} else if (i!=calc_limit || carry == 1){
			left_dig = 0;
			lens[opr1_index] += 1;
			printf("incrementing length of left");
		} else{
			printf("do nothing as expected");
		}
		if (i < right_len){
			right_dig = right_val[i];
		} else {
			right_dig = 0;
		}
		sum_dig = left_dig + right_dig + carry;
		printf("summing %d and %d and %d (c) to get %d\n", 
			   left_dig, right_dig, carry, sum_dig);
		left_val[i] = sum_dig % 10;
		if (sum_dig >= 10){
			carry = 1;
		} else {
			carry = 0;
		}

		
	
	}
}

/* process the '*' operator */
void 
multiply(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {

}

/* process the '^' operator */
void 
power(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {

}

int get_bigger(int a, int b){
	if (a >= b){
		return a;
	}
	else {
		return b;
	}

}
int get_opr2_len(char *opr2_str){
	int i =0;
	while (opr2_str[i] != 0){ 
		i++;
	}
	return i;
}

void set_huge_to_str(huge_t *huge, char *opr2_str){
	int length = get_opr2_len(opr2_str);
	int i;
	for (i = length-1; i >= 0; i--) {
		printf("%d", i);
		(*huge)[i] = (opr2_str[(length-1)-i] - CH_ZERO);
	}
}

/*
function add_return_carry
takes in memory address of digit t and adds factor to it
sets location of carry to carry
*/
void add_return_carry(digit_t *digit, int factor, int *carry){
	int sum = *digit + factor;
	*digit = sum%10;
	*carry = sum/10;
}

void trim_huge(huge_t huge, int len){
	printf("trim boi");
}
