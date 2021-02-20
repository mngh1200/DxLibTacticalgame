#include "Timer.h"

namespace Utility {
	/**
	 * @fn
	 * �^�C�}�[�Z�b�g
	 * @param (ms) �J�E���g����
	 */
	void Timer::setTimer(int ms)
	{
		countFrame_ = (ms - ONE_FRAME_MS + 1) / ONE_FRAME_MS;
	}

	/**
	 * @fn
	 * �J�E���g�_�E��
	 * @return �J�E���g�I������true��Ԃ�
	 */
	bool Timer::checkAndCountTimer()
	{
		if (countFrame_ >= 0)
		{
			countFrame_--;

			if (countFrame_ < 0) // �J�E���g�I��
			{
				return true;
			}
		}
		return false;
	}

}