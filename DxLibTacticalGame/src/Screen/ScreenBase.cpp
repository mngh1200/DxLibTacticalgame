#include "ScreenBase.h"
#include "FrameWork/Game.h"

namespace Screen
{
	/**
	 * @fn
	 * ���������ς݂ł��邩����
	 * @detail ���������ς݂ł����true
	 */
	bool ScreenBase::isInited() const
	{
		return isInited_;
	}

	/**
	 * @fn
	 * ����������ɌĂяo���A���������ςݔ���ϐ���true�ɕύX
	 */
	void ScreenBase::inited()
	{
		isInited_ = true;
	}
}