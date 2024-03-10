#include <cstdio>
#include <windows.h>

int main(int argc, char* argv[]) {
  printf("Hello, World!\n");

  return MessageBox(NULL, "hello, world", "caption", 0);
}
