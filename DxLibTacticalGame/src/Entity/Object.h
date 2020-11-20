#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Object
	{
	public:
		Object() : 
			isMouseDown_(false), isMouseOver_(false), objectType(OBJECT),
			isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1) {};
		virtual ~Object() {};

		virtual void init(int layerId, int objectId, Shape shape);

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

		virtual void render() const;

		/**
		 * @enum ObjectType
		 * �I�u�W�F�N�g�̎��
		 */
		enum ObjectType {
			OBJECT,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS
		};

		//! Object�̎��
		ObjectType objectType;

	protected:
		/**
		 * @fn
		 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		void onMouseLeftDown() {};
		/**
		 * @fn
		 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		void onMouseLeftUp() {};
		/**
		 * @fn
		 * �N���b�N���ꂽ�u�Ԃ̏���
		 */
		void onMouseClick() {};
		/**
		 * @fn
		 * �}�E�X�Ƀz�o�[���ꂽ�u�Ԃ̏���
		 */
		void onMouseOver() {};
		/**
		 * @fn
		 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
		 */
		void onMouseOut() {};

		/**
		 * @fn
		 * �}�E�X���E�{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		void onMouseRightDown() {};
		/**
		 * @fn
		 * �}�E�X�̉E�{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		void onMouseRightUp() {};

		/**
		 * @fn
		 * �}�E�X���z�C�[���{�^���ŉ������ꂽ�u�Ԃ̏���
		 */
		void onMouseWheelDown() {};
		/**
		 * @fn
		 * �}�E�X�̃z�C�[���{�^���������グ��ꂽ�u�Ԃ̏���
		 */
		void onMouseWheelUp() {};

		void renderExtend() const;

		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;

		//! �}�E�X�ɍ��{�^���ŉ�������Ă��邩�i�Ƃ肠�������{�^�������j
		bool isMouseDown_;

		//! �}�E�X�Ƀz�o�[����Ă��邩
		bool isMouseOver_;

		//! �摜�𓧉߂����邩
		bool isTrans_;

		//! DxLib�œǂݍ��񂾉摜�̃p�X
		int imagePath_;

		//! �������C���[ID
		int layerId_;

		//! �I�u�W�F�N�gID
		int objectId_;
	};
}