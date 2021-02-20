#include "ContextMenu.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	ContextMenu::ContextMenu() : width_(0), menuBtns_{}, displayArea_{ Shape(0, 0, WIN_W, WIN_H) }
	{
		type_ = CONTEXT_MENU;
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * コンテキストメニュー表示
	 * @param (sourceX) 発生座標X
	 * @param (sourceX) 発生座標Y
	 */
	void ContextMenu::show(int sourceX, int sourceY)
	{
		show(Shape(sourceX, sourceY, 1, 1));
	}

	/**
	 * @fn
	 * コンテキストメニュー表示
	 * @param (sourceShape) 発生源の矩形
	 */
	void ContextMenu::show(Shape sourceShape)
	{
		int size = menuBtns_.size();

		if (size == 0) // 表示ボタンがない場合は有効化しない
		{
			return;
		}

		shape_.disabledHit = false;

		// 矩形座標指定
		int x = sourceShape.getX2();
		int y = sourceShape.getY2();
		int h = menuBtns_.size() * (BUTTON_MARGIN_BOTTOM + BUTTON_HEIGHT) - BUTTON_MARGIN_BOTTOM + PADDING * 2;

		if (x + width_ > displayArea_.getX2())
		{
			x = sourceShape.x - width_;
		}

		if (y + h > displayArea_.getY2())
		{
			y = sourceShape.y - h;
		}

		shape_.set(x, y, width_, h);

		// ボタン位置指定
		int num = 0;
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			int x = shape_.x + PADDING;
			int y = shape_.y + PADDING + (BUTTON_MARGIN_BOTTOM + BUTTON_HEIGHT) * num;

			itr->shape.set(x, y, width_ - PADDING * 2, BUTTON_HEIGHT);

			++num;
		}
	}

	/**
	 * @fn
	 * コンテキストメニューを非表示
	 */
	void ContextMenu::hide()
	{
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * 実行されているボタンのkeyを返す
	 * @param (x) マウスのX座標
	 * @param (y) マウスのY座標
	 * @param (eventType) イベントの種類
	 * @return key(対象がない場合は-1を返す)
	 */
	int ContextMenu::checkRunButton(int x, int y, int eventType) const
	{
		if (shape_.disabledHit)
		{
			return -1;
		}

		FrameWork::Controller& cont = FrameWork::Controller::getInstance();

		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->isDisabled) // 無効の場合は無視
			{
				continue;
			}

			if (eventType == MOUSE_INPUT_LOG_CLICK && itr->shape.isHit(x, y)) // クリック判定
			{
				return itr->key;
			}
			else if (cont.isKeyPressed(itr->keyCode)) // キー判定
			{
				return itr->key;
			}
		}
		return -1;
	}

	/**
	 * @fn
	 * 描画
	 */
	void ContextMenu::render() const
	{
		if (shape_.disabledHit)
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// メニュー枠
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, rm.getColor(ColorType::MAIN_COLOR), TRUE);

		int x, y;
		if (isMouseDown_ || isMouseOver_)
		{
			DxLib::GetMousePoint(&x, &y);
		}

		// ボタン
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			int textColor = rm.getColor(ColorType::SUB_COLOR);
			int backgroundColor = rm.getColor(ColorType::MAIN_COLOR);

			if (itr->isDisabled)
			{
				textColor = rm.getColor(ColorType::NEGATIVE_COLOR);
			}
			else if (isMouseDown_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}
			else if (isMouseOver_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}

			DxLib::DrawRoundRectAA((float)itr->shape.x, (float)itr->shape.y, (float)itr->shape.getX2(), (float)itr->shape.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, backgroundColor, TRUE);
			DxLib::DrawFormatStringToHandle(itr->shape.x + BUTTON_PADDING_X, itr->shape.y + BUTTON_PADDING_Y, textColor, rm.getHdlFont(FontType::NORMAL_S18), itr->text.c_str());
		}
	}





	/**
	 * @fn
	 * ボタン追加
	 * @param (key) ボタンのkey
	 * @param (text) テキスト
	 * @param (keyCode) キーコードを返す 
	 */
	void ContextMenu::addMenuButton(int key, string text, int keyCode)
	{
		menuBtns_.push_back(MenuButton{ key, text, keyCode });

		// テキスト内容からメニューの幅調整
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int width = (PADDING + BUTTON_PADDING_X) * 2 + DxLib::GetDrawFormatStringWidthToHandle(rm.getHdlFont(FontType::NORMAL_S18), text.c_str());

		if (width > width_)
		{
			width_ = width;
		}
	}

	/**
	 * @fn
	 * ボタンをクリア
	 */
	void ContextMenu::clearMenuButton()
	{
		menuBtns_.clear();
		width_ = 0;
	}

	/**
	 * @fn
	 * 特定のボタンを無効状態の切り替え
	 * @param (key) 対象ボタンのキー
	 * @param (isDisabled) true: 無効化 false: 有効化
	 */
	void ContextMenu::setDisabledMenuButton(int key, bool isDisabled)
	{
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->key == key)
			{
				itr->isDisabled = isDisabled;
				return;
			}
		}
	}

	/**
	 * @fn
	 * コンテキストメニュー表示可能エリアを指定
	 * @param (displayArea) 表示可能エリア
	 */
	void ContextMenu::setDisplayArea(Shape displayArea)
	{
		displayArea_ = displayArea;
	}
}