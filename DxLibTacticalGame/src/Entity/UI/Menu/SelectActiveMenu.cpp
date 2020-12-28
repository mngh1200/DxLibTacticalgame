#include "SelectActiveMenu.h"

namespace Entity {

	/**
	 * @fn
	 * �J�n�i�\���j
	 * @param (unitX) �I�𒆂̃��j�b�g��x���W
	 * @param (unitY) �I�𒆂̃��j�b�g��y���W
	 */
	void SelectActiveMenu::start(int unitX, int unitY)
	{
		menuBtns_.clear();
		
		int size = 2; // �e�X�g���� �{�^���̌�

		shape_.set(unitX + CHIP_SIZE, unitY + CHIP_SIZE, WIDTH, size * (BUTTON_HEIGHT + BUTTON_MARGIN) + PADDING * 2);

		// �E�[�Ō��؂��ꍇ
		if (shape_.getX2() > WIN_W)
		{
			shape_.x = unitX - shape_.w;
		}

		// �ŉ����Ō��؂��ꍇ
		if (shape_.getY2() > WIN_H)
		{
			shape_.y = unitY - shape_.h;
		}

		int buttonX = shape_.x + PADDING; // �{�^��X���W
		int buttonY = shape_.y + PADDING; // �{�^��Y���W
		

		addMenuButton(ButtonKind::WAIT, "�ҋ@", buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN);
		addMenuButton(ButtonKind::CANCEL, "�L�����Z��", buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN);

		

		isDisplay_ = true;
		shape_.disabledHit = false;
	}

	/**
	 * @fn
	 * �I���i��\�����j
	 */
	void SelectActiveMenu::end()
	{
		shape_.disabledHit = true;
		isDisplay_ = false;
	}

	/**
	 * @fn
	 * �`��
	 */
	void SelectActiveMenu::render() const
	{
		if (!isDisplay_)
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

			if (isMouseDown_ && itr->shape.isHit(x, y))
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
			DxLib::DrawFormatStringToHandle(itr->shape.x + BUTTON_PADDING, itr->shape.y + BUTTON_PADDING, textColor, rm.getHdlFont(FontType::NORMAL_S24), itr->text.c_str());
		}
	}


	/**
	 * @fn
	 * ��������Ă���{�^����key�擾
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @return key
	 */
	int SelectActiveMenu::getHitButtonKey(int x, int y)
	{
		if (!isDisplay_)
		{
			return -1;
		}

		MenuButton* hitBtn = getHitMenuButton(x, y);

		if (hitBtn == nullptr)
		{
			return -1;
		}

		return hitBtn->key;
	}

	/**
	 * @fn
	 * �{�^���ǉ�
	 * @param (key) �{�^����key
	 * @param (text) �e�L�X�g
	 * @param (buttonX) x���W
	 * @param (buttonY) y���W
	 * @param (buttonW) ��
	 * @param (buttonH) ����
	 * @param (margin) �㉺�̗]��
	 */
	void SelectActiveMenu::addMenuButton(int key, string text, int buttonX, int buttonY, int buttonW, int buttonH, int margin)
	{
		int size = menuBtns_.size();
		menuBtns_.push_back(MenuButton{ key, text, Shape(buttonX, buttonY + (buttonH + margin) * size, buttonW, buttonH)});
	}


	/**
	 * @fn
	 * ��������Ă���{�^�����擾
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @return ��������Ă���{�^��
	 */
	MenuButton* SelectActiveMenu::getHitMenuButton(int x, int y)
	{
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->shape.isHit(x, y))
			{
				return &*itr;
			}
		}

		return nullptr;
	}

}