#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/FontManager.h"
#include "Entity/UI/Back.h"
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
		const static int PADDING_TOP = 10;		//! 上側の余白
		const static int PADDING_LEFT = 25;		//! 左側の余白
		const static int LEFT_AREA_WIDTH = 800; //! 左側のエリアの幅

		const static int COURSE_TOP = 70;		//! コース一覧のY座標
		const static int COURSE_SIZE = 100;		//! コースボタンの幅と高さ
		const static int COUST_MARGIN = 45;		//! コースボタンの余白

		const static int RIGHT_AREA_PADDING_TOP = 25;	//! （右エリアの）上側の余白
		const static int RIGHT_AREA_PADDING_LEFT = 80;	//! (右エリアの)左側の余白

		int courseTitle_; // コース名表示欄








		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		enum UIid
		{
			BACK_BTN
		};
	};
}