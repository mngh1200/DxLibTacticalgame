#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Entity/Object.h"
#include "FrameWork/Controller.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file ContextMenu.h
 * @brief コンテキストメニュー
 */

namespace Entity
{
	struct MenuButton
	{
		int key; //! ボタンの種類識別用のキー
		string text; //! ボタンのテキスト
		int keyCode; //! 入力キーコード (-1は対象なし)
		Shape shape; //! ボタンの矩形座標
		bool isDisabled = false; //! 無効状態になっているか
	};

	class ContextMenu : public Object
	{
	public:
		ContextMenu();
		~ContextMenu() {};

		int checkRunButton(int x, int y, int eventType) const;

		void addMenuButton(int key, string text, int keyCode = -1);
		void clearMenuButton();

		void setDisabledMenuButton(int key, bool isDisabled);

		void setDisplayArea(Shape displayArea);

		void show(int sourceX, int sourceY);
		void show(Shape sourceShape);
		void hide();

		void render() const override;

	private:

		constexpr static int PADDING = 12; //! メニューの余白

		constexpr static int RECT_ROUND = 15; //! 角丸のrx, ry値
		constexpr static int POS_NUM = 16; //! 角丸のpos_num
		
		constexpr static int TEXT_SIZE = 18; //! ボタンテキストサイズ

		constexpr static int BUTTON_MARGIN_BOTTOM = 5; //! ボタンのマージン
		constexpr static int BUTTON_PADDING = 10; //! ボタンの余白
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // ボタン高さ

		vector<MenuButton> menuBtns_; //! メニューボタンリスト

		Shape displayArea_; //! 表示可能エリア

		int width_;
	};
}