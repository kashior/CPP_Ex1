

#include "AlgorithmRegistration.h"
#include "RPSTourManager.h"



AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    RPSTourManager::getTourManager().registerAlgorithm(id, factoryMethod);
}

