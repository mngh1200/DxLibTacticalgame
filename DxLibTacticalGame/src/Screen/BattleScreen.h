#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "Screen/SelectScreen.h"
#include "Screen/MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Utility/SaveManager.h"
#include "Entity/UI/Back.h"
#include "Entity/Battle/ResultScene.h"
#include "Battle/BattleManager.h"
#include "Battle/PlayerBattleController.h"
#include "Battle/EnemyBattleController.h"
#include "Battle/SetUnits.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Menu/ContextMenu.h"

using namespace std;

/**
 * @file BattleScreen.h
 * @brief �o�g�����
 */

namespace Screen
{
	class BattleScreen : public ScreenBase
	{
	public:
		BattleScreen() : 
			btlMng_{},
			playerBtlCont_{},
			openScreen_(-1),
			stageId_(0), 
			setUnitNum_(0),
			netHandler_(-1),
			isServer_(false)
		{};
		~BattleScreen() {};

		// ���C���[
		enum Layer
		{
			MASK,
			TOP_UI,
			CONTEXT_MENU,
			EFFECT,
			UI,
			PLAYER_UNIT,
			ENEMY_UNIT,
			MAP,
			BACK,
			LEN
		};

		// ���UI���C���[�̃I�u�W�F�N�gID
		enum TopUiId
		{
			MESSAGE,
			RESULT_SCENE
		};

		void init() override;
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType) override;
		void updateByAnimation() override;

		void setStage(int id);

		void prepareNetMatch(int netHandler, bool isServer, int mapId, int unitNum);

	private:
		void startBattle();
		void turnEnd();
		void confirmSetUnits();
		void showHint();
		void showCheckWinText();

		// �o�g���Ǘ��N���X
		Battle::BattleManager btlMng_;

		// �o�g����ʏ�̃v���C���[����Ǘ��N���X
		Battle::PlayerBattleController playerBtlCont_;

		// �G���j�b�g����N���X
		Battle::EnemyBattleController enemyBtlCont_;

		// �z�u�\���j�b�g��
		int setUnitNum_;

		//! �I���X�e�[�W��ID
		int stageId_;

		//! �\���ΏۃX�N���[��
		int openScreen_;

		//! �l�b�g�n���h���i�ʐM�ΐ�o�Ȃ��Ƃ��� -1�j
		int netHandler_;

		//! �T�[�o�[����
		bool isServer_;

		//! �V�X�e�����j���[
		shared_ptr<Entity::ContextMenu> systemMenu_;

		// �V�X�e�����j���[�̃L�[
		enum SystemMenuKey
		{
			TURN_END,			//! �^�[���G���h
			CHECK_WIN_TEXT,		//! ���s����
			HINT,				//! �q���g�\��
			BACK_SELECT_SCREEN,	//! �Z���N�g��ʂ֖߂�
			BACK_MENU_SCREEN	//! ���j���[��ʂ֖߂�
		};

		// �V�[���̎��
		enum Scene
		{
			SET_UNITS,		//! ���j�b�g�z�u
			WAIT_ENEMY_SET, //! �G���̃��j�b�g�z�u�҂�
			PLAYER_TURN,	//! �v���C���[�^�[��
			ENEMY_TURN,		//! �G�^�[��
			RESULT_ANIME,	//! ���s���ʑO�̃A�j���[�V����
			RESULT			//! ���s����
		};
	};
}