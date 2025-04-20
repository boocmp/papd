#include "projects.h"

#include <filesystem>

#include "../executor/executors.h"
#include "../executor/generators.h"

#include "../third_party/json/json.h"

Module::Type ParseModuleType(const std::string &type) {
  if (type == "C++") {
    return Module::Type::kCpp;
  } else if (type == "FreeFem++") {
    return Module::Type::kFreeFem;
  } else if (type == "Python") {
    return Module::Type::kPython;
  }

  return Module::Type::kUnsupported;
}

Module ParseModule(const json::JsonFile &mfile) {
  const auto name = mfile.Get("name").get<std::string>();
  const auto type = ParseModuleType(mfile.Get("type").get<std::string>());
  const auto entry_poiny = mfile.Get("entrypoint").get<std::string>();
  const auto output = mfile.Get("output").get<std::string>();
  return {name, type, entry_poiny, output};
}

std::vector<Task::Input> ParseInput(const json::json &obj) {
  std::vector<Task::Input> r;
  for (const auto &[index, input] : obj.items()) {
    for (const auto &[source, path] : input.items()) {
      if (source == "DB.Constants") {
        r.emplace_back(db::Constants(), path);
        break;
      } else if (source == "Config") {
        auto raw_config = std::make_shared<db::Config>(path);
        auto &config = *raw_config;

        for (const auto &[key, cfg_value] : path.items()) {
          r.emplace_back(config, key, raw_config);
        }
      } else {
        throw std::runtime_error("Can't input find source: " + source);
      }
      break;
    }
  }
  return r;
}

std::ostream &operator<<(std::ostream &o, Module::Type t) {
  switch (t) {
  case Module::Type::kUnsupported:
    return o << "(Unsupported type)";
  case Module::Type::kCpp:
    return o << "(C++)";
  case Module::Type::kFreeFem:
    return o << "(FreeFem++)";
  case Module::Type::kPython:
    return o << "(Python)";
  }
  return o;
}

std::vector<Project> GetAllProjects(const std::string &projects_root) {
  int max_fails_count = 15;

  std::vector<Project> projects;

  for (const auto &proj : std::filesystem::directory_iterator(projects_root)) {
    if (max_fails_count <= 0) {
      break;
    }

    try {
      json::JsonFile pfile(proj.path() / "metadata.json");

      const auto name = pfile.Get("name").get<std::string>();

      projects.push_back({.name = name, .root = proj.path()});

      const auto tasks = pfile.Get("tasks");

      for (const auto &t : tasks) {
        std::filesystem::path mpath = proj.path().root_directory() / "modules" /
                                      t["module"].get<std::string>();

        json::JsonFile mfile(mpath / "metadata.json");

        Task task;
        task.module = ParseModule(mfile);
        task.module.root = mpath;
        task.input = ParseInput(t["input"]);

        projects.back().tasks.push_back(std::move(task));
      }

    } catch (const std::exception &e) {
      --max_fails_count;
      std::cout << "Project error: " << e.what() << std::endl;
      continue;
    }
  }
  return projects;
}

std::unique_ptr<BaseGenerator> GetGenerator(const Task &task) {
  switch (task.module.type) {
  case Module::Type::kCpp:
    return std::make_unique<CppGenerator>(task);
  case Module::Type::kPython:
    return std::make_unique<PythonGenerator>(task);
  case Module::Type::kFreeFem:
    return std::make_unique<FreeFemGenerator>(task);
  }
  return nullptr;
}

std::unique_ptr<BaseTask> GetExecutionTask(const Task &task) {
  std::unique_ptr<BaseTask> result;
  switch (task.module.type) {
  case Module::Type::kCpp:
    result = std::make_unique<ExecutableTask>(task.module.root / "build" /
                                              "Release" / task.module.name);
    break;

  case Module::Type::kPython:
    result = std::make_unique<PyhtonTask>(task.module.root /
                                          task.module.entry_point);
    break;
  case Module::Type::kFreeFem:
    result = std::make_unique<FreeFemTask>(task.module.root /
                                           task.module.entry_point);
  }
  if (result && task.module.output == "stdout") {
    result->CollectStdout();
  }

  return result;
}

void ExecuteProject(const Project &project) {
  std::cout << "Executing " << project.name << std::endl;

  const auto cwd = std::filesystem::current_path();

  for (const auto &task : project.tasks) {
    // Restore current working directory on each step.
    std::filesystem::current_path(cwd);
    {
      auto generator = GetGenerator(task);
      if (!generator) {
        std::cout << "Can't create generator for task: " << task.module.name;
        return;
      }
      generator->Generate();
    }
    {
      auto executor = GetExecutionTask(task);
      const auto outdir = executor->Exec();

      std::filesystem::current_path(cwd);
      try {
        const auto result_dir =
            cwd / "output" / project.name / task.name / task.module.name;
        std::filesystem::create_directories(result_dir);
        std::filesystem::copy(
            outdir / task.module.output, result_dir / task.module.output,
            std::filesystem::copy_options::overwrite_existing |
                std::filesystem::copy_options::recursive);
      } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
      }
    }
  }

  std::filesystem::current_path(cwd);
  std::cout << "Done: " << project.name << std::endl;
}
