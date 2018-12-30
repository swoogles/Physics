//
// Created by bfrasure on 10/30/18.
//

#include "PhysicsSandboxProperties.h"
#include "Serializer.h"

float Serializer::parseStringAsFloat(string value) {
  return strtof(value.c_str(), 0);
}

long Serializer::parseStringAsLong(string value) {
//  auto strValue = value.c_str();
  return strtol(value.c_str(), nullptr, 10);
}
