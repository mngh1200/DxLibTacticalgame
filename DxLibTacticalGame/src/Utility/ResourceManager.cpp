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
     * リソースのロード
     * @return 終了フラグ（0が正常）
     */
	int ResourceManager::load()
	{
		int ret = 0;

		resource_.insert(std::make_pair(ResourceType::IMAGE, vector<int*>()));
		resource_.at(ResourceType::IMAGE).push_back(new int[IMAGE_ID_LEN]);
		loadImage("resource/image/menu/background.jpg", resource_.at(ResourceType::IMAGE).at(0));


		resource_.insert(std::make_pair(ResourceType::MAP, vector<int*>()));
		resource_.at(ResourceType::MAP).push_back(new int[8]);
		loadMapchip("resource/mapchip.png", resource_.at(ResourceType::MAP).at(0));
		// プレイヤー画像の読み込み
		resource_.insert(std::make_pair(ResourceType::PLAYER, vector<int*>()));
		resource_.at(ResourceType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/lancer.png", resource_.at(ResourceType::PLAYER).at(UnitKey::LANCER));
		resource_.insert(std::make_pair(ResourceType::PLAYER, vector<int*>()));
		resource_.at(ResourceType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/cavalry.png", resource_.at(ResourceType::PLAYER).at(UnitKey::CAVALRY));
		resource_.insert(std::make_pair(ResourceType::PLAYER, vector<int*>()));
		resource_.at(ResourceType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/gunner.png", resource_.at(ResourceType:: PLAYER).at(UnitKey::GUNNER));
		// エネミー画像の読み込み
		resource_.insert(std::make_pair(ResourceType::ENEMY, vector<int*>()));
		resource_.at(ResourceType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/lancer_enemy.png", resource_.at(ResourceType::ENEMY).at(UnitKey::LANCER));
		resource_.insert(std::make_pair(ResourceType::ENEMY, vector<int*>()));
		resource_.at(ResourceType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/cavalry_enemy.png", resource_.at(ResourceType::ENEMY).at(UnitKey::CAVALRY));
		resource_.insert(std::make_pair(ResourceType::ENEMY, vector<int*>()));
		resource_.at(ResourceType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/gunner_enemy.png", resource_.at(ResourceType::ENEMY).at(UnitKey::GUNNER));
		
		return ret;
	}

	/**
	 * @fn
	 * 指定されたオブジェクト取得(1毎絵用)
	 * @param (type) リソースタイプ
	 * @param (kind) リソースの種類
	 * @return 対象リソースのID
	 */
	int ResourceManager::getResource(ResourceType type, int kind) const
	{
		vector<int*> test = resource_.at(type);
		return resource_.at(type).at(kind)[0];
	}
	/**
     * @fn
     * 指定されたオブジェクト取得
     * @param (type) リソースタイプ
     * @param (kind) リソースの種類
	 * @param (pos) リソースの位置
     * @return 対象リソースのID
     */
	int ResourceManager::getResource(ResourceType type, int kind, int pos) const
	{
		vector<int*> test = resource_.at(type);
		return resource_.at(type).at(kind)[pos];
	}
	/**
	 * @fn
	 * マップチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

}