#pragma once

#include "base_task.h"

class ExecutableTask : public BaseTask {
public:
  explicit ExecutableTask(const std::filesystem::path &executable);

  std::filesystem::path Exec() override;

private:
  const std::filesystem::path executable_;
};
