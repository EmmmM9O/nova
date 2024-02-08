#pragma once
namespace nova {
class RuntimeModule {
 public:
  virtual ~RuntimeModule(){};
  virtual int Initialize() = 0;
  virtual void Finalize() = 0;
  virtual void Tick() = 0;
};
class Application : public RuntimeModule {
 public:
  virtual ~Application(){};
  virtual int Initialize() override = 0;
  virtual void Finalize() override = 0;
  virtual void Tick() override = 0;
  virtual bool IsQuit() = 0;
};
}  // namespace nova
