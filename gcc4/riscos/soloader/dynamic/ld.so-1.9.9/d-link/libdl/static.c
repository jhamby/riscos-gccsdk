
#include <stdio.h>
#include <errno.h>

void *dlopen(void) {
  perror("Using 'dlopen' requires dynamic linking");
  errno = ENOSYS;
  return NULL;
}

char *dlerror(void) {
  return "Dynamic linking required";
}

int dlclose(void) {
  errno = ENOSYS;
  return -1;
}

void *dlsym(void) {
  perror("Using 'dlsym' requires dynamic linking");
  errno = ENOSYS;
  return NULL;
}

int dladdr(void) {
  perror("Using 'dladdr' requires dynamic linking");
  errno = ENOSYS;
  return -1;
}
