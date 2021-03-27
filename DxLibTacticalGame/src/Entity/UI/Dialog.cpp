#include "Dialog.h"

namespace Entity {
	
	/**
	 * @fn
	 * ���b�Z�[�W���Z�b�g
	 * @param (message) ���b�Z�[�W����
	 */
	void Dialog::show(const char* message, int frameLayerId, int contentLayerId)
	{
		if (isShow_)
		{
			end();
		}

		// �e��I�u�W�F�N�g�ǉ�
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �t���[��
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

		// �{�^��
		okBtn_ = make_shared<TextButton>();

		okBtn_->setShape(frameX + PADDING, frameY + frameH - PADDING_BOTTOM - BUTTON_H, frameW - PADDING * 2, BUTTON_H);
		okBtn_->setText("OK", FONT_TYPE);
		okBtn_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
		okBtn_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
		okBtn_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);

		objectsControl.addObject(contentLayerId, okBtn_);

		// ���b�Z�[�W
		messageBox_ = make_shared<Text>(message, frameX + PADDING, frameY + PADDING, FONT_TYPE, TEXT_COLOR_TYPE);
		objectsControl.addFigure(contentLayerId, messageBox_);

		isShow_ = true;
	}

	/**
	 * @fn
	 * �_�C�A���O��\��
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
	 * �ΏۃI�u�W�F�N�g��OK�{�^���Ɠ������i�N���b�N����p�j
	 * @return OK�{�^���̏ꍇ true��Ԃ�
	 */
	bool Dialog::isEqualsOkBtn(shared_ptr<Object> objSp) const
	{
		return objSp && okBtn_ == objSp;
	}

}