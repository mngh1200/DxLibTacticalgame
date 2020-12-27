#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file SelectActiveMenu.h
 * @brief ���j�b�g�̍s���I�����j���[
 */

namespace Entity
{
	struct MenuButton
	{
		int key; //! �{�^���̎�ގ��ʗp�̃L�[
		string text; //! �{�^���̃e�L�X�g
		Shape shape; //! �{�^���̍��W�ƈʒu
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

		constexpr static int PADDING = 10; //! ���j���[�̗]��
		constexpr static int WIDTH = 100;  //! ���j���[�̕�
		constexpr static int BUTTON_MARGIN = 5; //! �{�^���̃}�[�W��

		vector<MenuButton> menuBtns_; //! ���j���[�{�^�����X�g
	};


}