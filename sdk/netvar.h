#pragma once

#include <includes.h>

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
				DumpRecursive(pProperty->m_pDataTable); // make threaded or async
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

/*
class Player {
	NETVAR(m_fFlags, float, "DT_BasePlayer->m_fFlags")
}

// Gettings offsets from class
((Player*)local_player)->m_fFlags();
*/


	//CBasePlayer is DT_BasePlayer
	//CCSPlayer is DT_CSPlayer
//m_iHealth + NetVarManager::Get().GetOffset("DT_BasePlayer->m_fFlags");
// problems EVERYTHING ITS ALL FUCKING FUCKED THE m_Collision DOESNT HAVE ANYTHING ITS THE WRONG OFFSET, THE FUCKING LOCAL PLAYER PLUS BULLSHIT DOESNT WORK AND THE FIRST SHIT AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



//Get().GetOffset("DT_BasePlayer->m_fFlags");
//Get().GetOffset("DT_CSPlayer->m_ArmorValue");
