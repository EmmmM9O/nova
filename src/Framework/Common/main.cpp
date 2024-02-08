#include <cstdio>

#include "../Interface/Interface.hpp"
namespace nova {
extern Application* app;
}
using namespace nova;
int main(int argc, char** argv) {
  int res;
  if ((res = app->Initialize()) != 0) {
    printf("error code:%d\n", res);
    return res;
  }
  while (!app->IsQuit()) {
    app->Tick();
  }
  app->Finalize();
  return 0;
}
