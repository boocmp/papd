#pragma once

#include <vector>

#include "../project/projects.h"

namespace dflow {

std::vector<Project>
GenerateDflowProjec(const std::filesystem::path &calculation_result_file);

} // namespace dflow
