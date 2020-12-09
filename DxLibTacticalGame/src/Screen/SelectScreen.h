#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/FontManager.h"
#include "Entity/UI/Back.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file SelectScreen.h
 * @brief �L�����y�[���Z���N�g���
 */

namespace Screen
{
	class SelectScreen : public ScreenBase
	{
	public:
		SelectScreen() : courseTitle_(-1) {};
		~SelectScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		const static int PADDING_TOP = 10;		//! �㑤�̗]��
		const static int PADDING_LEFT = 25;		//! �����̗]��
		const static int LEFT_AREA_WIDTH = 800; //! �����̃G���A�̕�

		const static int COURSE_TOP = 70;		//! �R�[�X�ꗗ��Y���W
		const static int COURSE_SIZE = 100;		//! �R�[�X�{�^���̕��ƍ���
		const static int COUST_MARGIN = 45;		//! �R�[�X�{�^���̗]��

		const static int RIGHT_AREA_PADDING_TOP = 25;	//! �i�E�G���A�́j�㑤�̗]��
		const static int RIGHT_AREA_PADDING_LEFT = 80;	//! (�E�G���A��)�����̗]��

		int courseTitle_; // �R�[�X���\����








		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		enum UIid
		{
			BACK_BTN
		};
	};
}