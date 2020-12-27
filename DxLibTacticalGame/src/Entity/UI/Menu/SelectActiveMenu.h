#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Entity/Object.h"
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
	};

	class SelectActiveMenu : public Object
	{
	public:
		SelectActiveMenu() = delete;
		SelectActiveMenu(int unitX, int unitY);
		~SelectActiveMenu() {};

		enum ButtonKind
		{
			WAIT,
			CANCEL
		};

		void onClickMenu(int x, int y);

		virtual void render() const override;

	private:
		void addMenuButton(int key, string text, int buttonX, int buttonY, int buttonW, int buttonH, int margin);
		MenuButton& getHitMenuButton(int x, int y);

		constexpr static int PADDING = 10; //! メニューの余白
		constexpr static int WIDTH = 100;  //! メニューの幅
		constexpr static int BUTTON_MARGIN = 5; //! ボタンのマージン

		vector<MenuButton> menuBtns_; //! メニューボタンリスト
	};


}