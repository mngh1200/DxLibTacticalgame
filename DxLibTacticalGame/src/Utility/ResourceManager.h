#pragma once
#include "Define.h"
#include "DxLib.h"
#include <map>
#include <vector>
using namespace std;

/**
 * @file Resource Manager.h
 * @brief リソースを管理するクラス
 */

namespace
{
	enum FontType : int {
		NORMAL_S18,
		NORMAL_S24,
		NORMAL_S32,
		BLACK_S48,
		FONT_LEN
	};

	enum ColorType : int {
		MAIN_COLOR,
		MAIN_COLOR_ON,
		SUB_COLOR,
		SUB_COLOR_LITE,
		SUB_COLOR_DARK,
		TITLE_TEXT,
		PLAYER_COLOR,
		PLAYER_COLOR_LITE,
		ENEMY_COLOR,
		ENEMY_COLOR_LITE,
		POSITIVE_COLOR,
		POSITIVE_LITE_COLOR,
		NEGATIVE_COLOR,
		ACCENT_COLOR,
		COLOR_LEN
	};

	enum SoundKind : int {
		CLICK,
		CHECK,
		BACK,
		BORN,
		SOUNDS_LEN
	};

	// イメージタイプ
	enum ImageType {
		PLAYER,
		ENEMY,
		MAP,
		IMAGE
	};
	// PLAYER、ENEMYのりソースの格納場所
	enum UnitKey {
		LANCER,
		CAVALRY,
		GUNNER
	};
	// PLAYER、ENEMYのアニメーション開始位置(仮)
	enum UnitStartPos {
		TOP,
		LEFT = 4,
		RIGHT = 8,
		DOWN = 12
	};

	// ImageのID
	enum ImageId {
		BACKGROUND_MENU,
		MASS_ATACK,
		MASS_MOVE,
		IMAGE_ID_LEN
	};
}

namespace Utility
{
	class ResourceManager
	{
	public:

		//for singleton
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		static ResourceManager& getInstance();

		int load();

		int getImage(int type, int kind) const;
		int getImage(int type, int kind, int pos) const;

		int getHdlFont(int kind) const;

		int getColor(int kind) const;

		int getSound(int kind) const;

		static void playSound(int kind);

	private:
		ResourceManager()
			:image_{}, hdlFont_{}, colorType_{}, sounds_{}
		{};
		~ResourceManager();

		void loadMapchip(const char* resourcePath, int* handle);
		void loadCharacterchip(const char* resourcePath, int* handle);
		void loadImage(const char* resourcePath, int* handle);

		map<int, std::vector<int*>> image_;

		int loadImages();
		int loadFonts();
		int loadColors();
		int loadSounds();

		void loadFont(const LPCSTR fontFilePath);
		void unloadFont(const LPCSTR fontFilePath);

		int hdlFont_[FontType::FONT_LEN];
		int colorType_[ColorType::COLOR_LEN];
		int sounds_[SoundKind::SOUNDS_LEN];
	};
}