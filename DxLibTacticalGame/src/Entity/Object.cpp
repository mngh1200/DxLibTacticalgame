#include "Object.h"

namespace Entity {
	/**
	 * @fn
	 * オブジェクトの実態を
	 * @param (shape) オブジェクトの実体要素
	 */
	Object::Object(Shape* shape)
	{
		shape_ = shape;
		isMouseDown_ = false;
		isMouseOver_ = false;
		objectType_ = OBJECT;
		isTrans_ = false;
		imagePath_ = -1;
	}
	/**
	 * @fn
	 * マウスイベント
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType)
	 * @param (isOtherHit) マウスのy座標
	 * @return マウスが接触しているときにtrue
	 */
	bool Object::checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit)
	{
		bool isHit = false;

		if (shape_ != nullptr)
		{
			isHit = !isOtherHit && (shape_->isHit(x, y));

			// マウスポインタが接触」
			if (isHit)
			{
				if (button == MOUSE_INPUT_LEFT)
				{
					if (eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseLeftDown(); // 左押下時
						isMouseDown_ = true;
					}
					else
					{
						onMouseLeftUp(); // 左離し時

						if (isMouseDown_)
						{
							onMouseClick();
							// eventTypeをクリックに書き換える
							*eventType = MOUSE_INPUT_LOG_CLICK;
						}
							
						isMouseDown_ = false;
					}
				}
				else if (button == MOUSE_INPUT_RIGHT)
				{
					if (eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseRightDown(); // 右押下時
					}
					else
					{
						onMouseRightUp(); // 右離し時
					}
				}
				else if (button == MOUSE_INPUT_MIDDLE)
				{
					if (eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseWheelDown(); // ホイール押下時
					}
					else
					{
						onMouseWheelUp(); // ホイール離し時
					}
				}

				if (!isMouseOver_ && !isMouseDown_) {
					isMouseOver_ = true;
					onMouseOver(); // マウスホバーした瞬間
				}
			}
			else if (isMouseOver_)
			{
				// マウスポインタ非接触時　かつ　1フレーム前まで接触していた場合
				onMouseOut(); // ホバーアウト
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return isHit;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Object::render() const
	{
		DxLib::DrawGraph(shape_->x, shape_->y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * shape_のサイズに合わせた描画処理
	 */
	void Object::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_->x, shape_->y, shape_->getX2(), shape_->getY2(), imagePath_, isTrans_);
	}
}