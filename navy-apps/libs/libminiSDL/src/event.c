#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  if(ev != NULL){
    char buf[64];
    if(NDL_PollEvent(buf, sizeof(buf))){
      char type;
      char name[13];
      sscanf(buf, "k%c %s", &type, name);
      if(type == 'u') ev->type = SDL_KEYUP;
      else if(type == 'd') ev->type = SDL_KEYDOWN;
      int i = 0;
      for(; i < sizeof(keyname)/sizeof(char); i ++) if(strcmp(name,keyname[i]) == 1) break;
      ev->key.keysym.sym = i;
      return 1;
    }
  }
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  if(event != NULL){
    while(1){
      char buf[64];
      if (NDL_PollEvent(buf, sizeof(buf))) {
        char type;
        char name[13];
        sscanf(buf, "k%c %s", &type, name);
        if(type == 'u') event->type = SDL_KEYUP;
        else if(type == 'd') event->type = SDL_KEYDOWN;
        int i = 0;
        for(; i < sizeof(keyname)/sizeof(char); i ++) if(strcmp(name,keyname[i]) == 1) break;
        event->key.keysym.sym = i;
        return 1;
      }
    }
  }
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
