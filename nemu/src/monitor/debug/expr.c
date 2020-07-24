#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  /* TODO: PA1.5 */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-", '-'},			// substract
  {"\\*", '*'},			// multiply
  {"/", '/'},			// divide
  {"\\d+", TK_NUM},		// number
  {"\\(", '('},			// left bracket
  {"\\)", ')'}			// right bracket
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

		/* TODO: PA1.5 */
		
		if(substr_len >= 32){
			printf("%.*s  The length of the substring is too long.\n", substr_len, substr_start);
			return false;
		}

		if(nr_token > 31) {
			printf("The count of tokens(nr_token) is out of the maximum count(32)");
			return false;
		}
        switch (rules[i].token_type) {
			case TK_NOTYPE:
				break;
			case TK_NUM:
				strncpy(tokens[nr_token].str, substr_start, substr_len);
				tokens[nr_token].str[substr_len] = '\0';
			default: 
				tokens[nr_token].type = rules[i].token_type;
				++ nr_token;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

/* TODO: PA1.5 */

/*  return 1  if the parentheses is a valid expression with left and right bracket.
 *  return 0  if the parentheses is a valid expression but without left and right bracket.
 *  return -1 if the parenteses isn't a valid expression. 
 */
int check_parentheses(int p, int q)
{
	bool LR = false;
	if(tokens[p].type == '(' && tokens[q].type ==')') LR = true;
	char stack[36];
	int top = 0, i;
	for(i = p; i < q; ++ i){
		if(tokens[i].type == '(') stack[++ top] = '(';
		else if(tokens[i].type == ')') {
			if(top > 0 && stack[top] == '(') --top;
			else return -1;	
		}
	}
	if(LR) return 1;
	return 0;
}

bool isOp(int ch){
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}
/* return the priority of the oprator*/
int op_priority(int op)
{
	int pri;
	switch(op){
		case '+':
		case '-':
			pri = 1;
			break;
		case '*':
		case '/':
			pri = 2;
			break;
		default:
			pri = 10;
	}
	return pri;
}
int compare(int op1, int op2){
	int p1 = op_priority(op1);
	int p2 = op_priority(op2);
	return p1 < p2 ?  -1 : (p1 == p2 ? 0 : 1);
}

/* return -1 if there is no main operator */
int get_main_op(int p, int q)
{
	int inBracket = 0, i, pos = -1;
	for(i = p; i <= q; ++ i) {
		int type = tokens[i].type;
		if( !inBracket && isOp(type)){
			if(pos == -1) pos = i;
			else if(compare(type, tokens[pos].type) <= 0 ) pos = i; 
		}
		else if(type == '(' ) inBracket ++ ;
		else if(type == ')' ) inBracket -- ;
	}
	return pos;
}

uint32_t myexit(int p, int q, bool *success){
	printf("Invalid expression: [%d, %d]\n", p, q);
	success = false;
	return 0;
}

/*add a parameter to judge whether the eval is success. */
uint32_t eval(int p, int q, bool *success)
{
	if(p > q) {
		return myexit(p, q, success);
	}else if(p == q){
		if(tokens[p].type == TK_NUM) {
			uint32_t val = 0;
			sscanf(tokens[p].str, "%u", &val);
			return val;
		}
		return myexit(p, q, success);
	}

	int ret = check_parentheses(p, q);
	if(ret == -1) {
		return myexit(p, q, success);
	}
	
	if(ret == 1) {
		return eval(p + 1, q - 1, success);
	}
	
	int pos = get_main_op(p, q);
	if(pos == -1){
		return myexit(p, q, success);
	}
		
	uint32_t val1, val2, val;
	val1 = eval(p, pos - 1, success);
	if(*success == 0) return 0;
	val2 = eval(pos + 1, q, success);
	if(*success == 0) return 0;

	switch(tokens[pos].type){
		case '+':
			val = val1 + val2;
			break;
		case '-':
			val = val1 - val2;
			break;
		case '*':
			val = val1 * val2;
			break;
		case '/':
			if(val2 == 0) {
				printf("Divide 0 error at [%d, %d]", p, q);
				return *success = false;
			}
			val = val1 / val2;
			break;
		default:
			return myexit(p, q, success);
	}

	return val;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */

  return 0;
}
