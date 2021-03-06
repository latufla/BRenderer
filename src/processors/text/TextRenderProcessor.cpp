#include "TextRenderProcessor.h"
#include "../../utils/SharedHeaders.h"

#include "../../exceptions/Exception.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../../assets/AssetLoader.h"

using std::string;
using std::array;
using std::pair;
using std::out_of_range;
using std::shared_ptr;
using std::weak_ptr;

using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::translate;

namespace br {
	TextRenderProcessor::TextRenderProcessor(shared_ptr<IAssetLoader>loader)
		: ProcessorBase(loader) {
	}

	TextRenderProcessor::~TextRenderProcessor() {
		auto allTextFields = idToTextField;
		for(auto& i : allTextFields) {
			removeTextField(i.first);
		}
	}

	void TextRenderProcessor::addTextField(uint32_t id, string text, string fontName, uint8_t fontSize, const vec4& color, const vec2& position) {
		auto it = idToTextField.find(id);
		if(it != cend(idToTextField))
			throw InvalidObjectIdException(EXCEPTION_INFO, id);

		auto sGConnector = graphics.lock();
		if(!sGConnector)
			throw WeakPtrException(EXCEPTION_INFO);

		Font& font = loader->getFontBy(fontName, fontSize);
		if(!hasTextFieldWithFont(font))
			loadFontToGpu(font);

		auto wSize = sGConnector->getWindowSize();
		vec2 scaleFactor = {(2.0f * sGConnector->getAspectRatio()) / wSize.w, 2.0f / wSize.h};

		TextField field{font, text, color, position, scaleFactor};
		loadTextFieldToGpu(field);

		idToTextField.emplace(id, field);
	}

	void TextRenderProcessor::removeTextField(uint32_t id) {
		std::string name = "";
		uint32_t size = 0;
		std::string nameAsKey = "";
		try {
			auto& field = idToTextField.at(id);
			name = field.getFontName();
			size = field.getFontSize();
			nameAsKey = field.getUniqueName();
		} catch(out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		idToTextField.erase(id);

		deleteTextFieldFromGpu(nameAsKey);
		
		Font& font = loader->getFontBy(name, size);
		if(!hasTextFieldWithFont(font)) // last out
			deleteFontFromGpu(font);
	}

	void TextRenderProcessor::translateTextField(uint32_t id, const vec2& position) {
		TextField* field;
		try {
			field = &idToTextField.at(id);
		} catch(out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}

		field->setPosition(position);
	}

	void TextRenderProcessor::doStep(const StepData& stepData) {
		auto sGConnector = graphics.lock();
		if(!sGConnector)
			throw WeakPtrException(EXCEPTION_INFO);

		sGConnector->setBlending(true);

		auto programContext = nameToProgramContext.at(AssetLoader::TEXT_PROGRAM);
		for(auto& i : idToTextField) {
			TextField& object = i.second;
			
			std::vector<IGraphicsConnector::ProgramParam> params;

 			IGraphicsConnector::ProgramParam color;
			color.id = programContext->getLoc("color");
			color.vec4 = std::make_shared<glm::vec4>(object.getColor());
 			params.push_back(color);
 
			IGraphicsConnector::ProgramParam mvp;
			mvp.id = programContext->getLoc("mvp");
			mat4 translation = translate(mat4(), vec3(object.getPosition(), 0.0f));
			mvp.mat4 = std::make_shared<glm::mat4>(translation * stepData.ortho);
			params.push_back(mvp);
			
			auto& buffer = meshToBuffer.at(object.getUniqueName());
			sGConnector->draw(buffer, programContext, params);
		}

		sGConnector->setBlending(false);		
	}


	void TextRenderProcessor::loadTextFieldToGpu(TextField& field) {
		if(idToTextField.empty()) {
			auto program = loader->getProgramBy(AssetLoader::TEXT_PROGRAM);
			loadProgramToGpu(program->getName(), program);
		}

		auto mesh = field.getMesh();
		loadGeometryToGpu(field.getUniqueName(), mesh->getRawVertices(), mesh->getIndices());

		Font& font = loader->getFontBy(field.getFontName(), field.getFontSize());
		Texture2d& atlas = font.getAtlas();
		uint32_t textureId = textureToId.at(atlas.getPath());
		
		auto& buffer = meshToBuffer.at(field.getUniqueName());
		buffer.texture = textureId;
	}

	void TextRenderProcessor::deleteTextFieldFromGpu(std::string nameAsKey) {
		if(idToTextField.empty())
			deleteProgramFromGpu(AssetLoader::TEXT_PROGRAM);

		deleteGeometryFromGpu(nameAsKey);
	}


	void TextRenderProcessor::loadFontToGpu(Font& font) {
		Texture2d& atlas = font.getAtlas();
		loadTextureToGpu(atlas);
	}

	void TextRenderProcessor::deleteFontFromGpu(Font& font) {
		Texture2d& atlas = font.getAtlas();
		deleteTextureFromGpu(atlas.getPath());
	}

	bool TextRenderProcessor::hasTextFieldWithFont(Font& font) {
		auto& it = find_if(cbegin(idToTextField), cend(idToTextField), [&font](pair<uint32_t, TextField> i)->bool {
			return i.second.getFontName() == font.getName()
				&& i.second.getFontSize() == font.getSize();
		});
		return it != cend(idToTextField);
	}
}