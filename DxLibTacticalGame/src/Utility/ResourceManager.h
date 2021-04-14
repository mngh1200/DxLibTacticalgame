#pragma once
#include "DxLib.h"
#include "Battle/BattleDefine.h"
#include "resource.h"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

namespace Battle
{
	class CheckWin;
}

/**
 * @file Resource Manager.h
 * @brief リソースを管理するクラス
 */

namespace
{
	enum FontType : int {
		NORMAL_S18,
		NORMAL_S20,
		NORMAL_S24,
		NORMAL_S32,
		BLACK_S24,
		BLACK_S32,
		BLACK_S48,
		BLACK_S64,
		FONT_LEN
	};

	enum ColorType : int {
		WHITE,
		MAIN_COLOR,
		MAIN_COLOR_ON,
		MAIN_COLOR_ON_DARK,
		SUB_COLOR,
		SUB_COLOR_LITE,
		SUB_COLOR_BIT_LITE,
		SUB_COLOR_DARK,
		TITLE_TEXT,
		PLAYER_COLOR,
		PLAYER_COLOR_LITE,
		ENEMY_COLOR,
		ENEMY_COLOR_LITE,
		POSITIVE_COLOR,
		POSITIVE_LITE_COLOR,
		NEGATIVE_COLOR,
		NEGATIVE_COLOR_LITE,
		NEGATIVE_COLOR_DARK,
		ACCENT_COLOR,
		COLOR_LEN
	};

	enum SoundKind : int {
		CLICK,			//! クリック
		CHECK,			//! 選択
		BACK,			//! 戻る
		MESSAGE,		//! メッセージ
		BORN,			//! 新コース
		ATTACK_CLOSE,	//! 近接攻撃
		ATTACK_FIRE,	//! 射撃
		MOVE,			//! 移動
		WAIT,			//! 待機
		SELECT_UNIT,	//! ユニット選択
		WIN,			//! 勝利
		LOSE,			//! 敗北
		SOUNDS_LEN
	};

	// イメージタイプ
	enum ImageType
	{
		UNIT,
		MAP,
		EFFECT,
		IMAGE
	};

	// マップイメージの種類
	enum MapImageKind
	{
		ALL,
		MOUNTAIN,
		RIVER,
		STATE
	};

	// マップチップ種類
	enum MapChipPos
	{
		PLAIN_POS,
		FOREST_POS,
		PLAYER_FORT_POS,
		ENEMY_FORT_POS,
		MAP_CHIP_LEN
	};

	// エフェクト画像の種類
	enum EffectId
	{
		DAMAGE,
		MISS,
		BULLET,
		ARROW
	};

	// ユニットのImageType (状態)
	enum UnitImageKind {
		NORMAL, // 通常
		ACTED   // 行動済み
	};

	// ImageのID
	enum ImageId {
		BACKGROUND_MENU,
		TITLE_IMAGE_ID,
		IMAGE_ID_LEN
	};

	// マスの状態による効果の画像pos
	enum MassStatePos
	{
		MOVABLE_POS,
		MOVABLE_AND_ATTACKED_POS,
		ATTACKABLE_POS,
		MASS_STATE_LEN
	};

	// 弾薬画像のpos
	enum BulletPos
	{
		BULLET_BODY,
		UNLOADED, // 未装填
		BULLET_POS_LEN
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

		static void playSound(int kind);

		// ステージデータをどこまで読み込むか
		enum UntilStageLoad
		{
			TITLE,		//! タイトルまで
			CHECK_WIN,	//! 勝敗条件まで
			ALL			//! 全部
		};

		static void loadStageTitle(const string stageKind, const int id, string* title);
		static void loadStageData(const string stageKind, const int id, string* title, string* hint, vector<int>* checkWinData);
		static void loadStageData(const string stageKind, const int id, string* title, string* hint, vector<int>* checkWinData, vector<int>* extraRules, std::array<std::array<int, MAP_MASS_W>, MAP_MASS_H>* mapData, vector<vector<int>>* units, int untilLoad = UntilStageLoad::ALL);

		boolean isLoaded() const;

	private:
		ResourceManager()
			:image_{}, hdlFont_{}, colorType_{}, sounds_{}, loadFlag{ false }
		{};
		~ResourceManager();

		//! 山画像読込用の情報
		constexpr static int MOUNTAIN_W = 4;
		constexpr static int MOUNTAIN_H = 4;
		constexpr static int MOUNTAIN_ALL = MOUNTAIN_W * MOUNTAIN_H;

		//! 川画像読込用の情報
		constexpr static int RIVER_NUM = 4;



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

		//! フォントデータ
		int hdlFont_[FontType::FONT_LEN];

		//! カラーデータ
		int colorType_[ColorType::COLOR_LEN];

		//! 音声データ
		int sounds_[SoundKind::SOUNDS_LEN];

		//! ロード済みであるか
		boolean loadFlag;

		//! 読み込んだフォントのハンドル
		vector<HANDLE> fontHandleList_;
	};
}