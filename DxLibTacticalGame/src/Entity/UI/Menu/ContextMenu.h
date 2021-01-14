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
		Shape shape; //! �{�^���̋�`���W
		bool isDisabled = false; //! ������ԂɂȂ��Ă��邩
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

		constexpr static int PADDING = 12; //! ���j���[�̗]��

		constexpr static int RECT_ROUND = 15; //! �p�ۂ�rx, ry�l
		constexpr static int POS_NUM = 16; //! �p�ۂ�pos_num
		
		constexpr static int TEXT_SIZE = 18; //! �{�^���e�L�X�g�T�C�Y

		constexpr static int BUTTON_MARGIN_BOTTOM = 5; //! �{�^���̃}�[�W��
		constexpr static int BUTTON_PADDING = 10; //! �{�^���̗]��
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // �{�^������

		vector<MenuButton> menuBtns_; //! ���j���[�{�^�����X�g

		Shape displayArea_; //! �\���\�G���A

		int width_;
	};
}