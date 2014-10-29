// BLRenderer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "src\WindowVendor.h"

#include "ObjectBase.h"
#include "src\GrEngineConnector.h"
#include "src\Infos.h"
#include "src\Utils.h"
#include <gtc\type_ptr.hpp>

using namespace std;

vector<ObjectBase> objects;

int _tmain(int argc, _TCHAR* argv[]) {

	objects.push_back({ 42, CUBE });

	GrEngineConnector& renderer = GrEngineConnector::getInstance();
	int rendererFail = renderer.init(0, 0, 1024, 768);
	if (rendererFail)
		return rendererFail;

	renderer.setCamera(7.48f, 6.5f, 5.34f);
	
	const Model3dInfo& info = Infos::getInfo(CUBE);
	renderer.loadModel(info.getModelDir(), info.getModelName());
	renderer.attachAnimation(info.getModelPath(), "models/Cube/CubeIdle.dae", "idle");

	for (auto& s : objects) {
		uint32_t id = s.getId();
		const Model3dInfo& info = Infos::getInfo(s.getInfo());
		renderer.addObject(id, info.getModelPath());

		renderer.transform(id, Utils::toArray(s.getOrientation()));
	}

	renderer.playAnimation(42, "idle");

	const float fps = 1.0 / 60;
	const uint32_t step = (uint32_t)(fps * 1000);
	bool done = false;
	while (!done) {
		Sleep(step); // TODO: shirt WND 
		done = !renderer.doStep(step);
	}

	return 0;
}
