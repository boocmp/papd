#include "base_task.h"

void BaseTask::CollectStdout() { collect_stdout_ = true; }

bool BaseTask::ShouldCollectStdout() const { return collect_stdout_; }