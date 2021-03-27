#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "ModalFrame.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/View/Text.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file Dialog.h
 * @brief ダイアログクラス
 */

namespace Entity
{
	class Dialog
	{
	public:
		Dialog() : isShow_(false) {};
		~Dialog() {};

		void show(const char* message, int frameLayerId, int contentLayerId);
		void end();

		bool isEqualsOkBtn(shared_ptr<Object> objSp) const;

	private:
		constexpr static int PADDING = 30; //! 余白
		constexpr static int PADDING_BOTTOM = 20; //! 下余白
		
		constexpr static int FONT_TYPE = FontType::NORMAL_S24; //! フォントの種類

		constexpr static int MESSAGE_MARGIN_BOTTOM = 30; //! メッセージの下余白
		constexpr static int BUTTON_H = 50; //! ボタンの高さ

		constexpr static int TEXT_COLOR_TYPE = ColorType::SUB_COLOR; //! テキスト色
		constexpr static int TEXT_HOVER_COLOR_TYPE = ColorType::MAIN_COLOR; //! ホバー時のテキスト色

		constexpr static int BUTTON_COLOR_TYPE = ColorType::SUB_COLOR_LITE; //! ボタン色
		constexpr static int BUTTON_HOVER_COLOR_TYPE = ColorType::SUB_COLOR; //! ホバー時のボタン色

		bool isShow_;	//! 表示中であるか

		shared_ptr<ModalFrame> frame_; //! モーダルの枠オブジェクト

		shared_ptr<TextButton> okBtn_; //! OKボタンの矩形座標

		shared_ptr<Text> messageBox_;	//! メッセージ
	};
}