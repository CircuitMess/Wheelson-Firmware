#ifndef WHEELSON_FIRMWARE_DESCRETESLIDER_H
#define WHEELSON_FIRMWARE_DESCRETESLIDER_H

#include "SettingsElement.h"

namespace SettingsScreen {
	class DescreteSlider : public SettingsElement {
	public:
		DescreteSlider(ElementContainer* parent, String name, std::vector<uint8_t> shutDownTime);

		void toggle() override;

		void selectNext();

		void selectPrev();

		bool isSliderSelected() const;

	private:
		bool sliderIsSelected = false;

		int index = 4;

		std::vector<uint8_t> shutDownTime;
	protected:
		void drawControl() override;
	};
}


#endif //WHEELSON_FIRMWARE_DESCRETESLIDER_H
