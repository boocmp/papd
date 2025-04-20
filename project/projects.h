#pragma once

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "../database/config.h"
#include "../database/database.h"

struct Module {
  enum class Type {
    kUnsupported,
    kCpp,
    kPython,
    kFreeFem,
  };

  std::string name;
  Type type = Type::kUnsupported;
  std::string entry_point;
  std::string output;

  std::filesystem::path root;
};

struct Task {
  std::string name;
  Module module;

  struct Input {
    db::BaseDatabase &source;
    std::string path;

    std::shared_ptr<db::BaseDatabase> source_raw;
  };
  std::vector<Input> input;
};

struct Project {
  std::string name;
  std::filesystem::path root;
  std::vector<Task> tasks;
};

std::ostream &operator<<(std::ostream &o, Module::Type t);

Module ParseModule(const json::JsonFile &mfile);
std::vector<Task::Input> ParseInput(const json::json &obj);

std::vector<Project> GetAllProjects(const std::string &projects_root);

void ExecuteProject(const Project &project);

std::unique_ptr<class BaseGenerator> GetGenerator(const Task &module);