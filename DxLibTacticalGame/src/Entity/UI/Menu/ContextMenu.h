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
	};

	class ContextMenu : public Object
	{
	public:
		ContextMenu() : isDisplay_(false), menuBtns_{}, displayArea_{ Shape(0, 0, WIN_W, WIN_H) } {};
		~ContextMenu() {};

		int checkRunButton();

		void addMenuButton(int key, string text, int keyCode = -1);
		void clearMenuButton();

		void setDisplayArea(Shape displayArea);

		void start(int sourceX, int sourceY);
		void start(Shape sourceShape);
		void hide();

		void render() const override;

		void onMouseClick() override;

	private:
		MenuButton* getHitMenuButton(int x, int y);

		constexpr static int PADDING = 12; //! メニューの余白

		constexpr static int RECT_ROUND = 15; //! 角丸のrx, ry値
		constexpr static int POS_NUM = 16; //! 角丸のpos_num
		
		constexpr static int TEXT_SIZE = 24; //! ボタンテキストサイズ

		constexpr static int BUTTON_MARGIN = 5; //! ボタンのマージン
		constexpr static int BUTTON_PADDING = 10; //! ボタンの余白
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // ボタン高さ

		vector<MenuButton> menuBtns_; //! メニューボタンリスト

		Shape displayArea_; //! 表示可能エリア

		bool isDisplay_; //! 表示しているか
	};
}