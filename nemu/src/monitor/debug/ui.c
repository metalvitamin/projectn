#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include <memory/paddr.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <monitor/monitor.h>
void cpu_exec(uint64_t);
int is_batch_mode();
void show_wp();
void make_wp(char*);
void delete_wp(int);
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  if(args == NULL) cpu_exec(1);
  else cpu_exec(atoi(args));
  return 0;
}
static int cmd_info(char*args){   //todo: expand info w
  if (*args == 'r')
    isa_reg_display();
  else if (*args == 'w')
    show_wp();
  return 0;
}
static int cmd_x(char* args){   //todo: change this to a complete one
  paddr_t p;
  strtok(NULL," ");
  char *q;
  q = strtok(NULL," ");
  bool success = true;
  p = expr(q,&success);
  if (success){
    int  n = atoi(args);
    for (int i = 0; i < n; i ++)
      printf("%x\n",paddr_read(p+4*i,1));
    return 0;
  }
  else printf("invalid expr\n");
  return 0;
  
}
static int cmd_p(char*args){
  bool success = true;
  int n = expr(args, &success);
  if (success) {printf("%u\n",n); return 0;}
  printf("Wrong expression\n");
  return 0;
}
static int cmd_w(char* args){
  make_wp(args);
  return 0;
}
static int cmd_d(char* args){
  delete_wp(atoi(args));
  return 0;
}
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si","Execute the program n line",cmd_si},
  { "info","Print GPRs or Watchpoints",cmd_info},
  { "x","Output constantly N 4bits from the EXPR",cmd_x},
  { "p","Calculate the result of the expr given", cmd_p},
  { "w", "Make a Watchpoint", cmd_w},
  { "d", "Delete a Watchpoint", cmd_d}
  /* TODO: Add more commands */
  
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  #ifndef DEBUG
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }
  #endif

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
