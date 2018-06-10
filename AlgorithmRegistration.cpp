

#include "AlgorithmRegistration.h"
#include "RPSTourManager.h"
#include "RSPPlayer_203022041.h"

AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    RPSTourManager::getTourManager().registerAlgorithm(id, factoryMethod);
}

