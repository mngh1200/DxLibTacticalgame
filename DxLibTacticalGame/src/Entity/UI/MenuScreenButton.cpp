#include "MenuScreenButton.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	MenuScreenButton::MenuScreenButton() : textY_(0), animation_ {}
	{
		type_ = Figure::BUTTON;
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::BUTTON);
		textColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (text) �{�^���̕���
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	MenuScreenButton::MenuScreenButton(string text, int x, int y, int w, int h) : MenuScreenButton()
	{
		shape_ = Shape(x, y, w, h);
		text_ = text;
		textY_ = (y + h / 2) - TEXT_SIZE / 2;

		baseShape_ = shape_;
	}

	/**
	 * @fn
	 * �`��
	 */
	void MenuScreenButton::render() const
	{

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), backgroundColor_, TRUE);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + ACCENT_WIDTH, shape_.getY2(), fontManager.getColor(ColorType::NORMAL_TEXT), TRUE);

		DxLib::DrawFormatStringToHandle(shape_.x + ACCENT_WIDTH + TEXT_PADDING_LEFT, textY_, textColor_, fontManager.getHdlFont(FontType::MAIN_MENU), text_.c_str());
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 * @return true:�I��
	 */
	bool MenuScreenButton::animationUpdate()
	{
		if (animationId_ == EXPANSION || animationId_ == SHRINK)
		{
			return animation_.update(&shape_, baseShape_, 1.2F);
		}
		return true;
	}


	/**
	 * @fn
	 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
	 */
	void MenuScreenButton::onMouseLeftDown()
	{
		// �g��
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(EXPANSION, getLayerId(), getObjectId());
		animation_ = Animation(100);
	}

	/**
	 * @fn
	 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
	 */
	void MenuScreenButton::onMouseLeftUp()
	{
		// �k��
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(SHRINK, getLayerId(), getObjectId());
		animation_ = Animation(100, Animation::REVERSE);
		animation_.adjustFrame(shape_, baseShape_, 1.2F);
	}

	/**
	 * @fn
	 * �}�E�X�Ƀz�o�[���ꂽ�u�Ԃ̏���
	 */
	void MenuScreenButton::onMouseOver()
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);
		textColor_ = fontManager.getColor(ColorType::BUTTON);
	}

	/**
	 * @fn
	 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
	 */
	void MenuScreenButton::onMouseOut()
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::BUTTON);
		textColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);

		// �k��
		if (isMouseDown_)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			game.objectsControl.addAnimationObj(SHRINK, getLayerId(), getObjectId());
			animation_ = Animation(100, Animation::REVERSE);
			animation_.adjustFrame(shape_, baseShape_, 1.2F);
		}
	}
}