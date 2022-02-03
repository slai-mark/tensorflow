#ifndef PPU_STREAM_EXEC_H
#define PPU_STREAM_EXEC_H

extern "C" {

#include "tensorflow/c/experimental/stream_executor/stream_executor_test_util.h"
#include "tensorflow/c/experimental/stream_executor/stream_executor.h"

namespace salience {
namespace stream_exec {

void PopulateStreamExecutor(SP_StreamExecutor* se);
void PopulateTimerFns(SP_TimerFns* timer_fns);

} // namespace stream_exec
} // namespace salience

} // extern "C"
#endif
