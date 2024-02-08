#pragma once
#include "../Interface/Interface.hpp"
namespace nova {
class BaseApplication : public Application {
 public:
  virtual ~BaseApplication(){};
  virtual int Initialize() override;
  virtual void Finalize() override;
  virtual void Tick() override;
  virtual bool IsQuit() override;

 protected:
  bool b_quit;
};
}  // namespace nova
