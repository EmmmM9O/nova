#include "BaseApplication.hpp"
namespace nova {
void BaseApplication::Finalize() {}
int BaseApplication::Initialize() {
  b_quit = false;
  return 0;
}
void BaseApplication::Tick() {}
bool BaseApplication::IsQuit() { return b_quit; }
}  // namespace nova
