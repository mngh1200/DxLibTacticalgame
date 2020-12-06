#pragma once
#include "Define.h"
#include "DxLib.h"
#include <map>
#include <vector>
using namespace std;

/**
 * @file Resource Manager.h
 * @brief ���\�[�X���Ǘ�����N���X
 */
namespace Utility
{
	class ResourceManager
	{
	public:
		// ���\�[�X�^�C�v
		enum ResourceType {
			PLAYER,
			ENEMY,
			MAP,
			IMAGE
		};
		// PLAYER�AENEMY�̂�\�[�X�̊i�[�ꏊ
		enum UnitKey {
			LANCER,
			CAVALRY,
			GUNNER
		};
		// PLAYER�AENEMY�̃A�j���[�V�����J�n�ʒu(��)
		enum UnitStartPos {
			TOP,
			LEFT=4,
			RIGHT=8,
			DOWN=12
		};

		// Image��ID
		enum ImageId {
			BACKGROUND_MENU,
			IMAGE_ID_LEN
		};

		//for singleton
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		static ResourceManager& getInstance();

		int load();

		int getResource(ResourceType type, int kind, int pos) const;
	private:
		ResourceManager()
			:resource_{}
		{};
		~ResourceManager() {};

		void loadMapchip(const char* resourcePath, int* handle);
		void loadCharacterchip(const char* resourcePath, int* handle);
		void loadImage(const char* resourcePath, int* handle);

		map<ResourceType, std::vector<int*>> resource_;
		
		int hdlMapchip_[8];
	};
}