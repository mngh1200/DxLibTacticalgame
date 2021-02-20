#include "ContextMenu.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	ContextMenu::ContextMenu() : width_(0), menuBtns_{}, displayArea_{ Shape(0, 0, WIN_W, WIN_H) }
	{
		type_ = CONTEXT_MENU;
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * �R���e�L�X�g���j���[�\��
	 * @param (sourceX) �������WX
	 * @param (sourceX) �������WY
	 */
	void ContextMenu::show(int sourceX, int sourceY)
	{
		show(Shape(sourceX, sourceY, 1, 1));
	}

	/**
	 * @fn
	 * �R���e�L�X�g���j���[�\��
	 * @param (sourceShape) �������̋�`
	 */
	void ContextMenu::show(Shape sourceShape)
	{
		int size = menuBtns_.size();

		if (size == 0) // �\���{�^�����Ȃ��ꍇ�͗L�������Ȃ�
		{
			return;
		}

		shape_.disabledHit = false;

		// ��`���W�w��
		int x = sourceShape.getX2();
		int y = sourceShape.getY2();
		int h = menuBtns_.size() * (BUTTON_MARGIN_BOTTOM + BUTTON_HEIGHT) - BUTTON_MARGIN_BOTTOM + PADDING * 2;

		if (x + width_ > displayArea_.getX2())
		{
			x = sourceShape.x - width_;
		}

		if (y + h > displayArea_.getY2())
		{
			y = sourceShape.y - h;
		}

		shape_.set(x, y, width_, h);

		// �{�^���ʒu�w��
		int num = 0;
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			int x = shape_.x + PADDING;
			int y = shape_.y + PADDING + (BUTTON_MARGIN_BOTTOM + BUTTON_HEIGHT) * num;

			itr->shape.set(x, y, width_ - PADDING * 2, BUTTON_HEIGHT);

			++num;
		}
	}

	/**
	 * @fn
	 * �R���e�L�X�g���j���[���\��
	 */
	void ContextMenu::hide()
	{
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * ���s����Ă���{�^����key��Ԃ�
	 * @param (x) �}�E�X��X���W
	 * @param (y) �}�E�X��Y���W
	 * @param (eventType) �C�x���g�̎��
	 * @return key(�Ώۂ��Ȃ��ꍇ��-1��Ԃ�)
	 */
	int ContextMenu::checkRunButton(int x, int y, int eventType) const
	{
		if (shape_.disabledHit)
		{
			return -1;
		}

		FrameWork::Controller& cont = FrameWork::Controller::getInstance();

		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->isDisabled) // �����̏ꍇ�͖���
			{
				continue;
			}

			if (eventType == MOUSE_INPUT_LOG_CLICK && itr->shape.isHit(x, y)) // �N���b�N����
			{
				return itr->key;
			}
			else if (cont.isKeyPressed(itr->keyCode)) // �L�[����
			{
				return itr->key;
			}
		}
		return -1;
	}

	/**
	 * @fn
	 * �`��
	 */
	void ContextMenu::render() const
	{
		if (shape_.disabledHit)
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// ���j���[�g
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, rm.getColor(ColorType::MAIN_COLOR), TRUE);

		int x, y;
		if (isMouseDown_ || isMouseOver_)
		{
			DxLib::GetMousePoint(&x, &y);
		}

		// �{�^��
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			int textColor = rm.getColor(ColorType::SUB_COLOR);
			int backgroundColor = rm.getColor(ColorType::MAIN_COLOR);

			if (itr->isDisabled)
			{
				textColor = rm.getColor(ColorType::NEGATIVE_COLOR);
			}
			else if (isMouseDown_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}
			else if (isMouseOver_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}

			DxLib::DrawRoundRectAA((float)itr->shape.x, (float)itr->shape.y, (float)itr->shape.getX2(), (float)itr->shape.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, backgroundColor, TRUE);
			DxLib::DrawFormatStringToHandle(itr->shape.x + BUTTON_PADDING_X, itr->shape.y + BUTTON_PADDING_Y, textColor, rm.getHdlFont(FontType::NORMAL_S18), itr->text.c_str());
		}
	}





	/**
	 * @fn
	 * �{�^���ǉ�
	 * @param (key) �{�^����key
	 * @param (text) �e�L�X�g
	 * @param (keyCode) �L�[�R�[�h��Ԃ� 
	 */
	void ContextMenu::addMenuButton(int key, string text, int keyCode)
	{
		menuBtns_.push_back(MenuButton{ key, text, keyCode });

		// �e�L�X�g���e���烁�j���[�̕�����
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int width = (PADDING + BUTTON_PADDING_X) * 2 + DxLib::GetDrawFormatStringWidthToHandle(rm.getHdlFont(FontType::NORMAL_S18), text.c_str());

		if (width > width_)
		{
			width_ = width;
		}
	}

	/**
	 * @fn
	 * �{�^�����N���A
	 */
	void ContextMenu::clearMenuButton()
	{
		menuBtns_.clear();
		width_ = 0;
	}

	/**
	 * @fn
	 * ����̃{�^���𖳌���Ԃ̐؂�ւ�
	 * @param (key) �Ώۃ{�^���̃L�[
	 * @param (isDisabled) true: ������ false: �L����
	 */
	void ContextMenu::setDisabledMenuButton(int key, bool isDisabled)
	{
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->key == key)
			{
				itr->isDisabled = isDisabled;
				return;
			}
		}
	}

	/**
	 * @fn
	 * �R���e�L�X�g���j���[�\���\�G���A���w��
	 * @param (displayArea) �\���\�G���A
	 */
	void ContextMenu::setDisplayArea(Shape displayArea)
	{
		displayArea_ = displayArea;
	}
}