#include "Object.h"
#include "FrameWork/Game.h"

namespace Entity {
	/**
	 * @fn
	 * マウスイベント
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	 * @param (isOtherHit) 他の要素に接触済みであるか
	 * @return マウスが接触しているときにtrue
	 */
	bool Object::checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit)
	{
		bool isHit = false;

		if (&shape_ != nullptr)
		{
			isHit = !isOtherHit && (shape_.isHit(x, y));

			// マウスポインタが接触」
			if (isHit)
			{
				if (button == MOUSE_INPUT_LEFT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseLeftDown(x, y); // 左押下時
						isMouseDown_ = true;
					}
					else
					{
						onMouseLeftUp(x, y); // 左離し時
						
						if (isMouseDown_)
						{
							onMouseClick(x, y);
							*eventType = MOUSE_INPUT_LOG_CLICK;

							if (!isFocus_) // フォーカス
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
						onMouseRightDown(x, y); // 右押下時
					}
					else
					{
						onMouseRightUp(x, y); // 右離し時
					}
				}
				else if (button == MOUSE_INPUT_MIDDLE)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseWheelDown(x, y); // ホイール押下時
					}
					else
					{
						onMouseWheelUp(x, y); // ホイール離し時
					}
				}

				if (!isMouseOver_ && !isMouseDown_) {
					isMouseOver_ = true;
					onMouseOver(x, y); // マウスホバーした瞬間
				}
			}
			else if (isMouseOver_)
			{
				// マウスポインタ非接触時　かつ　1フレーム前まで接触していた場合
				onMouseOut(x, y); // ホバーアウト
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return isHit;
	}

	/**
	 * @fn
	 * フォーカスを外した時の共通処理（または、処理により強制的にフォーカスアウトするときに呼び出す）
	 */
	void Object::onBlurBase()
	{
		isFocus_ = false;
		onBlur();
	}

	/**
	 * @fn
	 * 処理により強制的にフォーカスするときに呼び出す
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