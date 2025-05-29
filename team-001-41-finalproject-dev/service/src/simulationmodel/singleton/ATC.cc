/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Helped to brainstorm different possible metrics to log
 */

#include "ATC.h"

#include <cmath>
#include <iostream>

#include "AirplaneDecorator.h"
#include "DataCollectionManager.h"
#include "DroneDecorator.h"
#include "HelicopterDecorator.h"

ATC ATC::instance;

ATC::ATC() {}

ATC::~ATC() {}

ATC& ATC::getInstance() { return instance; }

void ATC::addEntity(IEntity* entity) { flyingEntities.push_back(entity); }

void ATC::update(double dt) {
  // DCM integration
  DataCollectionManager* dcm = DataCollectionManager::getInstance();

  for (size_t i = 0; i < flyingEntities.size(); ++i) {
    for (size_t j = i + 1; j < flyingEntities.size(); ++j) {
      if (willCollide(flyingEntities[i], flyingEntities[j])) {
        dcm->logEvent(flyingEntities[i], "potential_collisions", 1.0);
        dcm->logEvent(flyingEntities[j], "potential_collisions", 1.0);

        if (!(flyingEntities[i]->isRerouted())) {
          flyingEntities[i]->reroute();

          dcm->logEvent(flyingEntities[j], "reroute_count", 1.0);
        } else if (!(flyingEntities[j]->isRerouted())) {
          flyingEntities[j]->reroute();

          dcm->logEvent(flyingEntities[j], "reroute_count", 1.0);
        } else {
          dcm->logEvent(flyingEntities[i], "collision_unavoidable", 1.0);
        }
      }
    }
  }
}

bool ATC::willCollide(IEntity* a, IEntity* b) {
  const float altitudeThreshold = 50.0f;
  const float baseCollisionTime = 5.0f;
  const float collisionDistanceThreshold = 50.0f;

  Vector3 posA = a->getPosition();
  Vector3 posB = b->getPosition();
  Vector3 dirA = a->getDirection().normalize();
  Vector3 dirB = b->getDirection().normalize();
  float speedA = a->getSpeed();
  float speedB = b->getSpeed();

  float altitudeDiff = std::abs(posA.y - posB.y);
  if (altitudeDiff > altitudeThreshold) {
    return false;
  }

  float maxSpeed = std::max(speedA, speedB);
  float collisionTimeThreshold = baseCollisionTime * pow(maxSpeed, 2) * 0.05;
  float maxDistance = 0.5 * collisionTimeThreshold;
  float currentDistance = (posA - posB).magnitude();
  if (currentDistance > maxDistance) {
    return false;
  }

  Vector3 relPos = posB - posA;
  Vector3 relVel = dirB * speedB - dirA * speedA;

  float relSpeedSquared = relVel * relVel;
  if (relSpeedSquared < 1) {
    return currentDistance < collisionDistanceThreshold;
  }

  float tClosest = -(relPos * relVel) / relSpeedSquared;

  if (tClosest < 0 || tClosest > collisionTimeThreshold) {
    return false;
  }

  Vector3 futurePosA = posA + dirA * speedA * tClosest;
  Vector3 futurePosB = posB + dirB * speedB * tClosest;
  float distanceAtClosest = (futurePosA - futurePosB).magnitude();

  return distanceAtClosest < collisionDistanceThreshold;
}

IEntity* ATC::chooseEntity(int i, int j) {
  int choice = rand() % 2;

  if (choice == 0) {
    if (!flyingEntities[i]->isRerouted()) {
      return flyingEntities[i];
    } else if (!flyingEntities[j]->isRerouted()) {
      return flyingEntities[j];
    } else {
      return nullptr;
    }

  } else {
    if (!flyingEntities[j]->isRerouted()) {
      return flyingEntities[j];
    } else if (!flyingEntities[i]->isRerouted()) {
      return flyingEntities[i];
    } else {
      return nullptr;
    }
  }
}
