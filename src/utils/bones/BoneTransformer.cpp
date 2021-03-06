#include "../SharedHeaders.h"
#include <gtc/matrix_transform.hpp>
#include "BoneTransformer.h"
#include "../Util.h"

using std::vector;
using std::shared_ptr;
using glm::mat4;
using glm::vec3;

using glm::translate;
using glm::scale;

namespace br {
	void BoneTransformer::transform(View& object, shared_ptr<IModel3d> model, BonesDataMap& outBonesData) {
		auto& boneTree = model->getBoneTree();
		auto& anim = model->getAnimationBy(object.getAnimationName());
		long long animTime = object.getAnimationTime();
		mat4 pTrans;
		doTransform(boneTree, anim, animTime, pTrans, outBonesData);
	}
	
	void BoneTransformer::doTransform(BNode<BoneNodeData>& boneTree, Animation3d& animation, long long animationTime, mat4 parentTransform, BonesDataMap& outBonesData) {
		uint32_t boneId = boneTree.getId();
		BoneNodeData& bNData = boneTree.getData();
		mat4 nodeTransform = bNData.getTransform();
	
		auto bAnim = animation.getBoneAnimation(boneId);
		if (bAnim) {
			vec3 scalingV = calcTimeInterpolation<vec3, Animation3d::Vec3Key>(animationTime, bAnim->scalings);
			mat4 scalingM;
			scalingM = scale(scalingM, scalingV);
	
			mat4 rotationM = calcTimeInterpolation<mat4, Animation3d::Mat4Key>(animationTime, bAnim->rotations);
	
			vec3 translationV = calcTimeInterpolation<vec3, Animation3d::Vec3Key>(animationTime, bAnim->positions);
			mat4 translationM;
			translationM = translate(translationM, translationV);
	
			nodeTransform = translationM * rotationM * scalingM;
		}
	
		mat4 globalTransform = parentTransform * nodeTransform;
	
		if (outBonesData.find(boneId) != end(outBonesData)) {
			BoneData& outBData = outBonesData[boneId];
			outBData.finalTransform = globalTransform * outBData.offset;
		}
	
		BNode<BoneNodeData>::ChildrenList& children = boneTree.getChildren();
		uint32_t nChildren = children.size();
		for (uint32_t i = 0; i < nChildren; ++i) {
			doTransform(children[i], animation, animationTime, globalTransform, outBonesData);
		}
	}
}