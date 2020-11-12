#include "Object.h"
using namespace FrameWork;

namespace Entity {

	Object::Object (Shape* shape)
	{
		shape_ = shape;
		isMouseDown_ = false;
		isMouseOver_ = false;
	}

	/**
	 * @fn
	 * 当たり判定（主にマウスイベント用に使用）
	 */
	bool Object::isHit(int x, int y, int mouseStatus)
	{
		if (shape_ != nullptr) 
		{
			bool isHit = shape_->isHit(x, y);

			// マウスポインタが接触
			if (isHit) 
			{
				if (mouseStatus == Controller::MOUSE_LEFT_DOWN)
				{
					onMouseLeftDown(); // 左押下時
				}
				else if (mouseStatus == Controller::MOUSE_LEFT_UP)
				{
					onMouseLeftUp(); // 左離し時
				}
				else if (mouseStatus == Controller::MOUSE_RIGHT_DOWN)
				{
					onMouseRightDown(); // 右押下時
				}
				else if (mouseStatus == Controller::MOUSE_RIGHT_UP)
				{
					onMouseRightUp(); // 右離し時
				}
				else if (mouseStatus == Controller::MOUSE_WHEEL_DOWN)
				{
					onMouseWheelDown(); // ホイール押下時
				}
				else if (mouseStatus == Controller::MOUSE_WHEEL_UP)
				{
					onMouseWheelUp(); // ホイール離し時
				}
				isMouseOver_ = true;
			}
			else if (isMouseOver_)
			{
				// マウスポインタ非接触時　かつ　1フレーム前まで接触していた場合
				onMouseOut(); // ホバーアウト
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * デストラクタ
	 */
	Object::~Object()
	{
		delete shape_;
	}
}