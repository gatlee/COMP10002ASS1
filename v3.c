/* Extended precision integer calculator
 * Implements +, *, and ^ (power of) operations
 *
 * Skeleton code written by Jianzhong Qi, March 2018
 * Rest of code written by Gatlee Kaw(994017), April 2018
 * Last modified: 18/04/18
 *
 *TODO: fix [0] indexing for n 
 *fix ){ --> ) { formatting
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INT_SIZE	100	/* max number of digits per integer value */
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

typedef int digit_t;				/* a decimal digit */
typedef digit_t huge_t[INT_SIZE];	/* one huge int "variable" */

/* add your constant and type definitions here */
#define N_OP		'n'
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
void set_huge_to_zero(huge_t *target, int *target_len);
void set_huge_to_str(huge_t *huge, char *opr2_str, int *length);
void add_int_to_digit(huge_t *target, int *target_len, int target_index,
                      int value);
void add_huges(huge_t *target, int *target_len, huge_t *source, int source_len);
void trim_zeros(huge_t *target, int *target_len);
void copy_huge(huge_t *target, int *target_len, huge_t *source, int source_len);
void mult_huge(huge_t *target, int *target_len, huge_t *source, int source_len);
void power_huge(huge_t *target, int *target_len, huge_t *exp, int exp_len);
int huges_equal(huge_t *a, int a_len, huge_t *b, int b_len);
void inc_huge(huge_t *target, int *target_len);
/****************************************************************/
/* main function controls all the action, do NOT modify this function */
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
	for(i=0; i<NUM_VARS; i++){
		set_huge_to_zero(&vars[i], &lens[i]);
	}
}

/* process the '=' operator */
void
assign(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	if (opr2_str[0] == N_OP){
		int opr2_index = opr2_str[1] - CH_ZERO;
		set_huge_to_zero(&vars[opr1_index], &lens[opr1_index]);
		
		add_huges(&vars[opr1_index], &lens[opr1_index], &vars[opr2_index],
		          lens[opr2_index]);
	}
	else{
		set_huge_to_str(&vars[opr1_index], opr2_str, &lens[opr1_index]);
	}

}
	
/* process the '+' operator */
void 
add(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	if (opr2_str[0] == N_OP){
		int opr2_index = opr2_str[1] - CH_ZERO;
		
		add_huges(&vars[opr1_index], &lens[opr1_index], &vars[opr2_index],
		          lens[opr2_index]);
	}
	else{
		huge_t to_add;
		int huge_len; 
		set_huge_to_str(&to_add, opr2_str, &huge_len);
		
		add_huges(&vars[opr1_index], &lens[opr1_index], &to_add,
		          huge_len);
	
	}
}

/* process the '*' operator */
void 
multiply(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	if (opr2_str[0] == N_OP){
		int opr2_index = opr2_str[1] - CH_ZERO;
			
		mult_huge(&vars[opr1_index], &lens[opr1_index], &vars[opr2_index],
		          lens[opr2_index]);

	
	}
	else{
		huge_t to_add;
		int huge_len; 
		set_huge_to_str(&to_add, opr2_str, &huge_len);
		
		mult_huge(&vars[opr1_index], &lens[opr1_index], &to_add,
		          huge_len);
	}

	
}


/* process the '^' operator */
void 
power(huge_t vars[], int lens[], int opr1_index, char *opr2_str) {
	if (opr2_str[0] == N_OP){
		int opr2_index = opr2_str[1] - CH_ZERO;
			
		power_huge(&vars[opr1_index], &lens[opr1_index], &vars[opr2_index],
		          lens[opr2_index]);

	
	}
	else{
		huge_t exp;
		int exp_len; 
		set_huge_to_str(&exp, opr2_str, &exp_len);
		
		power_huge(&vars[opr1_index], &lens[opr1_index], &exp, exp_len);
	}

	
}

/*Function: set_huge_to_zero
*---------------------------
*sets a huge to zero and its length to one
*
*   target: pointer of huge_t array
*   target_len: pointer to length of target
*/
void set_huge_to_zero(huge_t *target, int *target_len){
	*target_len = 1;
		
	(*target)[0]=0;
}

/*
*Function: set_huge_to_str
*-------------------------
*set huge to given str
*
*   huge: pointer of to huge_t array
*   opr2_str: user input string for characters proceeding and including first
*   operator
*	length: pointer to length of huge_t 
*   TODO: change name
*/
void
set_huge_to_str(huge_t *huge, char *opr2_str, int *length){
	int i=0;
	while (opr2_str[i] !=0) {
		i++;
	}
	*length = i;
	int j = 0;
	for (j=(*length) - 1; j>=0; j--) {
		(*huge)[j] = (opr2_str[((*length)-1) -j] - CH_ZERO);
	}
	
}
/*
*Function: add_huges
*-------------------
*adds huges together using repeated calls of add_int_to_digit for each
*digit of the corresponding target and source digits
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*   source: pointer of huge_t array to be added to target
*   source_len: length of source array
*   
*   TODO: n0=999, n0^n0 = incorrect value
*/
void
add_huges(huge_t *target, int *target_len, huge_t *source, int source_len) {
	int i;


	for(i = 0; i<source_len; i++){
		add_int_to_digit(target, target_len, i, (*source)[i]);
	}
	trim_zeros(target, target_len);
}

