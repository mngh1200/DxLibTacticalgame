#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Entity/Object.h"
#include "FrameWork/Controller.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file SelectActiveMenu.h
 * @brief ユニットの行動選択メニュー
 */

namespace Entity
{
	struct MenuButton
	{
		int key; //! ボタンの種類識別用のキー
		string text; //! ボタンのテキスト
		Shape shape; //! ボタンの座標と位置
		int keyCode; //! 入力キーコード
	};

	class SelectActiveMenu : public Object
	{
	public:
		SelectActiveMenu() : isDisplay_(false), menuBtns_{} {};
		~SelectActiveMenu() {};

		enum ButtonKind
		{
			WAIT,
			CANCEL
		};

		int getHitButtonKey(int x, int y);
		int getKeyPressButtonKey() const;

		void start(int unitX, int unitY);
		void end();

		void render() const override;

	private:
		void addMenuButton(int key, string text, int keyCode, int buttonX, int buttonY, int buttonW, int buttonH, int margin);
		MenuButton* getHitMenuButton(int x, int y);


		constexpr static int PADDING = 12; //! メニューの余白
		constexpr static int WIDTH = 230;  //! メニューの幅
		constexpr static int RECT_ROUND = 15; //! 角丸のrx, ry値
		constexpr static int POS_NUM = 16; //! 角丸のpos_num
		constexpr static int TEXT_SIZE = 24; //! ボタンテキストサイズ
		constexpr static int BUTTON_MARGIN = 5; //! ボタンのマージン
		constexpr static int BUTTON_PADDING = 10; //! ボタンの余白
		constexpr static int BUTTON_WIDTH = WIDTH - PADDING * 2; // ボタン幅
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // ボタン高さ
	

		vector<MenuButton> menuBtns_; //! メニューボタンリスト

		bool isDisplay_; //! 表示しているか
	};


}