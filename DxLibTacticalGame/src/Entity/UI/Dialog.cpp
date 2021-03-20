#include "Dialog.h"

namespace Entity {
	
	/**
	 * @fn
	 * メッセージをセット
	 * @param (message) メッセージ文言
	 */
	void Dialog::show(const char* message, int frameLayerId, int contentLayerId)
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

		// ボタン
		okBtn_ = make_shared<TextButton>();

		okBtn_->setShape(frameX + PADDING, frameY + frameH - PADDING_BOTTOM - BUTTON_H, frameW - PADDING * 2, BUTTON_H);
		okBtn_->setText("OK", FONT_TYPE);
		okBtn_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
		okBtn_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
		okBtn_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);

		objectsControl.addObject(contentLayerId, okBtn_);

		// メッセージ
		messageBox_ = make_shared<Text>(message, frameX + PADDING, frameY + PADDING, FONT_TYPE, TEXT_COLOR_TYPE);
		objectsControl.addFigure(contentLayerId, messageBox_);

		isShow_ = true;
	}

	/**
	 * @fn
	 * ダイアログ非表示
	 */
	void Dialog::end()
	{
		isShow_ = false;

		frame_->destroy();
		okBtn_->destroy();
		messageBox_->destroy();
	}

	/**
	 * @fn
	 * 対象オブジェクトがOKボタンと同じか（クリック判定用）
	 * @return OKボタンの場合 trueを返す
	 */
	bool Dialog::isEqualsOkBtn(shared_ptr<Object> objSp) const
	{
		return objSp && okBtn_ == objSp;
	}

}