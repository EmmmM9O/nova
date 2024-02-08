#include "BaseApplication.hpp"
#include <cstdio>
namespace nova {
void BaseApplication::Finalize() {}
int BaseApplication::Initialize() {
  b_quit = false;
  return 0;
}
void BaseApplication::Tick() {
    printf("hello\n");
    b_quit=true;
}
bool BaseApplication::IsQuit() { return b_quit; }
}  // namespace nova
