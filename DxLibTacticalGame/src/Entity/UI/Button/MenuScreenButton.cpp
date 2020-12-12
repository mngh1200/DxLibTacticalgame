#include "MenuScreenButton.h"

namespace Entity {
	const float MenuScreenButton::ANIMATION_SCALE = 1.1f;

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	MenuScreenButton::MenuScreenButton() : animation_ {}
	{
		setTextX(TEXT_PADDING_LEFT);
		setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, State::NORMAL);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_OVER);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_DOWN);
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
		setShape(x, y, w, h);
		setText(text, FontType::BLACK_S48);
		baseShape_ = shape_;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬(ObjectsControl::addAnimationObj���\�b�h��p�ŌĂяo��)
	 * @return �A�j���[�V�����쐬�\�ȏꍇtrue��Ԃ�
	 */
	bool MenuScreenButton::createAnimation(int animationId)
	{
		// ���Ɏ��s�ς݂̃A�j���[�V����������ꍇ�͍쐬�s��
		if (animationId == animationId_)
		{
			return false;
		}
		
		if (animationId == EXPANSION) // �g��
		{
			animation_ = Animation(ANIMATION_MS);
			return true;
		}
		else if (animationId == SHRINK) // �k��
		{
			animation_ = Animation(ANIMATION_MS, Animation::REVERSE);
			animation_.adjustFrame(shape_, baseShape_, ANIMATION_SCALE);
			return true;
		}
		return false;
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
			bool isEnd = animation_.update(&shape_, baseShape_, ANIMATION_SCALE);
			setSize(shape_.w, shape_.h); // �e�L�X�g�ʒu�����p�ɍēK�p
			return isEnd;
		}
		return true;
	}


	/**
	 * @fn
	 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
	 */
	void MenuScreenButton::onMouseLeftDown()
	{
		joinAnimationList(EXPANSION); // �g��
		
		// �T�E���h
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		DxLib::PlaySoundMem(fontM.getSound(SoundKind::MOUSE_DOWN), DX_PLAYTYPE_BACK);

	}

	/**
	 * @fn
	 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
	 */
	void MenuScreenButton::onMouseLeftUp()
	{
		joinAnimationList(SHRINK); // �k��
	}

	/**
	 * @fn
	 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
	 */
	void MenuScreenButton::onMouseOut()
	{
		if (isMouseDown_)
		{
			joinAnimationList(SHRINK); // �k��
		}
	}

}