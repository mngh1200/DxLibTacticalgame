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
 * @brief ���j�b�g�̍s���I�����j���[
 */

namespace Entity
{
	struct MenuButton
	{
		int key; //! �{�^���̎�ގ��ʗp�̃L�[
		string text; //! �{�^���̃e�L�X�g
		Shape shape; //! �{�^���̍��W�ƈʒu
		int keyCode; //! ���̓L�[�R�[�h
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


		constexpr static int PADDING = 12; //! ���j���[�̗]��
		constexpr static int WIDTH = 230;  //! ���j���[�̕�
		constexpr static int RECT_ROUND = 15; //! �p�ۂ�rx, ry�l
		constexpr static int POS_NUM = 16; //! �p�ۂ�pos_num
		constexpr static int TEXT_SIZE = 24; //! �{�^���e�L�X�g�T�C�Y
		constexpr static int BUTTON_MARGIN = 5; //! �{�^���̃}�[�W��
		constexpr static int BUTTON_PADDING = 10; //! �{�^���̗]��
		constexpr static int BUTTON_WIDTH = WIDTH - PADDING * 2; // �{�^����
		constexpr static int BUTTON_HEIGHT = TEXT_SIZE + BUTTON_PADDING * 2; // �{�^������
	

		vector<MenuButton> menuBtns_; //! ���j���[�{�^�����X�g

		bool isDisplay_; //! �\�����Ă��邩
	};


}