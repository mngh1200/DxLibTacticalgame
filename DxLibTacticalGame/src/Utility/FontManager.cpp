#include "FontManager.h"

namespace Utility {
	FontManager &FontManager::getInstance()
	{
		static FontManager instance;
		return instance;
	}

	FontManager::~FontManager()
	{
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");
	}

	int FontManager::load()
	{
		int ret = 0;

		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		hdlFont_[FontType::NORMAL_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S48]  = DxLib::CreateFontToHandle("Rounded M+ 1p black", 48, 3, DX_FONTTYPE_ANTIALIASING_4X4);

		// 色取得
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


		// 効果音取得
		sounds_[SoundKind::CLICK] = DxLib::LoadSoundMem("resource/sound/click.ogg");
		DxLib::ChangeVolumeSoundMem(210, sounds_[SoundKind::CLICK]);

		sounds_[SoundKind::CHECK] = DxLib::LoadSoundMem("resource/sound/check.ogg");
		DxLib::ChangeVolumeSoundMem(220, sounds_[SoundKind::CHECK]);

		sounds_[SoundKind::BACK] = DxLib::LoadSoundMem("resource/sound/back.wav");
		DxLib::ChangeVolumeSoundMem(180, sounds_[SoundKind::BACK]);

		sounds_[SoundKind::BORN] = DxLib::LoadSoundMem("resource/sound/born.wav");
		DxLib::ChangeVolumeSoundMem(190, sounds_[SoundKind::BORN]);

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

	void FontManager::loadFont(const LPCSTR fontFilePath)
	{
		if (AddFontResource(fontFilePath) > 0) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
	}

	void FontManager::unloadFont(const LPCSTR fontFilePath)
	{
		if (RemoveFontResource(fontFilePath)) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}
}