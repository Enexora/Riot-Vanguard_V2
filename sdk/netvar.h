#pragma once

#include "includes.h"

class NetVarManager
{
public:
	NetVarManager() {}

	std::map<std::string, RecvProp*> m_mapOffsets;

	static NetVarManager& Get()
	{
		static NetVarManager Instance;

		return Instance;
	}

	unsigned short GetOffset(std::string name)
	{
		return m_mapOffsets[name]->m_Offset;
	}

	RecvProp* GetPropPtr(std::string name)
	{
		return m_mapOffsets[name];
	}

	inline void DumpRecursive(RecvTable* pTable) {
		for (auto i = 0; i < pTable->m_nProps; ++i)
		{
			auto pProperty = &pTable->m_pProps[i];
 
			if (!pProperty || isdigit(pProperty->m_pVarName[0]))
				continue;
 
			if (strcmp(pProperty->m_pVarName, "baseclass") == 0)
				continue;
 
			if (pProperty->m_RecvType == DPT_DataTable &&
				pProperty->m_pDataTable != nullptr &&
				pProperty->m_pDataTable->m_pNetTableName[0] == 'D')
			{
				DumpRecursive(pProperty->m_pDataTable);
			}
 
			std::string strHash = pTable->m_pNetTableName;
			strHash += "->";
			strHash += pProperty->m_pVarName;
 
			m_mapOffsets[strHash] = pProperty;

			// this is where each netvar is at
		}
	}
};

// These preprocessors are for classes
#define PNETVAR(funcname, type, netvarname) type* funcname() \
{ \
	static uintptr_t offset = NetVarManager::Get().GetOffset(netvarname); \
	return reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETVAR(funcname, type, netvarname) type& funcname() \
{ \
	static uintptr_t offset = NetVarManager::Get().GetOffset(netvarname); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

class Player {
public:
	NETVAR(m_vecOrigin, Vector3, "DT_BaseEntity->m_vecOrigin")
	NETVAR(m_vecVelocity, Vector3, "DT_BasePlayer->m_vecVelocity")
	QAngle m_aimPunchAngle() {
		static uintptr_t offset1 = NetVarManager::Get().GetOffset("DT_Local->m_aimPunchAngle");
		return *(QAngle*)(this + 0x2FCC + offset1);
	}
	NETVAR(m_bIsScoped, bool, "DT_CSPlayer->m_bIsScoped")
	NETVAR(m_flLowerBodyYawTarget, float, "DT_CSPlayer->m_flLowerBodyYawTarget")
	NETVAR(m_flSimulationTime, float, "DT_BaseEntity->m_flSimulationTime")
	PNETVAR(m_iHideHud, int, "DT_Local->m_iHideHud")
	NETVAR(m_vecViewOffset, Vector3, "DT_BasePlayer->m_vecViewOffset")
	PNETVAR(m_skybox3d_origin, Vector3,  "DT_Local->m_skybox3d.origin");
	PNETVAR(m_skybox3d_scale, int,  "DT_Local->m_skybox3d.scale");
	PNETVAR(m_angRotation, QAngle, "DT_BaseEntity->angRotation")
	PNETVAR(m_fFlags, int, "DT_BasePlayer->m_fFlags")
	NETVAR(m_hActiveWeapon, DWORD, "DT_BaseCombatCharacter->m_hActiveWeapon")
	NETVAR(m_Local, int, "DT_LocalPlayerExclusive->m_Local")
	NETVAR(m_nTickBase, int, "DT_LocalPlayerExclusive->m_nTickBase")
};

class WeaponEntity {
public:
	NETVAR(m_zoomLevel, int, "DT_WeaponCSBaseGun->m_zoomLevel")
	NETVAR(m_iBurstShotsRemaining, int, "DT_WeaponCSBaseGun->m_iBurstShotsRemaining")
	NETVAR(m_flNextPrimaryAttack, float, "DT_LocalActiveWeaponData->m_flNextPrimaryAttack")
	//NETVAR(m_iItemDefinitionIndex, int, "DT_ScriptCreatedItem->m_iItemDefinitionIndex")
	int m_iItemDefinitionIndex() {
		static int offset = NetVarManager::Get().GetOffset("DT_EconEntity->m_AttributeManager");
		static int offset1 = NetVarManager::Get().GetOffset("DT_AttributeContainer->m_Item");
		static int offset2 = NetVarManager::Get().GetOffset("DT_ScriptCreatedItem->m_iItemDefinitionIndex");
		return *(int*)(this + offset + offset1 + offset2);
	}
	bool isSniper() {
		switch (m_iItemDefinitionIndex()) {
		case WEAPON_AWP:
			return true;
			break;
		case WEAPON_SSG08:
			return true;
			break;
		case WEAPON_G3SG1:
			return true;
			break;
		case WEAPON_SCAR20:
			return true;
			break;
		default:
			return false;
			break;
		}
	}
};

// Gettings offsets from class
//((Player*)local_player)->m_fFlags();
// 
	//CBasePlayer is DT_BasePlayer
	//CCSPlayer is DT_CSPlayer
//m_iHealth + NetVarManager::Get().GetOffset("DT_BasePlayer->m_fFlags");
// problems EVERYTHING ITS ALL FUCKING FUCKED THE m_Collision DOESNT HAVE ANYTHING ITS THE WRONG OFFSET, THE FUCKING LOCAL PLAYER PLUS BULLSHIT DOESNT WORK AND THE FIRST SHIT AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
// DT_PlayerState->deadflag
//Get().GetOffset("DT_BasePlayer->m_fFlags");
//Get().GetOffset("DT_CSPlayer->m_ArmorValue");
