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

		hdlFont_[FontType::NORMAL] = DxLib::LoadFontDataToHandle("resource/font/rounded-mplus/rounded-mplus-1p-medium-S24-T3-B4.dft", 0); // DxLib::CreateFontToHandle("メイリオ", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::TITLE] = DxLib::CreateFontToHandle("メイリオ", 90, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::MAIN_MENU] = DxLib::LoadFontDataToHandle("resource/font/rounded-mplus/rounded-mplus-1p-medium-S32-T3-B4.dft", 0);

		colorType_[ColorType::BACK] = DxLib::GetColor(247, 241, 230);
		colorType_[ColorType::NORMAL_TEXT] = DxLib::GetColor(124, 104, 95);
		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);
		colorType_[ColorType::BUTTON] = DxLib::GetColor(232, 220, 197);

		sounds_[SoundKind::MOUSE_DOWN] = DxLib::LoadSoundMem("resource/sound/mouse-down.wav");
		DxLib::ChangeVolumeSoundMem(128, sounds_[SoundKind::MOUSE_DOWN]);

		return ret;
	}

	int FontManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}
	int FontManager::getColor(int kind) const
	{
		return colorType_[kind];
	}
	int FontManager::getSound(int kind) const
	{
		return sounds_[kind];
	}
}