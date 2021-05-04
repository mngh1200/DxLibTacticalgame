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
		Dialog() : isShow_(false), isBtn1Disabled_(false), isBtn2Disabled_(false), is2btn_(false) {};
		~Dialog() {};

		void show(const char* message, int frameLayerId, int contentLayerId, const char* btn1text = "OK", const char* btn2text = "");
		void end();

		void setMessage(const char* message);
		void setDisabledBtn1(bool isDisabled);
		void setDisabledBtn2(bool isDisabled);

		bool isEqualsBtn1(shared_ptr<Object> objSp) const;
		bool isEqualsBtn2(shared_ptr<Object> objSp) const;

	private:
		void adjustSize();

		constexpr static int PADDING = 25; //! 余白
		constexpr static int PADDING_BOTTOM = 15; //! 下余白
		
		constexpr static int FONT_TYPE = FontType::NORMAL_S24; //! フォントの種類

		constexpr static int MESSAGE_MARGIN_BOTTOM = 30; //! メッセージの下余白
		constexpr static int BUTTON_H = 50; //! ボタンの高さ
		constexpr static int BUTTON_MARGIN = 10; //! ボタンの余白

		constexpr static int TEXT_COLOR_TYPE = ColorType::SUB_COLOR; //! テキスト色
		constexpr static int TEXT_HOVER_COLOR_TYPE = ColorType::MAIN_COLOR; //! ホバー時のテキスト色

		constexpr static int BUTTON_COLOR_TYPE = ColorType::SUB_COLOR_LITE; //! ボタン色
		constexpr static int BUTTON_HOVER_COLOR_TYPE = ColorType::SUB_COLOR; //! ホバー時のボタン色

		bool isShow_;	//! 表示中であるか

		bool is2btn_;	//! ボタンが２つ構成であるか

		bool isBtn1Disabled_, isBtn2Disabled_; //! 各ボタンが無効であるか

		shared_ptr<ModalFrame> frame_; //! モーダルの枠オブジェクト

		shared_ptr<TextButton> btn1_, btn2_; //! ボタン要素

		shared_ptr<Text> messageBox_;	//! メッセージ
	};
}