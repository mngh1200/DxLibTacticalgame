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
 * @brief ���\�[�X���Ǘ�����N���X
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
		CLICK,			//! �N���b�N
		CHECK,			//! �I��
		BACK,			//! �߂�
		MESSAGE,		//! ���b�Z�[�W
		BORN,			//! �V�R�[�X
		ATTACK_CLOSE,	//! �ߐڍU��
		ATTACK_FIRE,	//! �ˌ�
		MOVE,			//! �ړ�
		WAIT,			//! �ҋ@
		SELECT_UNIT,	//! ���j�b�g�I��
		WIN,			//! ����
		LOSE,			//! �s�k
		SOUNDS_LEN
	};

	// �C���[�W�^�C�v
	enum ImageType
	{
		UNIT,
		MAP,
		EFFECT,
		IMAGE
	};

	// �}�b�v�C���[�W�̎��
	enum MapImageKind
	{
		ALL,
		MOUNTAIN,
		RIVER,
		STATE
	};

	// �}�b�v�`�b�v���
	enum MapChipPos
	{
		PLAIN_POS,
		FOREST_POS,
		PLAYER_FORT_POS,
		ENEMY_FORT_POS,
		MAP_CHIP_LEN
	};

	// �G�t�F�N�g�摜�̎��
	enum EffectId
	{
		DAMAGE,
		MISS,
		BULLET,
		ARROW
	};

	// ���j�b�g��ImageType (���)
	enum UnitImageKind {
		NORMAL, // �ʏ�
		ACTED   // �s���ς�
	};

	// Image��ID
	enum ImageId {
		BACKGROUND_MENU,
		TITLE_IMAGE_ID,
		IMAGE_ID_LEN
	};

	// �}�X�̏�Ԃɂ����ʂ̉摜pos
	enum MassStatePos
	{
		MOVABLE_POS,
		MOVABLE_AND_ATTACKED_POS,
		ATTACKABLE_POS,
		MASS_STATE_LEN
	};

	// �e��摜��pos
	enum BulletPos
	{
		BULLET_BODY,
		UNLOADED, // �����U
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

		// �X�e�[�W�f�[�^���ǂ��܂œǂݍ��ނ�
		enum UntilStageLoad
		{
			TITLE,		//! �^�C�g���܂�
			CHECK_WIN,	//! ���s�����܂�
			ALL			//! �S��
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

		//! �R�摜�Ǎ��p�̏��
		constexpr static int MOUNTAIN_W = 4;
		constexpr static int MOUNTAIN_H = 4;
		constexpr static int MOUNTAIN_ALL = MOUNTAIN_W * MOUNTAIN_H;

		//! ��摜�Ǎ��p�̏��
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

		//! �t�H���g�f�[�^
		int hdlFont_[FontType::FONT_LEN];

		//! �J���[�f�[�^
		int colorType_[ColorType::COLOR_LEN];

		//! �����f�[�^
		int sounds_[SoundKind::SOUNDS_LEN];

		//! ���[�h�ς݂ł��邩
		boolean loadFlag;

		//! �ǂݍ��񂾃t�H���g�̃n���h��
		vector<HANDLE> fontHandleList_;
	};
}