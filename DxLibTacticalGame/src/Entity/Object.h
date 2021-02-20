#pragma once
#include <climits>
#include <string>
#include "Figure.h"
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Object.h
 * @brief �}�E�X�C�x���g�����m�����ʓ��v�f�̒��ۃN���X
 */

namespace Entity
{
	class Object : public Figure
	{
	public:

		Object() : isMouseDown_(false), isMouseOver_(false) { isView_ = false; };
		virtual ~Object() {};

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

	protected:
		/**
		 * @fn
		 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseLeftDown(int x, int y) {};
		/**
		 * @fn
		 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseLeftUp(int x, int y) {};
		/**
		 * @fn
		 * �N���b�N���ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseClick(int x, int y) {};
		/**
		 * @fn
		 * �}�E�X�Ƀz�o�[���ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseOver(int x, int y) {};
		/**
		 * @fn
		 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
		 */
		virtual void onMouseOut(int x, int y) {};

		/**
		 * @fn
		 * �}�E�X���E�{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseRightDown(int x, int y) {};
		/**
		 * @fn
		 * �}�E�X�̉E�{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseRightUp(int x, int y) {};

		/**
		 * @fn
		 * �}�E�X���z�C�[���{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseWheelDown(int x, int y) {};
		/**
		 * @fn
		 * �}�E�X�̃z�C�[���{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseWheelUp(int x, int y) {};

		//! �}�E�X�ɍ��{�^���ŉ�������Ă��邩�i�Ƃ肠�������{�^�������j
		bool isMouseDown_;

		//! �}�E�X�Ƀz�o�[����Ă��邩
		bool isMouseOver_;
	};
}