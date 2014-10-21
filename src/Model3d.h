#pragma once
#include <vector>
#include "Mesh3d.h"
#include "Material3d.h"
#include "tree\Node.h"
#include "Animation3d.h"

class Model3d {
public:
	Model3d();
	Model3d(std::string, const std::vector<Mesh3d>&, const std::vector<std::string>&, std::shared_ptr<Node>, std::shared_ptr<Animation3d>);

	~Model3d();

	std::string getName() const { return name; }
	std::vector<Mesh3d>& getMeshes() { return meshes; }
	std::vector<Material3d>& getMaterials() { return materials; }

	std::shared_ptr<Animation3d> getAnimation() const { return animation; }

	Node::NodePtr getBoneTree() const { return boneTree; }

	operator std::string() const;

	std::string getUniqueMeshName(const Mesh3d&);

	glm::mat4& getGlobalInverseTransform() { return globalInverseTransform; }
	void setGlobalInverseTransform(glm::mat4 val) { globalInverseTransform = val; }

private:
	std::string name;
	
	std::vector<Mesh3d> meshes;
	std::vector<Material3d> materials;

	std::shared_ptr<Node> boneTree;
	std::shared_ptr<Animation3d> animation;

	glm::mat4 globalInverseTransform;
};

