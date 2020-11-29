#include "ResourceManager.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	int ResourceManager::load()
	{
		int ret = 0;
		resource_.insert(std::make_pair(MAP, vector<int*>()));
		resource_.at(MAP).push_back(new int[8]);
		loadMapchip("resource/mapchip.png", resource_.at(MAP).at(0));
		
		return ret;
	}

	int ResourceManager::getResource(ResourceType type, int kind, int pos) const
	{
		vector<int*> test = resource_.at(type);
		return resource_.at(type).at(kind)[pos];
	}

	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}

	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 3, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}

	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

}