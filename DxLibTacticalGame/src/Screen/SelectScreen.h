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
#include "Battle/CheckWin.h"

using namespace std;

/**
 * @file SelectScreen.h
 * @brief キャンペーンセレクト画面
 */

namespace Screen
{
	class SelectScreen : public ScreenBase
	{
	public:
		SelectScreen() : 
			selectedCourseId_(-1), 
			stageKind_("stage"),
			openScreen_(Screen::MAIN_MENU), 
			newCourseId_(-1) {};
		~SelectScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		void updateStageInfo();

		constexpr static int PADDING_TOP = 10;		//! 上側の余白
		constexpr static int LEFT_AREA_WIDTH = 800; //! 左側のエリアの幅

		constexpr static int COURSE_TOP = 70;		//! コース一覧のY座標
		constexpr static int COURSE_MARGIN_X = 40;		//! コースボタンの余白
		constexpr static int COURSE_MARGIN_Y = 30;		//! コースボタンの余白
		constexpr static int COURSE_COLUMN_NUM = 5;		//! コースボタンの列数

		constexpr static int RIGHT_AREA_PADDING_LEFT = 80;	//! (右エリアの)左側の余白

		constexpr static int STAGE_INFO_Y = COURSE_TOP + 32 + 30; //! 勝利条件等を記載するY座標
		constexpr static int LINE_HEIGHT = 36;	//! 行の高さ
		constexpr static int HINT_MARGIN_TOP = 20; //! ヒント表示の上の余白


		constexpr static int START_MARGIN = 70; //! スタートボタンの余白
		constexpr static int START_Y = 545;		//! スタートボタンのY座標
		constexpr static int START_HEIGHT = 100; //! スタートボタンの高さ

		constexpr static int BACK_SIZE = 60;	//! 戻るボタンのサイズ

		string stageKind_;
		int selectedCourseId_; //! 選択中のコースID
		int newCourseId_; // 新コースID

		shared_ptr<Entity::Text> stageTitle_;	//! ステージタイトル

		shared_ptr<Entity::Text> winLabel_;		//! 勝利条件ラベル
		shared_ptr<Entity::Text> winValue_;		//! 勝利条件の内容
		shared_ptr<Entity::Text> loseLabel_;	//! 敗北条件ラベル
		shared_ptr<Entity::Text> loseValue_;	//! 敗北条件ラベル

		shared_ptr<Entity::Text> hintLabel_;		//! ヒントラベル
		shared_ptr<Entity::Text> stageHint_;	//! ステージヒント

		enum Layer
		{
			MASK,
			UI,
			COURSE_BUTTON,
			BACK,
			LEN
		};

		int openScreen_; //! 遷移予定のスクリーン

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
			BORN,
			SELECT
		};
	};
}