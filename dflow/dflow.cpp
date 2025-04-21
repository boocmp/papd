#include "dflow.h"

#include <filesystem>

#include "../third_party/json/json.h"

namespace {

// TODO: config name
void ExtendInput(Task &task, const std::string &name, const json::json &path) {
  auto raw_config = std::make_shared<db::Config>(path);
  auto &config = *raw_config;

  for (const auto &[key, cfg_value] : path.items()) {
    task.input.emplace_back(config, key, raw_config);
  }
}

std::string toCelsium(float v) { return std::to_string(v - 274.15f); }

Task CreateAC(const json::json &cell, int cell_index) {
  Task task;
  task.name = std::to_string(cell_index);
  task.module = ParseModule(json::JsonFile("./modules/3AC/metadata.json"));

  // TODO: root
  task.module.root = std::filesystem::current_path() / "modules" / "3AC";

  json::json base_config;
  base_config[0]["DB.Constants"] = "temperature.20 C";
  base_config[1]["Config"]["freq"] = 60;
  base_config[1]["Config"]["iter"] = 100;

  task.input = ParseInput(base_config);
  ExtendInput(task, "data", cell["data"]);
  ExtendInput(task, "linear_condition", cell["linear_condition"]);
  return task;
}

Task CreateBIO(const json::json &cell, int cell_index) {
  Task task;
  task.name = std::to_string(cell_index);
  task.module = ParseModule(json::JsonFile("./modules/BIO/metadata.json"));

  // TODO: root
  task.module.root = std::filesystem::current_path() / "modules" / "BIO";

  json::json base_config;
  base_config[0]["DB.Constants"] = "temperature.20 C";
  base_config[1]["Config"]["L"] = 10;
  base_config[1]["Config"]["Nx"] = 100;

  task.input = ParseInput(base_config);
  ExtendInput(task, "data", cell["data"]);
  ExtendInput(task, "linear_condition", cell["linear_condition"]);
  return task;
}

} // namespace

namespace dflow {

std::vector<Project>
GenerateDflowProjec(const std::filesystem::path &calculation_result_file) {
  json::JsonFile results(calculation_result_file);

  const auto pipes = results.Get("result.simulation_results")[0]["pipes"];

  std::vector<Project> projects;

  for (const auto &pipe : pipes.items()) {
    Project proj;
    proj.name = pipe.key();
    proj.root = "./projects/papd";

    size_t i = 0;
    for (const auto &cell : pipe.value()["cells"].items()) {
      proj.tasks.push_back(CreateAC(cell.value(), i));
      proj.tasks.push_back(CreateBIO(cell.value(), i));

      ++i;
    }
    projects.push_back(std::move(proj));
  }

  return projects;
}

} // namespace dflow