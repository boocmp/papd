#include "executable_task.h"

#include <filesystem>

ExecutableTask::ExecutableTask(const std::filesystem::path &executable)
    : executable_(executable) {}

std::filesystem::path ExecutableTask::Exec() {
  const auto cwd = std::filesystem::current_path();

  std::filesystem::current_path(
      cwd / std::filesystem::relative(executable_, cwd).remove_filename());
  auto outdir = std::filesystem::current_path();

  std::string command = executable_.filename().generic_string();
  if (ShouldCollectStdout()) {
    command += " > stdout";
  }
  std::system(command.c_str());
  std::filesystem::current_path(cwd);

  return outdir;
}
