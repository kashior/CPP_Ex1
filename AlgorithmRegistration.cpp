

#include "AlgorithmRegistration.h"
#include "RPSTourManager.h"
#include "RSPPlayer_203022041.h"

AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> func) {

    RPSTourManager::_scores[id] = 0;
    RPSTourManager::_algorithms[id] = func;
}

