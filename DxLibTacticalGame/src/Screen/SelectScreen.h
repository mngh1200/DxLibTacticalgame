#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Screen/MenuScreen.h"
#include "Screen/BattleScreen.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button/CourseButton.h"
#include "Entity/View/Text.h"
#include "Entity/View/Panel.h"
#include "Battle/CheckWin.h"

using namespace std;

/**
 * @file SelectScreen.h
 * @brief �L�����y�[���Z���N�g���
 */

namespace Screen
{
	class SelectScreen : public ScreenBase
	{
	public:
		SelectScreen() : 
			selectedCourseId_(-1), 
			openScreen_(Screen::MAIN_MENU), 
			newCourseId_(-1) {};
		~SelectScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		void updateStageInfo();

		constexpr static int PADDING = 50;		//! �]��
		constexpr static int UNDER_LINE_MARGIN = 10; //! �����̗]��


		constexpr static int CONTENT_TOP = PADDING + 60;	//! �R���e���c������Y���W

		constexpr static int STAGE_LIST_W = 500;		//! �X�e�[�W���X�g�̕�
		constexpr static int STAGE_LIST_H = WIN_H - (PADDING + CONTENT_TOP); //! �X�e�[�W���X�g�̍���
		constexpr static int STAGE_LIST_PADDING = 20;	//! �X�e�[�W���X�g�̗]��

		constexpr static int STAGE_BUTTON_H = 36;		//! �X�e�[�W�{�^���̕�
		constexpr static int STAGE_BUTTON_MARGIN = 8;	//! �X�e�[�W�{�^���̗]��


		constexpr static int STAGE_INFO_MARGIN = 50;	//! �X�e�[�W��񗓂̗]��
		constexpr static int STAGE_INFO_X = PADDING + STAGE_LIST_W + STAGE_INFO_MARGIN; //! �X�e�[�W��񗓂�X���W
		constexpr static int STAGE_INFO_W = WIN_W - (STAGE_LIST_W + PADDING * 2 + STAGE_INFO_MARGIN); //! �X�e�[�W��񗓂̕�
		constexpr static int STAGE_INFO_H = 480;		//! �X�e�[�W��񗓂̍���
		constexpr static int STAGE_INFO_PADDING = 5;	//! �X�e�[�W��񗓂̗]���i�����j

		constexpr static int INFO_TEXT_X = STAGE_INFO_X + STAGE_INFO_PADDING;	//! �X�e�[�W���e�L�X�g��x���W
		constexpr static int INFO_TEXT_Y = CONTENT_TOP + 40;					//! �X�e�[�W���e�L�X�g��y���W

		constexpr static int LINE_HEIGHT = 32;	//! �s�̍���
		constexpr static int HINT_MARGIN_TOP = 20; //! �q���g�\���̏�̗]��

		constexpr static int BTN_MARGIN = 10; //! �{�^���̗]��
		constexpr static int BTN_W = STAGE_INFO_W / 2 - BTN_MARGIN;		//! �{�^���̕�
		constexpr static int BTN_H = 60; //! �{�^���̍���
		constexpr static int BTN_Y = CONTENT_TOP + STAGE_LIST_H - BTN_H;	//! �{�^���G���A��Y���W

		static const char* STAGE_KIND; //! �X�e�[�W�̎��

		int selectedCourseId_; //! �I�𒆂̃R�[�XID
		int newCourseId_; // �V�R�[�XID

		shared_ptr<Entity::Text> stageTitle_;	//! �X�e�[�W�^�C�g��

		shared_ptr<Entity::Text> winLabel_;		//! �����������x��
		shared_ptr<Entity::Text> winValue_;		//! ���������̓��e
		shared_ptr<Entity::Text> loseLabel_;	//! �s�k�������x��
		shared_ptr<Entity::Text> loseValue_;	//! �s�k�������x��

		shared_ptr<Entity::Text> hintLabel_;		//! �q���g���x��
		shared_ptr<Entity::Text> stageHint_;	//! �X�e�[�W�q���g

		enum Layer
		{
			MASK,
			COURSE_BUTTON,
			UI,
			PANEL,
			BACK,
			LEN
		};

		int openScreen_; //! �J�ڗ\��̃X�N���[��

		enum Screen
		{
			MAIN_MENU,
			BATTLE
		};

		enum UIid
		{
			BACK_BTN,
			START_BTN,
			COURSE_NAME,
			UIID_LEN
		};

		enum Scene
		{
			SELECT
		};
	};
}