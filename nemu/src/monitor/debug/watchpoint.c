#include "watchpoint.h"
#include "expr.h"
#include <monitor/monitor.h>
#define NR_WP 32
void cpu_exec(uint64_t);
static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp(){
  assert (free_ != NULL);
  WP *wp = free_;
  free_ = free_->next;
  
  if(head == NULL) {
    head = wp;
    wp->next = NULL;
    wp->NO = 1;
  }
  else {
    wp->next = head;
    wp->NO = head->NO + 1;
    head = wp;
  }
  return wp;
}

void free_wp(WP* wp){
  WP* new = head;
  if(head->next == NULL){
    head = NULL;
    wp->next = free_;
    free_ = wp;
  }
  else if (wp == head){
    head = head->next;
    wp->next = free_;
    free_ = wp;
  }
  else {
    for(;new->next != NULL; new = new->next){
      if (new->next == wp)
        break;
    }
    new->next = wp->next;
    wp->next = free_;
    free_ = wp;
  }
  
}
void make_wp(char* exp){
  WP* wp = new_wp();
  strncpy(wp->str, exp,32);
  wp->str[31] = '\0';
  bool success = true;
  wp->result = expr(wp->str,&success);
  while(true){
    WP* wp = head;
    cpu_exec(1);
    int answer = expr(wp->str, &success);
    for(;wp != NULL;wp = wp->next){
      if(wp->result != answer) {
        wp->result = answer;
        nemu_state.state = NEMU_STOP;
        return;
      }
    }
    if(nemu_state.state == NEMU_END || nemu_state.state == NEMU_ABORT) 
      return;
  }
}
void delete_wp(int n){
  WP* wp = head;
  for(;wp != NULL;wp = wp->next){
    if(wp->NO == n) free_wp(wp);
  }
}
void show_wp(){
  WP* wp = head;
  for(;wp != NULL;wp = wp->next){
    printf("Watchpoint No.%d, %s = %d\n",wp->NO,wp->str,wp->result);
  }
}
/* TODO: Implement the functionality of watchpoint */

