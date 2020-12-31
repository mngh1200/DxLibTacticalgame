#include "Mass.h"
#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	const vector<int> Mass::COST = { 1, 2, 2, INT_MAX };
	const vector<int> Mass::AGL_UP = { 0, 20, -20, 0 };
	const vector<int> Mass::DEF_UP = { 0,  3,  -3, 0 };
	const vector<string> Mass::TEXT1 = {"����: ���ɂȂ�", "�X: �R����̉�", "��:", "�R: �N���s��"};

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Mass::Mass(int kindId) : kindId_(kindId), state(State::NORMAL), passingMov(-1)
	{
		if (0 > kindId_ || kindId_ >= Kind::LEN)
		{
			kindId_ = Kind::PLAIN;
		}

		setImageIdFromKind(kindId);
	}

	/**
	 * @fn
	 * �ړ��R�X�g�擾
	 */
	int Mass::getCost() const
	{
		try
		{
			return COST.at(kindId_);
		}
		catch (out_of_range&) {}
		
		return INT_MAX;
	}

	/**
	 * @fn
	 * ���UP�擾
	 */
	int Mass::getAgl() const
	{
		try
		{
			return AGL_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;
	}

	/**
	 * @fn
	 * ���UP�擾
	 */
	int Mass::getDef() const
	{
		try
		{
			return DEF_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;

	}

	/**
	 * @fn
	 * �}�X�̎�ނ�Ԃ�
	 */
	int Mass::getKind() const
	{
		return kindId_;
	}

	/**
	 * @fn
	 * �摜��ID��Ԃ�
	 */
	int Mass::getImageId() const
	{
		return imageId_;
	}

	/**
	 * @fn
	 * �n�`���ʐ�������Ԃ�
	 * @param (kindId) �摜�̎��
	 */
	string Mass::getText(int line) const
	{
		if (kindId_ == Kind::OUT_OF_MAP)
		{
			return "";
		}

		if (line == 1)
		{
			return TEXT1.at(kindId_);
		}
		else if (line == 2)
		{
			int agl = getAgl();
			int def = getDef();

			if (agl == 0 && def == 0)
			{
				return "";
			}
			string aglSign = agl >= 0 ? "+" : "";
			string defSign = def >= 0 ? "+" : "";

			return "��� " + aglSign + to_string(agl) + " �h�� " + defSign + to_string(def);
		}

		return "";
	}

	/**
	 * @fn
	 * �摜�̎�ނ����ɉ摜ID���Z�b�g
	 * @param (kindId) �摜�̎��
	 */
	void Mass::setImageIdFromKind(int kindId)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �e�X�g����
		if (kindId == Kind::MOUNTAIN) // �R
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 0);
		}
		else if (kindId == Kind::FOREST) // �X
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 2);
		}
		else if (kindId == Kind::RIVER) // ��
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 3);
		}
		else // ����
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 1);
		}
	}
}