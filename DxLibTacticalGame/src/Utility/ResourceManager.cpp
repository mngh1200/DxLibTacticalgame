#include "ResourceManager.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}
	/**
	 * @fn
     * ���\�[�X�̃��[�h
     * @return �I���t���O�i0������j
     */
	int ResourceManager::load()
	{
		int ret = 0;

		resource_.insert(std::make_pair(ResourceType::IMAGE, vector<int*>()));
		resource_.at(ResourceType::IMAGE).push_back(new int[IMAGE_ID_LEN]);
		loadImage("resource/image/menu/background.jpg", resource_.at(ResourceType::IMAGE).at(0));


		resource_.insert(std::make_pair(MAP, vector<int*>()));
		resource_.at(MAP).push_back(new int[8]);
		loadMapchip("resource/mapchip.png", resource_.at(MAP).at(0));

		resource_.insert(std::make_pair(PLAYER, vector<int*>()));
		resource_.at(PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/lancer.png", resource_.at(PLAYER).at(LANCER));
		resource_.insert(std::make_pair(PLAYER, vector<int*>()));
		resource_.at(PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/cavalry.png", resource_.at(PLAYER).at(CAVALRY));
		resource_.insert(std::make_pair(PLAYER, vector<int*>()));
		resource_.at(PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/gunner.png", resource_.at(PLAYER).at(GUNNER));
		
		return ret;
	}

	/**
     * @fn
     * �w�肳�ꂽ�I�u�W�F�N�g�擾
     * @param (type) ���\�[�X�^�C�v
     * @param (kind) ���\�[�X�̎��
	 * @param (pos) ���\�[�X�̈ʒu
     * @return �Ώۃ��\�[�X��ID
     */
	int ResourceManager::getResource(ResourceType type, int kind, int pos) const
	{
		vector<int*> test = resource_.at(type);
		return resource_.at(type).at(kind)[pos];
	}
	/**
	 * @fn
	 * �}�b�v�`�b�v�p�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * �L�����`�b�v�p�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1���G�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

}