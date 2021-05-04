#include "Dialog.h"

namespace Entity {
	
	/**
	 * @fn
	 * メッセージをセット
	 * @param (message) メッセージ文言
	 * @param (frameLayerId) ダイアログ枠のレイヤーID
	 * @param (contentLayerId) ダイアログ内容そのレイヤーID
	 * @param (btn1text) ボタン１のテキスト
	 * @param (btn2text) ボタン２のテキスト（空白の場合はボタン１つ）
	 */
	void Dialog::show(const char* message, int frameLayerId, int contentLayerId, const char* btn1text, const char* btn2text)
	{
		if (isShow_)
		{
			end();
		}

		// 各種オブジェクト追加
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// フレーム
		frame_ = make_shared<ModalFrame>();

		int lineCount = 1;
		int frameW, frameH;

		DxLib::GetDrawStringSizeToHandle(&frameW, &frameH, &lineCount, message,
			DxLib::GetStringLength(message), rm.getHdlFont(FONT_TYPE));

		frameW += PADDING * 2;
		frameH += PADDING + MESSAGE_MARGIN_BOTTOM + BUTTON_H + PADDING_BOTTOM;

		int frameX = (WIN_W - frameW) / 2;
		int frameY = (WIN_H - frameH) / 2;

		frame_->setShape(frameX, frameY, frameW, frameH);

		objectsControl.addObject(frameLayerId, frame_);

		//! ボタン数が２つであるか
		bool is2btn = btn2text != "";

		// ボタンの幅計算
		int btnW = frameW - PADDING * 2;
		if (is2btn)
		{
			btnW = (btnW - BUTTON_MARGIN) / 2;
		}

		// ボタン1
		btn1_ = make_shared<TextButton>();

		btn1_->setShape(frameX + PADDING, frameY + frameH - PADDING_BOTTOM - BUTTON_H, btnW, BUTTON_H);
		btn1_->setText(btn1text, FONT_TYPE);
		btn1_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
		btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
		btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);

		objectsControl.addObject(contentLayerId, btn1_);

		// ボタン2
		if (is2btn)
		{
			btn2_ = make_shared<TextButton>();

			btn2_->setShape(frameX + PADDING + btnW + BUTTON_MARGIN, frameY + frameH - PADDING_BOTTOM - BUTTON_H, btnW, BUTTON_H);
			btn2_->setText(btn2text, FONT_TYPE);
			btn2_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);

			objectsControl.addObject(contentLayerId, btn2_);
		}


		// メッセージ
		messageBox_ = make_shared<Text>(message, frameX + PADDING, frameY + PADDING, FONT_TYPE, TEXT_COLOR_TYPE);
		objectsControl.addFigure(contentLayerId, messageBox_);

		isBtn1Disabled_ = isBtn2Disabled_ = false;

		isShow_ = true;
	}

	/**
	 * @fn
	 * ダイアログ非表示
	 */
	void Dialog::end()
	{
		isShow_ = false;

		if (frame_)
		{
			frame_->destroy();
		}
		
		if (btn1_)
		{
			btn1_->destroy();
		}
		
		if (btn2_)
		{
			btn2_->destroy();
		}
		
		if (messageBox_)
		{
			messageBox_->destroy();
		}
	}

	/**
	 * @fn
	 * メッセージ指定
	 * @param (message) メッセージテキスト
	 */
	void Dialog::setMessage(const char* message)
	{
		if (messageBox_)
		{
			messageBox_->setText(message);
		}
	}

	/**
	 * @fn
	 * ボタン１無効化
	 * @param (isDisabled) 無効にする場合 : true、有効化する場合 : false
	 */
	void Dialog::setDisabledBtn1(bool isDisabled)
	{
		// 状態が変化しない場合は終了
		if (isBtn1Disabled_ == isDisabled)
		{
			return;
		}

		isBtn1Disabled_ = isDisabled;

		if (isDisabled) // 無効化
		{
			btn1_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // 有効化
		{
			btn1_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
			btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
			btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * ボタン２無効化
	 * @param (isDisabled) 無効にする場合 : true、有効化する場合 : false
	 */
	void Dialog::setDisabledBtn2(bool isDisabled)
	{
		// 状態が変化しない場合は終了
		if (isBtn2Disabled_ == isDisabled)
		{
			return;
		}

		isBtn2Disabled_ = isDisabled;

		if (isDisabled) // 無効化
		{
			btn2_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // 有効化
		{
			btn2_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * 対象オブジェクトがボタン1と同じか（クリック判定用）
	 * @return ボタン1の場合 trueを返す
	 */
	bool Dialog::isEqualsBtn1(shared_ptr<Object> objSp) const
	{
		return objSp && !isBtn1Disabled_ && btn1_ == objSp;
	}

	/**
	 * @fn
	 * 対象オブジェクトがボタン2と同じか（クリック判定用）
	 * @return ボタン2の場合 trueを返す
	 */
	bool Dialog::isEqualsBtn2(shared_ptr<Object> objSp) const
	{
		return objSp && !isBtn2Disabled_  && btn2_ == objSp;
	}

}