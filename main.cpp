#include <fstream>
#include <iostream>
#include <ranges>

#include "database/json_database.h"
#include "third_party/json/json.h"

#include "executor/cpp_generator.h"
#include "executor/interpretator_task.h"

#include "project/projects.h"

#include "dflow/dflow.h"

int main() {
  const auto d =
      dflow::GenerateDflowProjec("./modules/dflow/calculation_results.json");
  std::cout << "Predictive Analysis of the Pipe Destruction" << std::endl;

  auto projects = GetAllProjects("./projects");

  for (const auto &[index, p] : std::views::enumerate(projects)) {
    std::cout << std::setw(projects.size() / 10 + 1) << index + 1 << ". "
              << p.name << std::endl;
    for (const auto &m : p.tasks) {
      std::cout << "  " << m.module.name << " " << m.module.type << std::endl;
    }
  }

  // 1. Enumerate all directories in the `projects` folder
  // 2. Read `metadata.json` for each project
  // 3. Launch the selected project
  // 4. Store the result in the output directory.
  // 5. Optionally run the result visualizer.

//  ExecuteProject(projects[0]);

  for (const auto &z : d) {
    ExecuteProject(z);
  }

  return 0;
}