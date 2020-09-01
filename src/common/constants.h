#pragma once

#include <string>

namespace raftcpp {

class RaftcppConstants {
public:
    constexpr static uint64_t DEFAULT_ELECTION_TIMER_TIMEOUT_MS = 5000;
    constexpr static uint64_t DEFAULT_HEARTBEAT_INTERVAL_MS = 3000;
};

}  // namespace raftcpp
