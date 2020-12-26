#include "Figure.h"
#include "FrameWork/Game.h"

namespace Entity {
	/**
	 * @fn
	 * ����������
	 * @param (layerId) �������C���[
	 * @param (objectId) �����I�u�W�F�N�g
	 * @param (shape) �ʒu�A�T�C�Y
	 */
	void Figure::init(int layerId, int objectId)
	{
		layerId_ = layerId;
		objectId_ = objectId;
	}

	/**
	 * @fn
	 * �A�j���[�V�����̒ǉ��\��
	 * @return �ǉ��ς݂̏ꍇ false
	 */
	bool Figure::isAnimation() const
	{
		return animationId_ != -1;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g���g��ObjectsControl�N���X�̃A�j���[�V�����I�u�W�F�N�g���X�g�ɒǉ�����
	 * @param (animationId) �Z�b�g����A�j���[�V����ID
	 */
	void Figure::joinAnimationList(int animationId)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(animationId, getLayerId(), getObjectId(), isView_);
	}

	/**
	 * @fn
	 * �A�j���[�V������ύX����
	 * @param (animationId) �Z�b�g����A�j���[�V����ID
	 */
	void Figure::changeAnimation(int animationId)
	{
		if (isAnimation())
		{
			animationId_ = animationId;
			createAnimation(animationId_);
		}
		else
		{
			joinAnimationList(animationId);
		}
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜(�\��)
	 */
	void Figure::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g���폜�\�񂳂�Ă��邩����
	 * @return �폜����ꍇtrue
	 */
	bool Figure::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * �������C���[ID���擾
	 * @return �������C���[ID
	 */
	int Figure::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int Figure::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int Figure::getType() const
	{
		return type_;
	}
}