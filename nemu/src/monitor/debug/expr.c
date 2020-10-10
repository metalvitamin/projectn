#include <isa.h>
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
enum {
  TK_NOTYPE = 256, TK_EQ, TK_LOG_AND, TK_MINUS, TK_POINTER ,TK_NUMBER,

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
  {"&&", TK_LOG_AND},   // logical and
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

static Token tokens[128] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
static inline void init_token(int o){
  tokens[nr_token].type = o;
  tokens[nr_token].str[0] = 0;

}

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
        assert (nr_token < 32);

        switch (rules[i].token_type) {
          case(TK_NUMBER):{ 
            
            /*if (substr_len >= 32){
              printf("substr_len = %d >= 32, it is overflowed.\n",substr_len); 
              return false;
            }*/
            assert(substr_len <= 32);
            init_token(TK_NUMBER);
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token ++;
            break;
            }
          case ('+'):{ init_token('+');  nr_token ++; break;}
          case ('-'):{ init_token('-');  nr_token ++; break;}
          case ('*'):{ init_token('*');  nr_token ++; break;}
          case ('/'):{ init_token('/');  nr_token ++; break;}
          case ('('):{ init_token('(');  nr_token ++; break;}
          case (')'):{ init_token(')');  nr_token ++;break;}
          case (TK_LOG_AND):{ init_token(TK_LOG_AND);  nr_token ++; break;}
          case (TK_EQ):{ init_token(TK_EQ);  nr_token ++; break;}
          case (TK_NOTYPE):{ break;}
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
static bool legal_pat(int p, int q){
  int i,j = 0;
  for (i = p; i <= q; i ++){
    assert(i<nr_token);
    if (tokens[i].type == '(') j ++;
    else if (tokens[i].type ==')') j --;
    if (j < 0) return false;
  }
  if(j != 0) return false;
  return true;
}
static bool legal_exp(int p,int q){
  int i;
  assert(q < nr_token);
  assert(p < nr_token);
  if(!legal_pat(p,q)) return false;
  /*if (tokens[0].type == '-') tokens[0].type =TK_MINUS;
  else if ( tokens[0].type == '*') tokens[0].type = TK_POINTER;*/
  if (tokens[q].type != TK_NUMBER && tokens[q].type !=')') return false;
  for (i = p; i < q; i ++){
    assert(i <nr_token);
    assert(i+1 <nr_token);
    if(((tokens[i].type == TK_NUMBER)||(tokens[i].type == ')')) && ((tokens[i+1].type == TK_NUMBER) ||(tokens[i+1].type == '(')))
      return false;
    /*else if (tokens[i].type < TK_NUMBER && tokens[i].type != ')' && tokens[i+1].type == '-'){
      tokens[i+1].type = TK_MINUS;
      continue;
    }*/
    else if (tokens[i].type < TK_NUMBER && tokens[i].type != ')' && tokens[i+1].type == '*'){
      tokens[i+1].type = TK_POINTER;
      continue;
    }
    else if (tokens[i].type <= TK_LOG_AND && tokens[i].type > ')' && tokens[i+1].type <= TK_LOG_AND && tokens[i+1].type >')' )
      return false;
  }
  return true;
}

static bool check_paternheses(int p ,int q){
  assert(q <nr_token);
  assert(p <nr_token);
  if ((tokens[p].type != '(' )|| (tokens[q].type != ')')){
    return false;
  }
  else return legal_pat(p+1,q-1);
}

static int main_operator_index(int p, int q){
  int j = 0, ty = 0, ind = 0;
  for (int i = p; i <= q; i ++){
    assert(i <nr_token);
    if (tokens[i].type == TK_NUMBER) continue;
    else if (tokens[i].type == '(') j ++;
    else if (tokens[i].type == ')') j --;
    else if (j != 0) continue;
    else if (tokens[i].type ==TK_LOG_AND) {
      ty = TK_LOG_AND;
      ind = i;
    }
    else if (tokens[i].type == TK_EQ){
      if (ty == TK_LOG_AND) continue;
      else {
        ty = TK_EQ;
        ind = i;
      } 
    }
    else if ((tokens[i].type == '+') || (tokens[i].type == '-')) {
      if(ty == TK_LOG_AND || ty == TK_EQ)continue;
      else {
        ty = tokens[i].type;
        ind = i;
      }
    }
    
    
    else if ((tokens[i].type == '*') || (tokens[i].type == '/')) {
      if (ty == TK_EQ || ty == TK_LOG_AND || (ty == '+') || (ty == '-')) continue ;
      else {
        ty = tokens[i].type;
        ind = i;
      }
    }
    
    else if (tokens[i].type == TK_POINTER){
      if (ty == TK_LOG_AND || ty ==TK_EQ || ty =='+' || ty=='-' || ty == '*' || ty == '/' || ty == TK_POINTER) continue;
      else {
        ty = tokens[i].type;
        ind = i;
      }
    }
    else if (tokens[i].type == TK_MINUS)  {
      if ( ty == TK_LOG_AND || ty == TK_EQ || ty == '+' || ty == '-' || ty =='*' || ty == '/' || ty == TK_POINTER || ty == TK_MINUS) continue;
      else {
        ty = TK_MINUS;
        ind = i;
        //printf("%d %d\n",ty,ind);
      }
    }
  }
  return ind;
}

static uint32_t atou(char *ch){
  uint32_t result = 0;
  char *c ;
  c = ch;
  assert(c != NULL);
  while (*c != '\0'){
    result = result*10 + *c - '0';
    c ++;
  }
  return result;
}
static uint32_t eval(int p, int q){   
  assert(p < nr_token);   
  assert(p < nr_token);
  if (p > q) assert(0);
  else if (p == q)  {assert(p <= nr_token);printf("%d\n",atou(tokens[p].str));return atou(tokens[p].str);}
  else if (check_paternheses(p,q)) return eval(p+1, q-1);
  else {
    int op = main_operator_index(p,q);
    assert(op < q);
    if (tokens[op].type == TK_MINUS ){
      assert(op == p);
      uint32_t val1 = eval(p+1,q);
      return (uint32_t) val1*-1;
      }
    /*else if (tokens[op].type == TK_POINTER){
      bool s;
      uint32_t val ;
      val = isa_reg_str2val(tokens[op+1].str, &s);
      if (s) return val;
      else assert(0);
    }*/
    else { 
    uint32_t val1 = eval(p , op - 1), val2 = eval( op + 1 , q);
    switch(tokens[op].type){
      case('+'):return val1+val2;
      case('-'):return val1-val2;
      case('*'):return val1*val2;
      case('/'):return (uint32_t) val1/val2;
      case(TK_EQ): return val1==val2;
      case(TK_LOG_AND): return val1&&val2;
    }
  }
  }
  return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
    }
  //printf("%d\n",tokens[0].type);
  //printf("%d\n",tokens[1].type);
  //printf("%d\n",nr_token);
  /* TODO: Insert codes to evaluate the expression. */
  //done 
  for (int i = 0; i < nr_token; i ++) {
  if (tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == '+'|| tokens[i - 1].type == '-'||tokens[i - 1].type == '*'|| tokens[i - 1].type == '/'||tokens[i - 1].type == '('|| tokens[i - 1].type == TK_MINUS) ) {
    tokens[i].type = TK_MINUS;
  }
  }
  if(legal_exp(0,nr_token-1)) return eval(0,nr_token-1);
  else {
    *success = false; 
    return 0;
    }
}
