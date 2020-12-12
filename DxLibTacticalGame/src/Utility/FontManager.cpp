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

		// hdlFont_[FontType::TITLE] = DxLib::CreateFontToHandle("ÉÅÉCÉäÉI", 90, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S24] = DxLib::LoadFontDataToHandle("resource/font/rounded-mplus/rounded-mplus-1p-medium-S24.dft", 0);
		hdlFont_[FontType::NORMAL_S32] = DxLib::LoadFontDataToHandle("resource/font/rounded-mplus/rounded-mplus-1p-medium-S32.dft", 0);
		hdlFont_[FontType::BLACK_S48] = DxLib::LoadFontDataToHandle("resource/font/rounded-mplus/rounded-mplus-1p-black-S48.dft", 0);

		// êFéÊìæ
		colorType_[ColorType::MAIN_COLOR] = DxLib::GetColor(250, 244, 232);
		colorType_[ColorType::MAIN_COLOR_ON] = DxLib::GetColor(254, 213, 175);

		colorType_[ColorType::SUB_COLOR] = DxLib::GetColor(124, 104, 95);

		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);
		
		colorType_[ColorType::PLAYER_COLOR] = DxLib::GetColor(35, 87, 137);
		colorType_[ColorType::ENEMY_COLOR] = DxLib::GetColor(209, 6, 6);

		colorType_[ColorType::POSITIVE_COLOR] = DxLib::GetColor(116, 142, 84);
		colorType_[ColorType::POSITIVE_LITE_COLOR] = DxLib::GetColor(185, 202, 165);

		colorType_[ColorType::NEGATIVE_COLOR] = DxLib::GetColor(139, 137, 130);
		
		colorType_[ColorType::ACCENT_COLOR] = DxLib::GetColor(204, 75, 194);


		// å¯â âπéÊìæ
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