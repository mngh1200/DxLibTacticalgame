#include "FontManager.h"

namespace Utility {
	FontManager &FontManager::getInstance()
	{
		static FontManager instance;
		return instance;
	}

	int FontManager::load()
	{
		int ret = 0;
		hdlFont_[MEIRYO] = DxLib::CreateFontToHandle("���C���I", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		hdlFont_[MSGOTHIC] = DxLib::CreateFontToHandle("MS�S�V�b�N", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		return ret;
	}

	int FontManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}
}