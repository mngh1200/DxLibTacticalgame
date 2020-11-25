#include "ScreenBase.h"
#include "FrameWork/Game.h"

namespace Screen
{
	/**
	 * @fn
	 * ‰Šúˆ—Ï‚İ‚Å‚ ‚é‚©”»’è
	 * @detail ‰Šúˆ—Ï‚İ‚Å‚ ‚ê‚Îtrue
	 */
	bool ScreenBase::isInited() const
	{
		return isInited_;
	}

	/**
	 * @fn
	 * ‰Šúˆ—Œã‚ÉŒÄ‚Ño‚µA‰Šúˆ—Ï‚İ”»’è•Ï”‚ğtrue‚É•ÏX
	 */
	void ScreenBase::inited()
	{
		isInited_ = true;
	}
}