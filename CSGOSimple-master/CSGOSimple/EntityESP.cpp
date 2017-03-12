#include "EntityESP.hpp"
#include "Options.hpp"
#include "Utils.hpp"
#include "sdk2.h"
#include "SourceEngine\SDK.hpp"
#include <d3d9.h> //D3DCOLOR_ARGB
EntityESP::EntityESP(C_CSPlayer* pEntity)
    : m_pEntity(pEntity)
{
}

EntityESP::~EntityESP()
{
}

void EntityESP::RenderESP(DrawManager& renderer)
{
    using namespace se;

    auto pLocal = C_CSPlayer::GetLocalPlayer();

    if(!pLocal) return;

    //Checks if its an enemy
    auto bIsEnemy = pLocal->GetTeamNum() != m_pEntity->GetTeamNum();

    auto allyColor = Options::g_bESPAllyColor;
    auto enemyColor = Options::g_bESPEnemyColor;

    //Get the appropriate drawing color
    auto d3dColor = bIsEnemy ?
        D3DCOLOR_RGBA(int(enemyColor[0] * 255.0f), int(enemyColor[1] * 255.0f), int(enemyColor[2] * 255.0f), int(enemyColor[3] * 255.0f)) :
        D3DCOLOR_RGBA(int(allyColor[0] * 255.0f), int(allyColor[1] * 255.0f), int(allyColor[2] * 255.0f), int(allyColor[3] * 255.0f));

    auto vOrigin = m_pEntity->GetOrigin();
    auto vHead = Utils::GetEntityBone(m_pEntity, ECSPlayerBones::head_0);

    //Offset the head a bit, so the box looks nicer
    vHead.z += 15.0f;

    Vector vScreenOrigin, vScreenHead;
    if(Utils::WorldToScreen(vHead, vScreenHead) &&
        Utils::WorldToScreen(vOrigin, vScreenOrigin)) {

        float height = abs(vScreenHead.y - vScreenOrigin.y);
        float width = height * 0.65f;
        renderer.AddRect({vScreenOrigin.x - width / 2, vScreenHead.y}, width, height, d3dColor);
    }
}

void EntityESP::RenderName(DrawManager& renderer)
{
	using namespace se;

	auto pLocal = C_CSPlayer::GetLocalPlayer();

	if (!pLocal) return;

	//Checks if its an enemy
	auto bIsEnemy = pLocal->GetTeamNum() != m_pEntity->GetTeamNum();

	auto allyColor = Options::g_bESPAllyColor;
	auto enemyColor = Options::g_bESPEnemyColor;

	//Get the appropriate drawing color
	auto d3dColor = bIsEnemy ?
		D3DCOLOR_RGBA(int(enemyColor[0] * 255.0f), int(enemyColor[1] * 255.0f), int(enemyColor[2] * 255.0f), int(enemyColor[3] * 255.0f)) :
		D3DCOLOR_RGBA(int(allyColor[0] * 255.0f), int(allyColor[1] * 255.0f), int(allyColor[2] * 255.0f), int(allyColor[3] * 255.0f));

	auto vOrigin = m_pEntity->GetOrigin();

	Vector vScreenOrigin;
	if (Utils::WorldToScreen(vOrigin, vScreenOrigin)) {

		player_info_t pInfo;
		Interfaces::Engine()->GetPlayerInfo(m_pEntity->EntIndex(), &pInfo);
		renderer.AddText({ vScreenOrigin.x, vScreenOrigin.y }, d3dColor, centered_x | outline, pInfo.szName);
	}
}