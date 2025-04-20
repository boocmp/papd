#pragma once

#include "base_task.h"

#include <filesystem>

class InterpretatorTask : public BaseTask {
public:
  InterpretatorTask(const std::filesystem::path &executable,
                    const std::vector<std::string> &command_line,
                    const std::filesystem::path &entry_point);
  ~InterpretatorTask() override;

  std::filesystem::path Exec() override;

private:
  const std::filesystem::path executable_;
  const std::vector<std::string> command_line_;
  const std::filesystem::path entry_point_;
};

class PyhtonTask : public InterpretatorTask {
public:
  explicit PyhtonTask(const std::filesystem::path entry_point);
  ~PyhtonTask() override;
};

class FreeFemTask : public InterpretatorTask {
public:
  explicit FreeFemTask(const std::filesystem::path entry_point);
  ~FreeFemTask() override;
};

// TODO: Add Fem++ task by subclassing the `InterpretatorTask` in the same way
// as `PythonTask`.
// Notes: These classes assume that interpretators are situated
// in the known locations or can be accessed through the PATH environment. It
// would be good to extend locations by some kind of `Environment` configuration
// object which is accessible everywhere (Sigleton?).