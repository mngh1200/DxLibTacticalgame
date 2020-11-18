#pragma once
#include <climits>
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class ViewObject
	{
	public:
		ViewObject() : objectType(ViewObject::ObjectType::VIEW), shape_(Shape(0,0,0,0)) {};
		ViewObject(Shape shape) : objectType(ViewObject::ObjectType::VIEW), shape_(shape) {}
		virtual ~ViewObject() {};

		virtual void render() const;

		/**
		 * @enum ObjectType
		 * �I�u�W�F�N�g�̎��
		 */
		enum ObjectType {
			VIEW,
			TEXT
		};

		//! ViewObject�̎��
		ObjectType objectType;

	protected:
		//! �I�u�W�F�N�g�̌`��(�ʒu��T�C�Y)
		Shape shape_;
	};
}