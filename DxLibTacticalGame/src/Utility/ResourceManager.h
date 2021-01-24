#pragma once
#include "Define.h"
#include "DxLib.h"
#include "resource.h"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
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
		WHITE,
		MAIN_COLOR,
		MAIN_COLOR_ON,
		MAIN_COLOR_ON_DARK,
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
		ATTACK_CLOSE,	//! 近接攻撃
		ATTACK_FIRE,	//! 射撃
		MOVE,			//! 移動
		WAIT,			//! 待機
		SELECT_UNIT,	//! ユニット選択
		SOUNDS_LEN
	};

	// イメージタイプ
	enum ImageType {
		UNIT,
		MAP,
		EFFECT,
		IMAGE
	};

	// エフェクト画像の種類
	enum EffectId
	{
		DAMAGE,
		MISS,
		BULLET
	};

	// ユニットのImageType (状態)
	enum UnitImageKind {
		NORMAL, // 通常
		ACTED   // 行動済み
	};

	// ImageのID
	enum ImageId {
		BACKGROUND_MENU,
		MASS_ATACK,
		MASS_MOVE,
		IMAGE_ID_LEN
	};

	// 弾薬画像のpos
	enum BulletPos
	{
		BULLET_BODY,
		UNLOADED, // 未装填
		BULLET_POS_LEN
	};

	enum MapId {
		STAGE1,
		STAGE2,
		MAP_ID_LEN
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

		int getImage(int type, int kind, int pos = 0) const;

		int getHdlFont(int kind) const;

		int getColor(int kind) const;

		int getSound(int kind) const;

		int getMapMass(int mapId, int w, int h) const;

		static void playSound(int kind);

		boolean isLoaded() const;

	private:
		ResourceManager()
			:image_{}, hdlFont_{}, colorType_{}, sounds_{}, loadFlag{ false }, mapList_{}
		{};
		~ResourceManager();

		void loadMapchip(const char* resourcePath, int* handle);
		void loadCharacterchip(const char* resourcePath, int* handle);
		void loadImage(const char* resourcePath, int* handle);

		void loadMapchip(const char* resourceName, const char* resourceType, int* handle);
		void loadCharacterchip(const char* resourceName, const char* resourceType, int* handle);
		void loadImage(const char* resourceName, const char* resourceType, int* handle);
		void loadDivImage(const char* resourceName, const char* resourceType, int allNum, int xNum, int yNum, int w, int h, int* handle);

		map<int, std::vector<int*>> image_;

		int loadImages();
		int loadFonts();
		int loadColors();
		int loadSounds();

		void loadFont(const LPCSTR fontFilePath);
		void unloadFont(const LPCSTR fontFilePath);

		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > loadMapCsv(const LPCSTR csvFilePath);

		int hdlFont_[FontType::FONT_LEN];
		int colorType_[ColorType::COLOR_LEN];
		int sounds_[SoundKind::SOUNDS_LEN];
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapList_[MapId::MAP_ID_LEN];

		boolean loadFlag;
	};
}