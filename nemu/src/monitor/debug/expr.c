#include <isa.h>
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUMBER,

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  //done

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+",TK_NUMBER}, // numbers
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"\\*", '*'},         // multify
  {"/", '/'},           // divide
  {"==", TK_EQ},        // equal
  {"\\(", '('},         // left bracket
  {"\\)", ')'},         // right bracket
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

        switch (rules[i].token_type) {
          case(TK_NUMBER):{ 
            tokens[nr_token].type = TK_NUMBER;
            if (substr_len >= 32){
              printf("substr_len = %d >= 32, it is overflowed.\n",substr_len); 
              return false;
            }
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            nr_token ++;break;}
          case ('+'):{ tokens[nr_token].type = '+'; nr_token ++; break;}
          case ('-'):{ tokens[nr_token].type = '-'; nr_token ++; break;}
          case ('*'):{ tokens[nr_token].type = '*'; nr_token ++; break;}
          case ('/'):{ tokens[nr_token].type = '/'; nr_token ++; break;}
          case ('('):{ tokens[nr_token].type = '('; nr_token ++; break;}
          case (')'):{ tokens[nr_token].type = ')'; nr_token ++; break;}
          default: TODO();
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

static bool legal_exp(int p,int q){
  int i,j = 0;
  for (i = p; i <= q; i ++){
    if (tokens[i].type == '(') j ++;
    else if (tokens[i].type ==')') j --;
    if (j < 0) return false;
  }
  if(j != 0) return false;
  for (i = p; i < q; i ++){
    if(((tokens[i].type == TK_NUMBER)||(tokens[i].type == ')')) && ((tokens[i+1].type == TK_NUMBER) ||(tokens[i+1].type == '(')))
      return false;
    else if (tokens[i].type < TK_NOTYPE && tokens[i].type > ')' && tokens[i+1].type < TK_NOTYPE && tokens[i+1].type >')' )
      return false;
  }
  return true;
}

static bool check_paternheses(int p ,int q){
  if ((tokens[p].type != '(' )|| (tokens[q].type != ')')){
    return false;
  }
  else return legal_exp(p+1,q-1);
}

static int main_operator_index(int p, int q){
  int j = 0, ty = 0, ind = 0;
  for (int i = p; i <= q; i ++){
    if (tokens[i].type == TK_NUMBER) continue;
    else if (tokens[i].type == '(') j ++;
    else if (tokens[i].type == ')') j --;
    else if (j != 0) continue;
    else if ((tokens[i].type == '+') || (tokens[i].type == '-')) {
      ty = tokens[i].type;
      ind = i;
    }
    else if ((tokens[i].type == '*') || (tokens[i].type == '/')) {
      if ((ty == '+') || (ty == '-')) continue ;
      else {
        ty = tokens[i].type;
        ind = 1;
      }
    }
  }
  return ind;
}

static uint32_t eval(int p, int q){
  if(!legal_exp(p,q)) {printf("the input is illegal.\n"); assert(0);}
  if (p > q) assert(0);
  else if (p == q)  return atoi(tokens[p].str);
  else if (check_paternheses(p,q) == true) return eval(p+1, q-1);
  else {
    int op = main_operator_index(p,q);
    uint32_t val1 = eval(p , op - 1), val2 = eval( op + 1 , q);
    switch(tokens[op].type){
      case('+'):return val1+val2;
      case('-'):return val1-val2;
      case('*'):return val1*val2;
      case('/'):return (uint32_t) val1/val2;
    }
  }
  return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
 

  return eval(0,nr_token);
}
