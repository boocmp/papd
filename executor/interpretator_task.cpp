#include "interpretator_task.h"

#include <cstdlib>

InterpretatorTask::InterpretatorTask(
    const std::filesystem::path &executable,
    const std::vector<std::string> &command_line,
    const std::filesystem::path &entry_point)
    : executable_(executable), command_line_(command_line),
      entry_point_(entry_point) {}

InterpretatorTask::~InterpretatorTask() = default;

std::filesystem::path InterpretatorTask::Exec() {
  auto command = executable_.generic_string();

  for (const auto &p : command_line_) {
    command.append(" " + p);
  }

  const auto cwd = std::filesystem::current_path();
  std::filesystem::current_path(
      cwd / std::filesystem::relative(entry_point_, cwd).remove_filename());

  command.append(" " + entry_point_.filename().generic_string());

  if (ShouldCollectStdout()) {
    command += " > stdout";
  }

  std::system(command.c_str());
  return std::filesystem::current_path();
}

// -------------------------------------------------------------------

PyhtonTask::PyhtonTask(const std::filesystem::path entry_point)
    : InterpretatorTask("python", {}, entry_point) {}

PyhtonTask::~PyhtonTask() = default;

// -------------------------------------------------------------------

FreeFemTask::FreeFemTask(const std::filesystem::path entry_point)
    : InterpretatorTask("FreeFem++.exe", {"-ne", "-v 0"}, entry_point) {}

FreeFemTask::~FreeFemTask() = default;
