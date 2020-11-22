#include "Object.h"

namespace Entity {
	/**
	 * @fn
	 * �}�E�X�C�x���g
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType)
	 * @param (isOtherHit) �}�E�X��y���W
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
						onMouseLeftDown(); // ��������
						isMouseDown_ = true;
					}
					else
					{
						onMouseLeftUp(); // ��������
						
						if (isMouseDown_)
						{

							onMouseClick();
							// eventType���N���b�N�ɏ��������� TODO:�s�
							*eventType = MOUSE_INPUT_LOG_CLICK;


						}
							
						isMouseDown_ = false;
					}
				}
				else if (button == MOUSE_INPUT_RIGHT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseRightDown(); // �E������
					}
					else
					{
						onMouseRightUp(); // �E������
					}
				}
				else if (button == MOUSE_INPUT_MIDDLE)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseWheelDown(); // �z�C�[��������
					}
					else
					{
						onMouseWheelUp(); // �z�C�[��������
					}
				}

				if (!isMouseOver_ && !isMouseDown_) {
					isMouseOver_ = true;
					onMouseOver(); // �}�E�X�z�o�[�����u��
				}
			}
			else if (isMouseOver_)
			{
				// �}�E�X�|�C���^��ڐG���@���@1�t���[���O�܂ŐڐG���Ă����ꍇ
				onMouseOut(); // �z�o�[�A�E�g
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return isHit;
	}
}