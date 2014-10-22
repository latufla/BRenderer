#pragma once
#include <vector>
#include <memory>

#include <string>
#include "WindowVendor.h"
#include "ObjectBase.h"
#include "Utils.h"

#include <array>
#include <utility>
#include "Model3dLoader.h"
#include <unordered_map>

class GrEngineConnector
{
public:
	~GrEngineConnector();
	
	static GrEngineConnector& getInstance(){
		static GrEngineConnector instance;
		return instance;
	}

	int32_t init();
	bool add(ObjectBase*);
	bool remove(ObjectBase*);
	bool draw();

	struct Camera {
		float x;
		float y;
		float z;
	};
	void setCamera(float, float, float);
	
	std::shared_ptr<WindowVendor> getWindow(){ return window; }

private:
	GrEngineConnector(){};
	GrEngineConnector(GrEngineConnector &){};
	GrEngineConnector operator=(GrEngineConnector&){};

	std::shared_ptr<WindowVendor> window;
	
	Model3dLoader loader;
	Camera camera;

	std::vector<ObjectBase*> objects;

	// VBO
	struct BufferData {
		uint32_t id;
		uint32_t length; // not size in bytes
	};
	typedef std::pair<BufferData, BufferData> BufferPair;
	std::map<std::string, BufferPair> meshToBuffer;
	// ---

	// textures
	std::map<std::string, uint32_t> meshToMaterial;

	// gl loading, sync
	void* display;
	void* surface;
	void* context;
	int32_t defaultProgram;
	
	int32_t initEgl();
	int32_t initShaders(std::string, std::string);

	uint32_t loadShader(uint32_t, const char*);
	uint32_t loadTexture(std::vector<unsigned char>&, int16_t, int16_t);
	// ---

	// animation
	struct BoneData {
		glm::mat4 offset;
		glm::mat4 finalTransform;
	};
	typedef std::unordered_map<uint32_t, BoneData> BonesDataMap;
	BonesDataMap createBonesData(std::shared_ptr<Model3d>, std::shared_ptr<Animation3d>, Mesh3d&);
	void transformBonesData(const glm::mat4&, Node::NodePtr, std::shared_ptr<Animation3d>, glm::mat4, BonesDataMap&);
	//
};

