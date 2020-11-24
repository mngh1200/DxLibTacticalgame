#pragma once
#include <climits>
#include <string>
#include "Figure.h"
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Object : public Figure
	{
	public:

		Object() : isMouseDown_(false), isMouseOver_(false) {};
		virtual ~Object() {};

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

	protected:
		/**
		 * @fn
		 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseLeftDown() {};
		/**
		 * @fn
		 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseLeftUp() {};
		/**
		 * @fn
		 * �N���b�N���ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseClick() {};
		/**
		 * @fn
		 * �}�E�X�Ƀz�o�[���ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseOver() {};
		/**
		 * @fn
		 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
		 */
		virtual void onMouseOut() {};

		/**
		 * @fn
		 * �}�E�X���E�{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseRightDown() {};
		/**
		 * @fn
		 * �}�E�X�̉E�{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseRightUp() {};

		/**
		 * @fn
		 * �}�E�X���z�C�[���{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseWheelDown() {};
		/**
		 * @fn
		 * �}�E�X�̃z�C�[���{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		virtual void onMouseWheelUp() {};

		//! �}�E�X�ɍ��{�^���ŉ�������Ă��邩�i�Ƃ肠�������{�^�������j
		bool isMouseDown_;

		//! �}�E�X�Ƀz�o�[����Ă��邩
		bool isMouseOver_;
	};
}