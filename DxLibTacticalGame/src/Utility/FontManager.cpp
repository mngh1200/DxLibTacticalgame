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
		hdlFont_[MEIRYO] = DxLib::CreateFontToHandle("メイリオ", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		hdlFont_[MSGOTHIC] = DxLib::CreateFontToHandle("MSゴシック", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		return ret;
	}

	int FontManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}
}