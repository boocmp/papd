#include "base_task.h"

#include <memory>

class BaseExecutor {
public:
  virtual ~BaseExecutor() = default;

  virtual void Execute() = 0;
};