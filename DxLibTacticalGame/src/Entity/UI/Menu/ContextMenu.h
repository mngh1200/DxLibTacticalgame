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
 * @brief �R���e�L�X�g���j���[
 */

namespace Entity
{
	struct MenuButton
	{
		int key; //! �{�^���̎�ގ��ʗp�̃L�[
		string text; //! �{�^���̃e�L�X�g
		int keyCode; //! ���̓L�[�R�[�h (-1�͑ΏۂȂ�)
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

		constexpr static int PADDING = 12; //! ���j���[�̗]��

		constexpr static int RECT_ROUND = 15; //! �p�ۂ�rx, ry�l
		constexpr static int POS_NUM = 16; //! �p�ۂ�pos_num
		
		constexpr static int TEXT_SIZE = 24; //! �{�^���e�L�X�g�T�C�Y

		constexpr static int BUTTON_MARGIN = 5; //! �{�^���̃}�[�W��
		constexpr static int BUTTON_PADDING = 10; //! �{�^���̗]��
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // �{�^������

		vector<MenuButton> menuBtns_; //! ���j���[�{�^�����X�g

		Shape displayArea_; //! �\���\�G���A

		bool isDisplay_; //! �\�����Ă��邩
	};
}