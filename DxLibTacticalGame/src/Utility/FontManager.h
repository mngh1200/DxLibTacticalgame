#pragma once
#include <climits>
#include <string>
#include "DxLib.h"
#include "Define.h"

namespace Utility
{
	class FontManager
	{
	public:
		enum FontType : int {
			MEIRYO,
			MSGOTHIC
		};
		// for singleton
		FontManager(const FontManager&) = delete;
		FontManager& operator=(const FontManager&) = delete;
		FontManager(FontManager&&) = delete;
		FontManager& operator = (FontManager&&) = delete;

		static FontManager& getInstance();

		int load();

		int getHdlFont(int kind) const;

	private:
		FontManager()
			:hdlFont_{}
		{};
		~FontManager() {};
		int hdlFont_[2];
	};
}