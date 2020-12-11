#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/FontManager.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button/CourseButton.h"
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
		constexpr static int PADDING_TOP = 10;		//! �㑤�̗]��
		constexpr static int LEFT_AREA_WIDTH = 800; //! �����̃G���A�̕�

		constexpr static int COURSE_TOP = 70;		//! �R�[�X�ꗗ��Y���W
		constexpr static int COURSE_MARGIN_X = 40;		//! �R�[�X�{�^���̗]��
		constexpr static int COURSE_MARGIN_Y = 30;		//! �R�[�X�{�^���̗]��
		constexpr static int COURSE_COLUMN_NUM = 5;		//! �R�[�X�{�^���̗�

		constexpr static int RIGHT_AREA_PADDING_TOP = 25;	//! �i�E�G���A�́j�㑤�̗]��
		constexpr static int RIGHT_AREA_PADDING_LEFT = 80;	//! (�E�G���A��)�����̗]��

		int selectedCourseId_; //! �I�𒆂̃R�[�XID

		int courseTitle_; //! �R�[�X���\����

		enum Layer
		{
			MASK,
			UI,
			COURSE_BUTTON,
			BACK,
			LEN
		};

		enum UIid
		{
			BACK_BTN
		};
	};
}