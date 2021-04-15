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
 * @brief キャンペーンセレクト画面
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

		constexpr static int PADDING = 50;		//! 余白
		constexpr static int UNDER_LINE_MARGIN = 10; //! 下線の余白


		constexpr static int CONTENT_TOP = PADDING + 60;	//! コンテンツ部分のY座標

		constexpr static int STAGE_LIST_W = 500;		//! ステージリストの幅
		constexpr static int STAGE_LIST_H = WIN_H - (PADDING + CONTENT_TOP); //! ステージリストの高さ
		constexpr static int STAGE_LIST_PADDING = 20;	//! ステージリストの余白

		constexpr static int STAGE_BUTTON_H = 36;		//! ステージボタンの幅
		constexpr static int STAGE_BUTTON_MARGIN = 8;	//! ステージボタンの余白


		constexpr static int STAGE_INFO_MARGIN = 50;	//! ステージ情報欄の余白
		constexpr static int STAGE_INFO_X = PADDING + STAGE_LIST_W + STAGE_INFO_MARGIN; //! ステージ情報欄のX座標
		constexpr static int STAGE_INFO_W = WIN_W - (STAGE_LIST_W + PADDING * 2 + STAGE_INFO_MARGIN); //! ステージ情報欄の幅
		constexpr static int STAGE_INFO_H = 480;		//! ステージ情報欄の高さ
		constexpr static int STAGE_INFO_PADDING = 5;	//! ステージ情報欄の余白（内側）

		constexpr static int INFO_TEXT_X = STAGE_INFO_X + STAGE_INFO_PADDING;	//! ステージ情報テキストのx座標
		constexpr static int INFO_TEXT_Y = CONTENT_TOP + 40;					//! ステージ情報テキストのy座標

		constexpr static int LINE_HEIGHT = 32;	//! 行の高さ
		constexpr static int HINT_MARGIN_TOP = 20; //! ヒント表示の上の余白

		constexpr static int BTN_MARGIN = 10; //! ボタンの余白
		constexpr static int BTN_W = STAGE_INFO_W / 2 - BTN_MARGIN;		//! ボタンの幅
		constexpr static int BTN_H = 60; //! ボタンの高さ
		constexpr static int BTN_Y = CONTENT_TOP + STAGE_LIST_H - BTN_H;	//! ボタンエリアのY座標

		static const char* STAGE_KIND; //! ステージの種類

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
			COURSE_BUTTON,
			UI,
			PANEL,
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
			SELECT
		};
	};
}