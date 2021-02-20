#include "Timer.h"

namespace Utility {
	/**
	 * @fn
	 * タイマーセット
	 * @param (ms) カウント時間
	 */
	void Timer::setTimer(int ms)
	{
		countFrame_ = (ms - ONE_FRAME_MS + 1) / ONE_FRAME_MS;
	}

	/**
	 * @fn
	 * カウントダウン
	 * @return カウント終了時にtrueを返す
	 */
	bool Timer::checkAndCountTimer()
	{
		if (countFrame_ >= 0)
		{
			countFrame_--;

			if (countFrame_ < 0) // カウント終了
			{
				return true;
			}
		}
		return false;
	}

}