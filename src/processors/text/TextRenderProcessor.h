#pragma once
#include "..\ProcessorBase.h"
#include <array>
#include "TextField.h"

namespace br {
	class TextRenderProcessor : public ProcessorBase {
	public:
		TextRenderProcessor(std::shared_ptr<IAssetLoader>loader);
		~TextRenderProcessor();

		void addTextField(uint32_t id,
			std::string text,
			std::string font,
			uint8_t fontSize,
			const glm::vec4& color,
			const glm::vec2& position);

		void removeTextField(uint32_t id);
		void translateTextField(uint32_t id, const glm::vec2& position);

	private:
		void doStep(const StepData& stepData) override;

		std::unordered_map<uint32_t, TextField> idToTextField;

		void loadFontToGpu(Font&);
		void deleteFontFromGpu(Font&);

		void loadTextFieldToGpu(TextField&);
		void deleteTextFieldFromGpu(std::string nameAsKey);
		bool hasTextFieldWithFont(Font&);
	};
}
