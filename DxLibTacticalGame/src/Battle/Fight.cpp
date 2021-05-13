#include "Fight.h"
#include "Screen/BattleScreen.h"

namespace Battle
{
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^���擾
	 */
	void Fight::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
 * @fn
	 * �U�����ꂽ�������擾
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (atkUnit) �U�������j�b�g
	 * @return ����
	 */
	int getAtkDirection(shared_ptr<Unit> defUnit, int atkUnitX, int atkUnitY)
	{
		int atkDirection = Direction::NONE_DIRECTION; // �U�����ꂽ����

		int defUnitX = defUnit->getMassX();
		int defUnitY = defUnit->getMassY();

		if (atkUnitX == defUnitX) // �㉺�̂ǂ��炩
		{
			if (atkUnitY > defUnitY)
			{
				atkDirection = Direction::BOTTOM;
			}
			else if (atkUnitY < defUnitY)
			{
				atkDirection = Direction::TOP;
			}
		}
		else if (atkUnitY == defUnitY)
		{
			if (atkUnitX > defUnitX)
			{
				atkDirection = Direction::LEFT;
			}
			else if (atkUnitX < defUnitX)
			{
				atkDirection = Direction::RIGHT;
			}
		}
		return atkDirection;
	}

	/**
	 * @fn
	 * �A�g�A��������
	 * @param (atkDirection) �U�����ꂽ����
	 * @param (atkedLogs) �U�����ꂽ�����̗���
	 * @return �A�g�A�������茋��
	 */
	int Fight::getCoordinatedAttack(int atkDirection, int atkedLogs)
	{
		int result = CoordinatedAttack::NONE;

		if (atkDirection == Direction::TOP || atkDirection == Direction::BOTTOM)
		{
			int tmpLog = atkedLogs & (Direction::TOP | Direction::BOTTOM); // ���Ε����̍U�����������i���j
			if (tmpLog != 0 && tmpLog != atkDirection) // ��������
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::LEFT | Direction::RIGHT)) // �A�g����
			{
				return CoordinatedAttack::COORDINATED;
			}
		}
		else if (atkDirection == Direction::LEFT || atkDirection == Direction::RIGHT)
		{
			int tmpLog = atkedLogs & (Direction::LEFT | Direction::RIGHT); // ���Ε����̍U�����������i���j
			if (tmpLog != 0 && tmpLog != atkDirection) // ��������
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::TOP | Direction::BOTTOM)) // �A�g����
			{
				return CoordinatedAttack::COORDINATED;
			}
		}

		return result;
	}

	/**
	 * @fn
	 * �퓬�\��
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @return ���e���X�V���ꂽ�ꍇ��true��Ԃ�
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit)
	{
		return setPrepare(actUnit, actUnit->getMassX(), actUnit->getMassY(), psvUnit);
	}

	/**
	 * @fn
	 * �퓬�\��
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (actMassX) �U�����d�|����}�X���WX
	 * @param (actMassY) �U�����d�|����}�X���WY
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @return ���e���X�V���ꂽ�ꍇ��true��Ԃ�
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, int actMassX, int actMassY, shared_ptr<Unit> psvUnit)
	{
		if (actUnit && psvUnit)
		{
			if (actUnit == actSide_.unit && psvSide_.unit == psvUnit)
			{
				return false; // �Ώۂ��ς���Ă��Ȃ��ꍇ
			}

			// ���̗\���_���[�W�\�����\��
			hidePredictUnitDamage();

			// ������ʈꗗ�N���A
			actSide_.extraEffects.clear();
			psvSide_.extraEffects.clear();

			// shared_ptr<Mass> mass = map_->getMass(psvUnit->getMassX(), psvUnit->getMassY()); // �h�䑤�̃}�X
			actSide_.isFirst = true;

			UnitInfo actInfo = actUnit->getInfo();
			// UnitInfo psvInfo = psvUnit->getInfo();

			// �U������
			actSide_.direction = getAtkDirection(psvUnit, actMassX, actMassY);

			// �A�g�A��������
			int coordinatedAttack = CoordinatedAttack::NONE;
			if (actInfo.isCloseAtack)
			{
				coordinatedAttack = getCoordinatedAttack(actSide_.direction, psvUnit->getCloseAttackLogs());
			}

			makeFightData(&psvSide_, psvUnit, actUnit, false, coordinatedAttack); // �h�䑤�v�Z (�}�����肪���邽�ߐ�ɏ���)

			if (psvSide_.isFirst)
			{
				actSide_.isFirst = false;
			}

			makeFightData(&actSide_, actUnit, psvUnit, true, coordinatedAttack);  // �U�����v�Z

			// �A�g�A�����̃_���[�W�v�Z
			if (coordinatedAttack == CoordinatedAttack::COORDINATED) // �A�g (�_���[�W�{�� 1.5)
			{
				actSide_.damage = actSide_.damage * 3 / 2;
				actSide_.extraEffects.push_back("�A�g");
			}
			else if (coordinatedAttack == CoordinatedAttack::PINCHING) // ���� (�_���[�W�{�� 2)
			{
				actSide_.damage = actSide_.damage * 2;
				actSide_.extraEffects.push_back("����");
			}
			

			return true;
		}
		return false;
	}


	/**
	 * @fn
	 * ���Z�b�g
	 */
	void Fight::reset()
	{
		hidePredictUnitDamage();
		actSide_ = { nullptr, true };
		psvSide_ = { nullptr, false };
		phase_ = Phase::NO_FIGHT;
	}

	/**
	 * @fn
	 * �U���̖����󋵂��Z�b�g
	 * @param (actHitState) �U�������������̖�����
	 * @param (psvHitState) �U�����ꂽ���̖�����
	 */
	void Fight::setHitState(int actHitState, int psvHitState)
	{
		if (actHitState != FightData::HitState::UNSETTLED)
		{
			actSide_.hitState = actHitState;
		}

		if (psvHitState != FightData::HitState::UNSETTLED)
		{
			psvSide_.hitState = psvHitState;
		}
	}

	/**
	 * @fn
	 * FightData����
	 * @param (fightData) ��������FightData�擾�p
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @param (mass) �U�����d�|����ꂽ���̂���}�X
	 * @param (isAct) �U���\��
	 * @param (coordinatedAttack) �A�g�A�������茋��
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, bool isAct, int coordinatedAttack)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		UnitInfo atkInfo = atkUnit->getInfo();
		UnitInfo defInfo = defUnit->getInfo();

		shared_ptr<Mass> mass = map_->getMass(defUnit->getMassX(), defUnit->getMassY());

		// �}���X�L������
		if (!isAct && atkInfo.ability.kind == Ability::Kind::AMBUSH && coordinatedAttack == CoordinatedAttack::NONE)
		{
			if (atkInfo.len > defInfo.len)
			{
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
				fightData->isAtk = true;
				fightData->isFirst = true;
			}
		}

		if (fightData->isAtk) // �U������ꍇ
		{
			// �ߐڍU������
			if (atkInfo.isCloseAtack)
			{
				fightData->isCloseAttack = true;
				fightData->extraEffects.push_back("�ߐ�");
			}
			else
			{
				fightData->isCloseAttack = false;
				fightData->extraEffects.push_back("�ˌ�");
			}

			// �ȉ��A�_���[�W�v�Z
			int def = defInfo.def + mass->getDef();

			// �ђʃX�L��
			if (atkInfo.ability.kind == Ability::Kind::THROUGH)
			{
				def = 0;
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
			}

			fightData->damage = atkInfo.atk - def;
			fightData->hitRate = 100 - mass->getAgl();

			// ��������
			int distance = Map::getMassDistance(atkUnit->getMassX(), atkUnit->getMassY(), defUnit->getMassX(), defUnit->getMassY());
			fightData->hitRate -= (distance - 1) * Gunner::ATTENUATION;

			if (fightData->hitRate < 0)
			{
				fightData->hitRate = 0;
			}
			else if (fightData->hitRate > 100)
			{
				fightData->hitRate = 100;
			}

			// �ˌ��X�L��
			if (isAct && isActSideFirst())
			{
				if (actSide_.unit->getInfo().ability.kind == Ability::Kind::RUSH)
				{
					if (isActSideFirst() && mass->getKind() == Mass::PLAIN)
					{
						actSide_.extraEffects.push_back(actSide_.unit->getInfo().ability.getLabel());
						actSide_.damage += 5;
					}
				}
			}
		}
		else // �U�����Ȃ��ꍇ
		{
			fightData->damage = 0;
			fightData->hitRate = 0;
		}

		// ��������
		if (fightData->hitRate > DxLib::GetRand(99))
		{
			fightData->hitState = FightData::HitState::HITTED;
		}
		else
		{
			fightData->hitState = FightData::HitState::MISS;
		}
		
	}

	/**
	 * @fn
	 * �퓬�J�n
	 */
	void Fight::start()
	{
		if (isPrepared())
		{
			phase_ = Phase::FIRST_EFFECT;

			FightData atkSide;
			FightData defSide;
			getAttackState(isActSideFirst(), atkSide, defSide);
			showExtraEffect(&atkSide, &defSide);

			// atack(isActSideFirst());
		}
	}


	/**
	 * @fn
	 * �퓬���̍X�V����
	 * @return �I������true��Ԃ�
	 */
	bool Fight::checkUpdate()
	{
		if (!psvSide_.unit && !actSide_.unit) // �Ώۃ��j�b�g�����݂��Ȃ��ꍇ
		{
			return true;
		}

		
		if (phase_ == Phase::FIRST_ATK || phase_ == Phase::SECOND_ATK) // ���j�b�g�A�j���[�V�������̏���
		{
			// �A�j���[�V�����p��
			if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
			{
				return false;
			}
			
			// �A�j���[�V�����I����
			if (phase_ == Phase::FIRST_ATK)
			{
				FightData atkSide;
				FightData defSide;
				if (getAttackState(!isActSideFirst(), atkSide, defSide)) // �U�����ꂽ���̍U�������邩
				{
					showExtraEffect(&atkSide, &defSide);
					phase_ = Phase::SECOND_EFFECT;
					return false;
				}				
			}
		}
		if (phase_ == Phase::FIRST_EFFECT || phase_ == Phase::SECOND_EFFECT) // ������ʃG�t�F�N�g���̏���
		{
			// �A�j���[�V�������̓���G�t�F�N�g�����邩
			for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
			{
				if ((*itr)->isAnimation())
				{
					return false;
				}
			}

			// ����G�t�F�N�g�I����
			FightData atkSide, defSide;
			bool isFirstAttack = true; //! �U�����d�|�������̍U��

			if (phase_ == Phase::SECOND_EFFECT)
			{
				isFirstAttack = false;
			}


			if (getAttackState(isActSideFirst() == isFirstAttack, atkSide, defSide)) // �U���\��
			{
				attack(&atkSide, &defSide);

				if (isFirstAttack)
				{
					phase_ = Phase::FIRST_ATK;
				}
				else
				{
					phase_ = Phase::SECOND_ATK;
				}

				return false;
			}
		}

		// ������ʃG�t�F�N�g���\����
		for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
		{
			(*itr)->hide();
		}

		// �퓬�I��
		actSide_.unit->endAction();

		// reset();
		return true;
	}

	/**
	 * @fn
	 * ���j�b�g�̗\���_���[�W�ʂ�\��
	 */
	void Fight::showPredictUnitDamage()
	{
		if (actSide_.unit)
		{
			actSide_.unit->setPredictDamage(psvSide_.damage);
		}
		if (psvSide_.unit)
		{
			psvSide_.unit->setPredictDamage(actSide_.damage);
		}
	}

	/**
	 * @fn
	 * ���j�b�g�̗\���_���[�W�ʂ��\��
	 */
	void Fight::hidePredictUnitDamage()
	{
		if (actSide_.unit)
		{
			actSide_.unit->clearPredictDamage();
		}
		if (psvSide_.unit)
		{
			psvSide_.unit->clearPredictDamage();
		}
	}

	/**
	 * @fn
	 * �U�����s
	 * @param (isActSideAtack) �h�䑤�̍U���ł��邩
	 * @return �U�����s�L��
	 */
	void Fight::attack(FightData* atkSide, FightData* defSide)
	{
		// �U�����s
		atkSide->unit->atack(defSide->unit->getX(), defSide->unit->getY());

		// �������� (hitState == HITTED�̏ꍇ�͊m�薽���AhitState == MISS�̏ꍇ�͊m��~�X)
		if (atkSide->hitState != FightData::HitState::MISS)
		{
			// atkSide->hitState = FightData::HitState::HITTED;

			// �_���[�W
			if (defSide->unit->damage(atkSide->damage, atkSide->direction, atkSide->isCloseAttack))
			{
				// ���S������
				map_->eraseUnit(defSide->unit);
			}
		}
		else
		{
			//atkSide->hitState = FightData::HitState::MISS;
			defSide->unit->avoid(); // ���
		}
	}

	/**
	 * @fn
	 * �U���̏󋵎擾
	 */
	bool Fight::getAttackState(bool isActSideAtack, FightData& atkSide, FightData& defSide)
	{
		if (isActSideAtack) // �U���d�|�������̍U��
		{
			atkSide = actSide_;
			defSide = psvSide_;
		}
		else // �U���d�|����ꂽ���̍U��
		{
			atkSide = psvSide_;
			defSide = actSide_;
		}

		// �U���s����
		if (!atkSide.isAtk || atkSide.unit->getInfo().hp <= 0)
		{
			return false;
		}

		return true;
	}

	/**
	 * @fn
	 * �U�����s
	 * @param (isActSideAtack) �h�䑤�̍U���ł��邩
	 * @return �U�����s�L��
	 */
	void Fight::showExtraEffect(FightData* atkSide, FightData* defSide)
	{
		// ������ʕ\��
		clearExtraEffect();
		int num = 0;
		for (auto itr = atkSide->extraEffects.begin(); itr != atkSide->extraEffects.end(); ++itr)
		{
			if ("�ߐ�" == (*itr) || "�ˌ�" == (*itr))
			{
				continue; // ������ʂłȂ����͖̂���
			}

			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<ExtraEffect> extraEffect = make_shared<ExtraEffect>();
			objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, extraEffect);
			extraEffect->initExtraEffect(atkSide->unit, defSide->unit, (*itr).c_str(), num);
			extraEffectList_.push_back(extraEffect);
			++num;
		}
	}

	/**
	 * @fn
	 * �����̓�����ʕ\�L�N���A
	 */
	void Fight::clearExtraEffect()
	{
		for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
		{
			(*itr)->destroy();
		}
		extraEffectList_.clear();
	}


	/**
	 * @fn
	 * FightData�擾
	 * @param (isAct) true: �U�����d�|�������Afalse: �U�����d�|����ꂽ��
	 * @return FightData
	 */
	const FightData& Fight::getFightData(bool isAct) const
	{
		if (isAct)
		{
			return actSide_;
		}

		return psvSide_;
	}

	/**
	 * @fn
	 * �퓬�\���ς݂ł��邩
	 * @return �\���ς݂̏ꍇtrue��Ԃ�
	 */
	bool Fight::isPrepared() const
	{
		return actSide_.unit != nullptr;
	}
}

