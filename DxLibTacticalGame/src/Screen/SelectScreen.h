#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/FontManager.h"
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
		SelectScreen() : courseTitle_(-1) {};
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

		constexpr static int RIGHT_AREA_PADDING_TOP = 25;	//! （右エリアの）上側の余白
		constexpr static int RIGHT_AREA_PADDING_LEFT = 80;	//! (右エリアの)左側の余白

		int selectedCourseId_; //! 選択中のコースID

		int courseTitle_; //! コース名表示欄

		enum Layer
		{
			MASK,
			UI,
			COURSE_BUTTON,
			BACK,
			LEN
		};

		enum UIid
		{
			BACK_BTN
		};
	};
}