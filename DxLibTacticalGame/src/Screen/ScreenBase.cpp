#include "ScreenBase.h"
#include "FrameWork/Game.h"

namespace Screen
{
	/**
	 * @fn
	 * 初期処理済みであるか判定
	 * @detail 初期処理済みであればtrue
	 */
	bool ScreenBase::isInited() const
	{
		return isInited_;
	}

	/**
	 * @fn
	 * 初期処理後に呼び出し、初期処理済み判定変数をtrueに変更
	 */
	void ScreenBase::inited()
	{
		isInited_ = true;
	}
}