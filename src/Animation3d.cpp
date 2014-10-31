#include "stdafx.h"
#include "Animation3d.h"
#include "Utils.h"

using std::string;
using std::to_string; 

using std::vector;
using std::unordered_map;

const std::string Animation3d::DEFAULT_ANIMATION_NAME = "default";

Animation3d::Animation3d() {
}

Animation3d::Animation3d(string name, double duration, double ticksPerSecond, unordered_map<uint32_t, BoneAnimation>& idToBoneAnimation) {
	this->name = name;
	this->duration = duration;
	this->ticksPerSecond = ticksPerSecond;
	this->idToBoneAnimation = idToBoneAnimation;
}


Animation3d::~Animation3d() {
}

Animation3d::operator string() const {
	string res = "{Animation3d name: " + name + " duration: " + to_string(duration) + " ticksPerSecond: " + to_string(ticksPerSecond);
	res += " boneAnimations: {";
	for (auto& i : idToBoneAnimation) {
		res += "\n{BoneAnimation boneId: " + to_string(i.second.boneId) + " boneName:" + i.second.boneName;
		res += "\n positions (" + to_string(i.second.positions.size()) + ")";

#ifdef FULL_REPORT
		res += ": { \n";
		for (auto& j : i.positions) {
			res += "{Vec3Key time: " + to_string(j.time) + " value: " + Utils::toString(j.value) + "}\n";
		}
#endif

		res += " rotations (" + to_string(i.second.rotations.size()) + ")";

#ifdef FULL_REPORT
		res += ": { \n";
		for (auto& j : i.rotations) {
			res += "{Mat4Key time: " + to_string(j.time) + " value: \n" + Utils::toString(j.value) + "}\n";
		}
#endif 
		
		res += " scalings (" + to_string(i.second.scalings.size()) + ")";

#ifdef FULL_REPORT
		res += ": { \n";
		for (auto& j : i.scalings) {
			res += "{Vec3Key time: " + to_string(j.time) + " value: " + Utils::toString(j.value) + "}\n";
		}

#endif

		res += "}\n";
	}

	res += "}}";
	return res;
}

Animation3d::BoneAnimation* Animation3d::getBoneAnimation(uint32_t boneId) {
	auto it = idToBoneAnimation.find(boneId);
	if (it != cend(idToBoneAnimation))
		return &it->second;

	return nullptr;
}


