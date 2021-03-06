#pragma once
#include <vector>
#include <memory>
#include "graphics/interfaces/IGraphicsConnector.h"
#include "assets/interfaces/IAssetLoader.h"

namespace br {	
	class ProcessorBase;
	class Renderer
	{
	public:
		Renderer() = delete;
		~Renderer() = default;

		Renderer(std::shared_ptr<IAssetLoader> loader, std::shared_ptr<IGraphicsConnector> graphics);
			
		bool doStep(long long stepMSec);
		
		void setCamera(float x, float y, float z);

		void addProcessor(std::shared_ptr<ProcessorBase> val);
		void removeProcessor(std::shared_ptr<ProcessorBase> val);

	private:
		std::shared_ptr<IAssetLoader> loader;
	
		std::shared_ptr<IGraphicsConnector> graphics;

		struct Camera {
			float x;
			float y;
			float z;
		} camera;
	
		std::vector<std::shared_ptr<ProcessorBase>> processors;
	};
}
