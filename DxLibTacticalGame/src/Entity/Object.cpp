#include "Object.h"
#include "FrameWork/Game.h"

namespace Entity {
	/**
	 * @fn
	 * �}�E�X�C�x���g
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	 * @param (isOtherHit) ���̗v�f�ɐڐG�ς݂ł��邩
	 * @return �}�E�X���ڐG���Ă���Ƃ���true
	 */
	bool Object::checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit)
	{
		bool isHit = false;

		if (&shape_ != nullptr)
		{
			isHit = !isOtherHit && (shape_.isHit(x, y));

			// �}�E�X�|�C���^���ڐG�v
			if (isHit)
			{
				if (button == MOUSE_INPUT_LEFT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseLeftDown(x, y); // ��������
						isMouseDown_ = true;
					}
					else
					{
						onMouseLeftUp(x, y); // ��������
						
						if (isMouseDown_)
						{
							onMouseClick(x, y);
							*eventType = MOUSE_INPUT_LOG_CLICK;

							if (!isFocus_) // �t�H�[�J�X
							{
								isFocus_ = true;
								onFocus();
							}
						}
							
						isMouseDown_ = false;
					}
				}
				else if (button == MOUSE_INPUT_RIGHT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseRightDown(x, y); // �E������
					}
					else
					{
						onMouseRightUp(x, y); // �E������
					}
				}
				else if (button == MOUSE_INPUT_MIDDLE)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseWheelDown(x, y); // �z�C�[��������
					}
					else
					{
						onMouseWheelUp(x, y); // �z�C�[��������
					}
				}

				if (!isMouseOver_ && !isMouseDown_) {
					isMouseOver_ = true;
					onMouseOver(x, y); // �}�E�X�z�o�[�����u��
				}
			}
			else if (isMouseOver_)
			{
				// �}�E�X�|�C���^��ڐG���@���@1�t���[���O�܂ŐڐG���Ă����ꍇ
				onMouseOut(x, y); // �z�o�[�A�E�g
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return isHit;
	}

	/**
	 * @fn
	 * �t�H�[�J�X���O�������̋��ʏ����i�܂��́A�����ɂ�苭���I�Ƀt�H�[�J�X�A�E�g����Ƃ��ɌĂяo���j
	 */
	void Object::onBlurBase()
	{
		isFocus_ = false;
		onBlur();
	}

	/**
	 * @fn
	 * �����ɂ�苭���I�Ƀt�H�[�J�X����Ƃ��ɌĂяo��
	 */
	void Object::onForceFocus()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		shared_ptr<Object> prevFocusObject = game.objectsControl.focusObject.lock();

		if (prevFocusObject)
		{
			prevFocusObject->onBlurBase();
		}

		isFocus_ = true;
		onFocus();

		game.objectsControl.focusObject = game.objectsControl.getObjectWp(getLayerId(), getObjectId());
	}
}