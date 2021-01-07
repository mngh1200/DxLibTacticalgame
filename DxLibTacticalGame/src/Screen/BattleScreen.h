#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "Screen/SelectScreen.h"
#include "Screen/MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/UI/Back.h"
#include "Battle/BattleManager.h"
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
		BattleScreen() : btlMng{}, openScreen_(-1) {};
		~BattleScreen() {};

		// ���C���[
		enum Layer
		{
			MASK,
			CONTEXT_MENU,
			EFFECT,
			UI,
			PLAYER_UNIT,
			ENEMY_UNIT,
			MAP,
			BACK,
			LEN
		};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		// �o�g���Ǘ��N���X
		Battle::BattleManager btlMng;

		//! �\���ΏۃX�N���[��
		int openScreen_;

		shared_ptr<Entity::ContextMenu> systemMenu_;

		// �V�[��
		enum Scene
		{
			PLAYER_TURN,
			ENEMY_TURN
		};

		// �V�X�e�����j���[�̃L�[
		enum SystemMenuKey
		{
			TURN_END,
			BACK_SELECT_SCREEN,
			BACK_MENU_SCREEN
		};
	};
}