/*
*Function: add_int_to_digit 
*--------------------------
*adds a single value to a huge at target_index's value. Handles carrying
*but not leading zeros
*
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*   target_index: index of digit in huge to add to 
*   value: value to add to digit
*/

void
add_int_to_digit(huge_t *target, int *target_len, int target_index, int value){
	if (target_index >= INT_SIZE){
		return;
	}
	
	/*expands length if required. sets new digit to 0*/
	if (*target_len <= target_index){
		*target_len += 1;
		(*target)[target_index] = 0; 	
	}

	if (value == 0){
		return;
	}
	
	/*calculates value of current digit and the carry*/	
	int sum = (*target)[target_index] + value;
	(*target)[target_index]= sum%10; 
	int carry = sum/10;

	/*adds the carry to next digit*/
	add_int_to_digit(target, target_len, target_index+1, carry);
	/*Recursion! Algorithms are fun!*/
	
}

/*
*Function: trim_zeros
*Shortens length of huge to omit leading zeros
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*/
void trim_zeros(huge_t *target, int *target_len){
	int i =(*target_len)-1;
	while ((*target)[i] == 0 && i > 0){
		i--;
	}
	(*target_len) = i + 1;
}	


/*
*Function huges_equal
*-------------------
*checks if huge a and b are equal
*   a: pointer of huge_t
*	a_len: length of a	
*   b: pointer of huge_t
*	b_len: length of b	
*
* returns: either 1 or 0 if true or not
*/

int huges_equal(huge_t *a, int a_len, huge_t *b, int b_len) {
	if(a_len != b_len) {
		return 0;
	}
	
	int i;
	for(i=0;i<a_len;i++) {
		if ((*a)[i] != (*b)[i]) {
			return 0;
		}
	}
	return 1;
}



/*
*Function: copy_huge
*-------------------
*sets target huge to value of source huge
*
*   target: huge_t array to be changed
*	target_len: pointer to length of huge_t 
*   source: pointer of huge_t array to be added to target
*   source_len: length of source array
*	
*/
void copy_huge(huge_t *target, int *target_len, huge_t *source, 
               int source_len) {

	(*target_len) = source_len;

	int i;
	for (i=0;i<source_len;i++){
		(*target)[i] = (*source)[i];
	}
}

/*
*Function: mult_huge
*-------------------
*changes mult to mult x source with correct formatting 
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*   source: pointer of huge_t array to be added to target
*   source_len: length of source array
*   
*/
void mult_huge(huge_t *target, int *target_len, huge_t *source, 
               int source_len) {

	huge_t total;
	int total_len=0;
	set_huge_to_zero(&total, &total_len);
	
	/*uses school multiplication algorithm*/
	int i, j;
	for (i=0; i<*target_len;i++){
		for (j=0; j<source_len;j++){
			
			int product = (*target)[i] * (*source)[j];
			add_int_to_digit(&total, &total_len, i+j, product);
		}
	}
	/*finally assigns target value and removes leading zeros*/	
	copy_huge(target, target_len, &total, total_len);
	trim_zeros(target, target_len);
}

/*
*Function: power_huge
*-------------------
*sets target to target^exp
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*   exp: pointer of huge_t array to be added to target
*   exp_len: length of source array
*   
*/

void power_huge(huge_t *target, int *target_len, huge_t *exp, int exp_len) {
	/*declares huge interators and sets to correct value*/	
	huge_t huge_i; 
	int huge_i_len=0;
	set_huge_to_zero(&huge_i, &huge_i_len);
	
	/*increases by one because x^2 requires 1 multiplication not 2*/
	inc_huge(&huge_i, &huge_i_len);	


	/*initialises copy of target to repeatedly multiply to target*/
	huge_t base;
	int base_len = *target_len; 
	copy_huge(&base, &base_len, target, *target_len);	

	/*repeatedly multiplies target by base until huges equal evals to true*/
	while(!(huges_equal(&huge_i, huge_i_len, exp, exp_len))){
		mult_huge(target, target_len, &base, base_len);

		inc_huge(&huge_i, &huge_i_len);
	}
}


/*
*Function: inc_huge
*-------------------
*adds 1 to huge
*
*   target: pointer of huge_t array
*	target_len: pointer to length of huge_t 
*   
*/
	
void inc_huge(huge_t *target, int *target_len){
	
	add_int_to_digit(target, target_len, 0,1);
	trim_zeros(target, target_len); 

}
