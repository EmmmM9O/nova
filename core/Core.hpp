#include <memory>

#include "core/application.hpp"
namespace nova {
class Core {
 public:
  static std::shared_ptr<Application> app;
};
}  // namespace nova
