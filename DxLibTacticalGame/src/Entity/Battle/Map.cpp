#include "Map.h"
#include "Entity/Unit/Gunner.h"
#include "Screen/BattleScreen.h"

namespace Entity {
	/**
     * @fn
     * �R���X�g���N�^
     */
	Map::Map()
	{
		type_ = Object::Type::MAP;

		shape_.set(0, MAP_Y, MAP_MASS_W * CHIP_SIZE, MAP_MASS_H * CHIP_SIZE);
	}

	/**
	 * @fn
	 * �}�b�v�f�[�^�ǂݍ���
	 */
	void Map::loadStageData(std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H >& mapData)
	{
		mass_.reserve(MAP_MASS_H); // �������m��

		int y = 0;
		int x = 0;
		for (auto yItr = mapData.begin(); yItr != mapData.end(); ++yItr)
		{
			auto line = *yItr;
			mass_.push_back(vector<shared_ptr<Mass>>());
			mass_[y].reserve(MAP_MASS_W);
			for (auto xItr = line.begin(); xItr != line.end(); ++xItr)
			{
					int kind = *xItr;
					if (kind >= Mass::Kind::LEN) {
						kind = Mass::Kind::OUT_OF_MAP;
					}
					else if (kind == Mass::Kind::FORT_PLAYER) // �����Ԏw��
					{
						playerFortMass_ = make_pair(x, y);
					}
					else if (kind == Mass::Kind::FORT_ENEMY) // �G�Ԏw��
					{
						enemyFortMass_ = make_pair(x, y);
					}
					mass_[y].push_back(make_shared<Mass>(kind));
					++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * ���j�b�g�f�[�^�Q�̓ǂݍ��݁i�匳�̓X�e�[�W�f�[�^�j
	 * @param (units) ���j�b�g�f�[�^�ix���W, y���W, ��ށA����(0)���G��(1)�j
	 */
	void Map::loadUnits(vector<vector<int>>& units)
	{
		for (auto itr = units.begin(); itr != units.end(); ++itr)
		{
			auto unit = *itr;

			if (unit.size() > 3)
			{
				setUnit(unit[0], unit[1], unit[2], unit[3] == 1);
			}
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void Map::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int x = 0;
		int y = 0;

		int mouseX = -1;
		int mouseY = -1;

		if (isMouseOver_) // �z�o�[���̂ݎ擾
		{
			DxLib::GetMousePoint(&mouseX, &mouseY);
			mouseX = getMassX(mouseX);
			mouseY = getMassY(mouseY);
		}

		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				int realX = getRealX(x);
				int realY = getRealY(y);
				DxLib::DrawGraph(realX, realY, (*cell)->getImageId(), FALSE);

				
				if (isMouseOver_) // �z�o�[���̕`��
				{
					if (x == mouseX && y == mouseY)
					{
						drawHoverMass(realX, realY);
					}
				}

				if ((*cell)->state == Mass::State::NORMAL)
				{
					// �������Ȃ�
				}
				if ((*cell)->state == Mass::State::MOVABLE) // �ړ��͈�
				{
					drawMoveableMass(realX, realY);
				}
				else if ((*cell)->state == Mass::State::ATK_ABLE) // �U���\�͈�
				{
					drawAtackMass(realX, realY);
				}
				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * ����}�X�Ƀ��j�b�g�ݒu
	 * @param (massX) �}�X���Wx
	 * @param (massY) �}�X���Wy
	 * @param (kind) ���j�b�g���
	 * @param (isEnemy) false: �v���C���[���j�b�g, true: �G���j�b�g
	 */
	void Map::setUnit(int massX, int massY, int kind, bool isEnemy)
	{

		if (isRange(massX, massY))
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<Entity::Unit> unit;
			
			if (kind == UnitKey::GUNNER)
			{
				unit = make_shared<Entity::Gunner>();
			}
			else
			{
				unit = make_shared<Entity::Unit>();
			}

			unit->init(massX, massY, kind, isEnemy);

			units_.emplace(make_pair(massX, massY), unit); // �V�K�ǉ��̂�

			if (!isEnemy)
			{
				objectsControl.addObject(Screen::BattleScreen::Layer::PLAYER_UNIT, unit);
			}
			else
			{
				objectsControl.addObject(Screen::BattleScreen::Layer::ENEMY_UNIT, unit);
			}
		}
	}

	/**
	 * @fn
	 * ����}�X�̃��j�b�g�擾
	 * @param (massX) �}�X���WX
	 * @param (massY) �}�X���WY
	 */
	shared_ptr<Unit> Map::getUnit(int massX, int massY)
	{
		pair<int, int> pos = make_pair(massX, massY);
		if (units_.count(pos) > 0)
		{
			return units_.at(pos);
		}

		return nullptr;
	}

	/**
	 * @fn
	 * �Ώۃ��j�b�g�����W�ォ��폜
	 * @param (unit) �Ώۃ��j�b�g
	 */
	void Map::eraseUnit(shared_ptr<Unit> unit)
	{
		units_.erase(make_pair(unit->getMassX(), unit->getMassY()));
	}


	/**
	 * @fn
	 * �}�X�̃C�e���[�^�̎n�_��Ԃ�
	 * @return �}�X�̃C�e���[�^�̎n�_
	*/
	vector<vector<shared_ptr<Mass>>>::iterator Map::massBegin()
	{
		return mass_.begin();
	}

	/**
	 * @fn
	 * �}�X�̃C�e���[�^�̏I�_��Ԃ�
	 * @return �}�X�̃C�e���[�^�̏I�_
	*/
	vector<vector<shared_ptr<Mass>>>::iterator Map::massEnd()
	{
		return mass_.end();
	}

	/**
	 * @fn
	 * ���j�b�g���X�g�̃C�e���[�^�̎n�_��Ԃ�
	 * @return ���j�b�g���X�g�̃C�e���[�^�̎n�_
	*/
	map<pair<int, int>, shared_ptr<Unit>>::iterator Map::unitsBegin()
	{
		return units_.begin();
	}

	/**
	 * @fn
	 * ���j�b�g���X�g�̃C�e���[�^�̏I�_��Ԃ�
	 * @return ���j�b�g���X�g�̃C�e���[�^�̏I�_
	*/
	map<pair<int, int>, shared_ptr<Unit>>::iterator Map::unitsEnd()
	{
		return units_.end();
	}

	/**
	 * @fn
	 * ���j�b�g�̃}�X�ړ��i�ړ��m�莞�j
	 * @param (unit) �Ώۃ��j�b�g
	*/
	void Map::confirmMove(shared_ptr<Unit> unit)
	{
		int baseX = unit->getBaseX();
		int baseY = unit->getBaseY();
		int massX = unit->getMassX();
		int massY = unit->getMassY();

		if (baseX != massX || baseY != massY) // �ړ����Ă���Ƃ��̂�
		{
			unit->setPos(massX, massY);
			units_.emplace(make_pair(massX, massY), unit);
			units_.erase(make_pair(baseX, baseY));
		}
	}

	/**
	 * @fn
	 * �ړ��\�͈͕\��
	 * @param (unit) �Ώۃ��j�b�g
	*/
	void Map::displayMovableRange(shared_ptr<Unit> unit)
	{
		if (unit)
		{
			bool isOwnUnit = !unit->isEnemy(); // ���R���j�b�g�ł��邩

			int move = unit->getInfo().mov;
			int x = unit->getMassX();
			int y = unit->getMassY();

			if (isOwnUnit)
			{
				// �����ʒu����̍U���͈͕\��
				displayAtackAbleRange(unit, x, y, true);
			}

			vector<pair<int, int>> posList;
			getMovableMasses(unit, x, y, move, posList, true);

			if (isOwnUnit) // ���R���j�b�g
			{
				for (auto itr = posList.begin(); itr != posList.end(); ++itr)
				{
					getMass(itr->first, itr->second)->state = Mass::State::MOVABLE; // �ړ��\�G���A�\��
				}
			}
			else // �G�R���j�b�g�̏ꍇ
			{
				for (auto itr = posList.begin(); itr != posList.end(); ++itr)
				{
					displayAtackAbleRange(unit, itr->first, itr->second); // �U���\�G���A�\��
				}
			}
			
		}
	}

	/**
	 * @fn
	 * �ړ��\�͈͂̒T��
	 * @param (unit) �Ώۃ��j�b�g
	 * @param (x) �}�X���WX
	 * @param (y) �}�X���WY
	 * @param (move) �c�ړ��R�X�g
	 * @param (isInit) �ŏ��̒T���ł��邩
	 * @param (isAtackAbleArea) �U���\�͈͂̒T���ł��邩
	*/
	void Map::getMovableMasses(shared_ptr<Unit> unit, int x, int y, int move, vector<pair<int, int>> &posList,  bool isInit)
	{
		shared_ptr<Mass> nowMass = getMass(x, y);

		// �}�b�v�O
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		shared_ptr<Unit> massUnit = getUnit(x, y);
		bool isOwnUnitOnMass = false; // �������j�b�g���}�X��ɑ��݂��邩

		
		if (!isInit) // ����ȊO�̏���
		{
			// �}�X��Ƀ��j�b�g������ꍇ(���g�͖���)
			if (massUnit)
			{
				if (massUnit->isEnemy() != unit->isEnemy()) // �G�R���j�b�g������ꍇ�͖���
				{
					return;
				}
				isOwnUnitOnMass = true;  // �������j�b�g������
			}

			// mov�R�X�g����(����̓R�X�g����Ȃ�)
			move = move - nowMass->getCost();
		}

		if (move > nowMass->passingMov)
		{
			// ���R�̕ʃ��j�b�g�����ɂ���ꍇ�́A�ړ��Ώۃ}�X�ɂ��Ȃ�
			if (!isOwnUnitOnMass)
			{
				posList.push_back(make_pair(x, y));
			}

			// �}�X�ʉߎ���mov�R�X�g��ێ�
			nowMass->passingMov = move;

			getMovableMasses(unit, x - 1, y, move, posList, false);
			getMovableMasses(unit, x + 1, y, move, posList, false);
			getMovableMasses(unit, x, y - 1, move, posList, false);
			getMovableMasses(unit, x, y + 1, move, posList, false);
		}
	}

	/**
	 * @fn
	 * ���ݒn�̍U���\�͈͕\��
	 * @param (unit) �Ώۃ��j�b�g
	 * @param (x) �U���N�_�}�X���WX
	 * @param (y) �U���N�_�}�X���WY
	 * @param (isExistEnemyOnly) true�̏ꍇ�A�U���Ώۃ��j�b�g������ꍇ�̂ݕ\��
	*/
	void Map::displayAtackAbleRange(shared_ptr<Unit> unit, int x, int y, bool isExistEnemyOnly)
	{
		// �U���\�̃}�X�̃��X�g���擾
		vector<pair<int, int>> posList;
		getAtackAbleMasses(unit, x, y, posList, isExistEnemyOnly);

		for (auto itr = posList.begin(); itr != posList.end(); ++itr)
		{
			getMass(itr->first, itr->second)->state = Mass::State::ATK_ABLE; // �U���\�G���A�\��
		}
	}

	/**
	 * @fn
	 * �U���Ώۃ}�X���擾
	 * @param (unit) �Ώۃ��j�b�g
	 * @param (x) �U���N�_�}�X���WX
	 * @param (y) �U���N�_�}�X���WY
	 * @param (posList) �Ώۃ}�X�̃��X�g��Ԃ����߂̎Q��
	 * @param (isExistEnemyOnly) true�̏ꍇ�A�U���Ώۃ��j�b�g������ꍇ�̂ݕ\��
	*/
	void Map::getAtackAbleMasses(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly)
	{
		if (unit)
		{
			if (!unit->isAtackable()) // ���j�b�g���U���s�̏��
			{
				return;
			}

			int range = unit->getInfo().range;

			for (int i = 1; i <= range; i++)
			{
				getAtackAbleMass(unit, x - i, y, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x + i, y, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x, y - i, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x, y + i, posList, isExistEnemyOnly);
			}
		}
	}

	/**
	 * @fn
	 * �U���\�}�X�ł��邩����
	 * @param (unit) �Ώۃ��j�b�g
	 * @param (x) �U���N�_�}�X���WX
	 * @param (y) �U���N�_�}�X���WY
	 * @param (posList) �Ώۃ}�X�̃��X�g��Ԃ����߂̎Q��
	 * @param (isExistEnemyOnly) true�̏ꍇ�A�U���Ώۃ��j�b�g������ꍇ�̂ݕ\��
	*/
	void Map::getAtackAbleMass(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly)
	{
		shared_ptr<Mass> nowMass = getMass(x, y);

		// �}�b�v�O
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		if (!isExistEnemyOnly)
		{
			posList.push_back(make_pair(x, y));
		}
		else
		{
			// �G���j�b�g�����݂���ꍇ�̂ݍU���\�ɂ���
			shared_ptr<Unit> targetUnit = getUnit(x, y);
			if (targetUnit && targetUnit->isEnemy() != unit->isEnemy())
			{
				posList.push_back(make_pair(x, y));
			}
		}
	}

	/**
	 * @fn
	 * �}�X�̃|�C���^��Ԃ�
	 * @param (x) �}�X��x���W
	 * @param (y) �}�X��y���W
	 * @return �}�X
	 */
	shared_ptr<Mass> Map::getMass(int massX, int massY)
	{
		if (isRange(massX, massY))
		{
			try
			{
				return mass_.at(massY).at(massX);
			}
			catch (out_of_range&) {}
		}

		shared_ptr<Mass> mass = make_shared<Mass>();
		return mass;
	}

	/**
	 * @fn
	 * �}�X�̏�Ԃ�����������
	 */
	void Map::clearMassState()
	{
		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				(*cell)->state = Mass::State::NORMAL;
				(*cell)->passingMov = -1;
			}
		}
	}

	/**
	 * @fn
	 * �}�b�v�͈͓��ł��邩����
	 * @param (x) �}�X��x���W
	 * @param (y) �}�X��y���W
	 * @return �͈͓��ł���ꍇ��true��Ԃ�
	 */
	bool Map::isRange(int x, int y) const
	{
		return 0 <= x && x < MAP_MASS_W && 0 <= y && y < MAP_MASS_H;
	}


	/**
	 * @fn
	 * �ړ��\�͈͂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawMoveableMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_MOVE), TRUE);
	}

	/**
	 * @fn
	 * �U���\�͈͂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawAtackMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_ATACK), TRUE);
	}

	/**
	 * @fn
	 * �z�o�[�}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawHoverMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/**
	 * @fn
	 * �I�𒆂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawSelectedMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/**
	 * @fn
	 * �}�X�̍��W���琳���̍��W���擾(x���W)
	 * @param (massX) �}�X���WX
	 */
	int Map::getRealX(int massX)
	{
		return massX * CHIP_SIZE;
	}

	/**
	 * @fn
	 * �}�X�̍��W���琳���̍��W���擾(y���W)
	 * @param (massY) �}�X���WY
	 */
	int Map::getRealY(int massY)
	{
		return massY * CHIP_SIZE + MAP_Y;
	}

	/**
	 * @fn
	 * �����̍��W����}�X�̍��W���擾(x���W)
	 * @param (realX) ������X���W
	 */
	int Map::getMassX(int realX)
	{
		return realX / CHIP_SIZE;
	}

	/**
	 * @fn
	 * �����̍��W����}�X�̍��W���擾(y���W)
	 * @param (realY) ������Y���W
	 */
	int Map::getMassY(int realY)
	{
		return (realY - MAP_Y) / CHIP_SIZE;
	}

	/**
	 * @fn
	 * �}�X���W����Ƀ}�X�Ԃ̋������擾
	 * @param x0 ��n�_��X���W
	 * @param y0 ��n�_��Y���W
	 * @param x1 �Ώےn�_��X���W
	 * @param y1 �Ώےn�_��Y���W
	 * @return �}�X���W��̋���
	 */
	int Map::getMassDistance(int x0, int y0, int x1, int y1)
	{
		return abs(x0 - x1) + abs(y0 - y1);
	}



}