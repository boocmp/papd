#pragma once

#include <filesystem>

class BaseTask {
public:
  virtual ~BaseTask() = default;

  virtual std::filesystem::path Exec() = 0;

  void CollectStdout();
  bool ShouldCollectStdout() const;

private:
  bool collect_stdout_ = false;
};