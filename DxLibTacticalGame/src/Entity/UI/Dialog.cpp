#include "Dialog.h"

namespace Entity {
	
	/**
	 * @fn
	 * ���b�Z�[�W���Z�b�g
	 * @param (message) ���b�Z�[�W����
	 * @param (frameLayerId) �_�C�A���O�g�̃��C���[ID
	 * @param (contentLayerId) �_�C�A���O���e���̃��C���[ID
	 * @param (btn1text) �{�^���P�̃e�L�X�g
	 * @param (btn2text) �{�^���Q�̃e�L�X�g�i�󔒂̏ꍇ�̓{�^���P�j
	 */
	void Dialog::show(const char* message, int frameLayerId, int contentLayerId, const char* btn1text, const char* btn2text)
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

		//! �{�^�������Q�ł��邩
		bool is2btn = btn2text != "";

		// �{�^���̕��v�Z
		int btnW = frameW - PADDING * 2;
		if (is2btn)
		{
			btnW = (btnW - BUTTON_MARGIN) / 2;
		}

		// �{�^��1
		btn1_ = make_shared<TextButton>();

		btn1_->setShape(frameX + PADDING, frameY + frameH - PADDING_BOTTOM - BUTTON_H, btnW, BUTTON_H);
		btn1_->setText(btn1text, FONT_TYPE);
		btn1_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
		btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
		btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);

		objectsControl.addObject(contentLayerId, btn1_);

		// �{�^��2
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


		// ���b�Z�[�W
		messageBox_ = make_shared<Text>(message, frameX + PADDING, frameY + PADDING, FONT_TYPE, TEXT_COLOR_TYPE);
		objectsControl.addFigure(contentLayerId, messageBox_);

		isBtn1Disabled_ = isBtn2Disabled_ = false;

		isShow_ = true;
	}

	/**
	 * @fn
	 * �_�C�A���O��\��
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
	 * ���b�Z�[�W�w��
	 * @param (message) ���b�Z�[�W�e�L�X�g
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
	 * �{�^���P������
	 * @param (isDisabled) �����ɂ���ꍇ : true�A�L��������ꍇ : false
	 */
	void Dialog::setDisabledBtn1(bool isDisabled)
	{
		// ��Ԃ��ω����Ȃ��ꍇ�͏I��
		if (isBtn1Disabled_ == isDisabled)
		{
			return;
		}

		isBtn1Disabled_ = isDisabled;

		if (isDisabled) // ������
		{
			btn1_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // �L����
		{
			btn1_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
			btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
			btn1_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * �{�^���Q������
	 * @param (isDisabled) �����ɂ���ꍇ : true�A�L��������ꍇ : false
	 */
	void Dialog::setDisabledBtn2(bool isDisabled)
	{
		// ��Ԃ��ω����Ȃ��ꍇ�͏I��
		if (isBtn2Disabled_ == isDisabled)
		{
			return;
		}

		isBtn2Disabled_ = isDisabled;

		if (isDisabled) // ������
		{
			btn2_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // �L����
		{
			btn2_->setColor(BUTTON_COLOR_TYPE, TEXT_COLOR_TYPE, TextButton::State::NORMAL);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_DOWN);
			btn2_->setColor(BUTTON_HOVER_COLOR_TYPE, TEXT_HOVER_COLOR_TYPE, TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * �ΏۃI�u�W�F�N�g���{�^��1�Ɠ������i�N���b�N����p�j
	 * @return �{�^��1�̏ꍇ true��Ԃ�
	 */
	bool Dialog::isEqualsBtn1(shared_ptr<Object> objSp) const
	{
		return objSp && !isBtn1Disabled_ && btn1_ == objSp;
	}

	/**
	 * @fn
	 * �ΏۃI�u�W�F�N�g���{�^��2�Ɠ������i�N���b�N����p�j
	 * @return �{�^��2�̏ꍇ true��Ԃ�
	 */
	bool Dialog::isEqualsBtn2(shared_ptr<Object> objSp) const
	{
		return objSp && !isBtn2Disabled_  && btn2_ == objSp;
	}

}