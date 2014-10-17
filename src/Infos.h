#pragma once
#include <vector>
#include <memory>
#include "ObjectInfo.h"
#include <map>

static const std::string CUBE = "Cube";
static const std::string GUN = "Gun";
static const std::string POKEMON_TRAINER = "PokemonTrainer";
static const std::string SPIDERMAN_SYMBIOTE = "SpidermanSymbiote";
static const std::string STAN_LEE = "StanLee";

class Infos
{
public:
	Infos() = delete;
	~Infos() = delete;

	
	static ObjectInfo getInfo(std::string name = "") {
		return infos[name];
	}
private:
	static std::map < std::string, ObjectInfo > infos;
};
