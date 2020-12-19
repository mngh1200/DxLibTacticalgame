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
		SelectScreen() : selectedCourseId_(-1), openScreen_(Screen::MAIN_MENU), newCourseId_(-1) {};
		~SelectScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		constexpr static int PADDING_TOP = 10;		//! 上側の余白
		constexpr static int LEFT_AREA_WIDTH = 800; //! 左側のエリアの幅

		constexpr static int COURSE_TOP = 70;		//! コース一覧のY座標
		constexpr static int COURSE_MARGIN_X = 40;		//! コースボタンの余白
		constexpr static int COURSE_MARGIN_Y = 30;		//! コースボタンの余白
		constexpr static int COURSE_COLUMN_NUM = 5;		//! コースボタンの列数

		constexpr static int RIGHT_AREA_PADDING_LEFT = 80;	//! (右エリアの)左側の余白

		constexpr static int START_MARGIN = 70; //! スタートボタンの余白
		constexpr static int START_Y = 545;		//! スタートボタンのY座標
		constexpr static int START_HEIGHT = 100; //! スタートボタンの高さ

		constexpr static int BACK_SIZE = 60;	//! 戻るボタンのサイズ

		int selectedCourseId_; //! 選択中のコースID
		int newCourseId_; // 新コースID

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