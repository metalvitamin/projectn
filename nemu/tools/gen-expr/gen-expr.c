#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static uint32_t x = 0;
static uint32_t len = 0;
static uint32_t choose(uint32_t n){
  return rand()%n;
}
static void gen_random_space(){
  assert(x < 65536);
  buf[x] = ' ';
  x++;
  buf[x] = '\0';
}
static void gen(char c){
  assert(x < 65536);
  buf[x] = c;
  x++;
  buf[x] = '\0';
  if (!choose(5)) gen_random_space();
}
static void gen_num(){
  for (int i = 0; i < 2; i ++ , x ++) 
    if (x < 65536)  buf[x] = '0'+(rand()%8);
    else assert(0);
  if (!choose(10)) gen_random_space();
  buf[x] = '\0';
}

static void gen_rand_op(){
  switch (choose(4))
  {
  case 0: gen('+');break;
  case 1: gen('-');break;
  case 2: gen('*');break;
  case 3: gen('/');break;
  }
}

static inline void gen_rand_expr() {
  if (len >100) gen_num();
  else 
    switch(choose(3)){
      case(0):{gen_num();break;}
      case(1):{gen('(');gen_rand_expr();gen(')');len ++;break;}
      default:{gen_rand_expr();gen_rand_op();gen_rand_expr();len++;len++; break;}
    }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; ) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    static int  __attribute__((used)) q ;
    q = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
    for (;x > 0; x --) buf[x] = '\0';
    len = 0;
    i ++;
  }
  return 0;
}
