#include "Core.hpp"

#include <memory>

#include "core/application.hpp"
namespace nova {
namespace Core {
Application *app = nullptr;
Graphics *graphics = nullptr;
async::Context *context = nullptr;
}  // namespace Core
}  // namespace nova
