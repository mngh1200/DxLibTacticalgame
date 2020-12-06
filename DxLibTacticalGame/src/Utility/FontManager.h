#pragma once
#include <climits>
#include <string>
#include "DxLib.h"
#include "Define.h"

namespace
{
	enum FontType : int {
		NORMAL,
		TITLE,
		MAIN_MENU,
		FONT_LEN
	};

	enum ColorType : int {
		BACK,
		NORMAL_TEXT,
		TITLE_TEXT,
		BUTTON,
		COLOR_LEN
	};

	enum SoundKind : int {
		MOUSE_DOWN,
		SOUNDS_LEN
	};
}

namespace Utility
{
	class FontManager
	{
	public:

		// for singleton
		FontManager(const FontManager&) = delete;
		FontManager& operator=(const FontManager&) = delete;
		FontManager(FontManager&&) = delete;
		FontManager& operator = (FontManager&&) = delete;

		static FontManager& getInstance();

		int load();

		int getHdlFont(int kind) const;

		int getColor(int kind) const;

		int getSound(int kind) const;

	private:
		FontManager()
			:hdlFont_{}, colorType_{}
		{};
		~FontManager() {};
		int hdlFont_[FontType::FONT_LEN];
		int colorType_[ColorType::COLOR_LEN];
		int sounds_[SoundKind::SOUNDS_LEN];
	};
}