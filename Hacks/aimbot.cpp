//
//  aimbot.cpp
//  vHook
//

#include "aimbot.h"
#include "autoshoot.h"
#include "antiaim.h"
#include "autowall.h"
#include "autostop.hpp"
#include "logshots.hpp"

C_BaseEntity* Aimbot::curTarget = nullptr;

void AutoSlow(C_BasePlayer* player, C_BaseCombatWeapon* active_weapon, CUserCmd* cmd)
{
    

    
    
   
}


template<class T, class U>
inline T clamp(T in, U low, U high)
{
    if (in <= low)
        return low;
    else if (in >= high)
        return high;
    else
        return in;
}

float LagFix()
{
    float updaterate = pCvar->FindVar("cl_updaterate")->GetFloat();
    ConVar* minupdate = pCvar->FindVar("sv_minupdaterate");
    ConVar* maxupdate = pCvar->FindVar("sv_maxupdaterate");
    
    if (minupdate && maxupdate)
        updaterate = maxupdate->GetFloat();
    
    float ratio = pCvar->FindVar("cl_interp_ratio")->GetFloat();
    
    if (ratio == 0)
        ratio = 1.0f;
    
    float lerp = pCvar->FindVar("cl_interp")->GetFloat();
    ConVar* cmin = pCvar->FindVar("sv_client_min_interp_ratio");
    ConVar* cmax = pCvar->FindVar("sv_client_max_interp_ratio");
    
    if (cmin && cmax && cmin->GetFloat() != 1)
        ratio = clamp(ratio, cmin->GetFloat(), cmax->GetFloat());
    
    return std::max(lerp, ratio / updaterate);
}

int MakeHitscan(C_BaseEntity* entity)
{
    vector<int> hitboxes;
    if(vars.aimbot.hitscan > 0)
    {
        if(vars.aimbot.baimhp){
            int huso = (entity->GetHealth());
            int health = vars.aimbot.baimxhp;
            
            if (huso < health) {
                
                hitboxes.push_back(HITBOX_PELVIS);
                hitboxes.push_back(HITBOX_BELLY);
                hitboxes.push_back(HITBOX_THORAX);
                hitboxes.push_back(HITBOX_LOWER_CHEST);
                hitboxes.push_back(HITBOX_UPPER_CHEST);
                hitboxes.push_back(HITBOX_RIGHT_THIGH);
                hitboxes.push_back(HITBOX_LEFT_THIGH);
                hitboxes.push_back(HITBOX_RIGHT_CALF);
                hitboxes.push_back(HITBOX_LEFT_CALF);
                hitboxes.push_back(HITBOX_RIGHT_FOOT);
                hitboxes.push_back(HITBOX_LEFT_FOOT);
                hitboxes.push_back(HITBOX_RIGHT_HAND);
                hitboxes.push_back(HITBOX_LEFT_HAND);
                hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
                hitboxes.push_back(HITBOX_RIGHT_FOREARM);
                hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
                hitboxes.push_back(HITBOX_LEFT_FOREARM);
            }
        }
        
        if (vars.aimbot.hitscantype == HITSCAN::low)
        { // low
            hitboxes.push_back(HITBOX_HEAD);
            hitboxes.push_back(HITBOX_NECK);
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_THORAX);
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
        }
        if (vars.aimbot.hitscantype == HITSCAN::normal)
        { //normal
            hitboxes.push_back(HITBOX_HEAD);
            hitboxes.push_back(HITBOX_NECK);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            
        }
        if (vars.aimbot.hitscantype == HITSCAN::high)
        { //high
            hitboxes.push_back(HITBOX_HEAD);
            hitboxes.push_back(HITBOX_NECK);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_CALF);
            hitboxes.push_back(HITBOX_LEFT_CALF);
            hitboxes.push_back(HITBOX_LEFT_FOREARM);
            hitboxes.push_back(HITBOX_RIGHT_FOREARM);
        }
        
        if (vars.aimbot.hitscantype == HITSCAN::extreme)
        {
            //extreme
            hitboxes.push_back(HITBOX_HEAD);
            hitboxes.push_back(HITBOX_NECK);
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_THORAX);
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_CALF);
            hitboxes.push_back(HITBOX_LEFT_CALF);
            hitboxes.push_back(HITBOX_RIGHT_FOOT);
            hitboxes.push_back(HITBOX_LEFT_FOOT);
            hitboxes.push_back(HITBOX_RIGHT_HAND);
            hitboxes.push_back(HITBOX_LEFT_HAND);
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_FOREARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_FOREARM);
        }
        if (vars.aimbot.hitscantype == HITSCAN::baim)
        {
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_THORAX);
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
        }
        if (vars.aimbot.hitscantype == HITSCAN::head){
            hitboxes.push_back(HITBOX_HEAD);
        }
        if (vars.aimbot.hitscantype == HITSCAN::upperbody){
            hitboxes.push_back(HITBOX_HEAD);
             hitboxes.push_back(HITGROUP_STOMACH);
             hitboxes.push_back(HITBOX_PELVIS);
        }
        if (vars.aimbot.hitscantype == HITSCAN::lowerbody){
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITGROUP_STOMACH);
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_FOOT);
            hitboxes.push_back(HITBOX_LEFT_FOOT);
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_FOREARM);
            hitboxes.push_back(HITBOX_LEFT_FOREARM);
            hitboxes.push_back(HITBOX_RIGHT_HAND);
            hitboxes.push_back(HITBOX_LEFT_HAND);
        }
        if (vars.aimbot.hitscantype == HITSCAN::arms){
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_FOREARM);
            hitboxes.push_back(HITBOX_LEFT_FOREARM);
            hitboxes.push_back(HITBOX_RIGHT_HAND);
            hitboxes.push_back(HITBOX_LEFT_HAND);
        }
        if (vars.aimbot.hitscantype == HITSCAN::legs){
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_FOOT);
            hitboxes.push_back(HITBOX_LEFT_FOOT);
        }
        if(vars.aimbot.hitscantype == HITSCAN::baimlowerbody){
            hitboxes.push_back(HITBOX_LOWER_CHEST);
            hitboxes.push_back(HITGROUP_STOMACH);
            hitboxes.push_back(HITBOX_PELVIS);
            hitboxes.push_back(HITBOX_LEFT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_THIGH);
            hitboxes.push_back(HITBOX_RIGHT_FOOT);
            hitboxes.push_back(HITBOX_LEFT_FOOT);
            hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
            hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
            hitboxes.push_back(HITBOX_RIGHT_FOREARM);
            hitboxes.push_back(HITBOX_LEFT_FOREARM);
            hitboxes.push_back(HITBOX_RIGHT_HAND);
            hitboxes.push_back(HITBOX_LEFT_HAND);
            hitboxes.push_back(HITBOX_BELLY);
            hitboxes.push_back(HITBOX_THORAX);
            hitboxes.push_back(HITBOX_UPPER_CHEST);
            hitboxes.push_back(HITBOX_RIGHT_CALF);
            hitboxes.push_back(HITBOX_LEFT_CALF);
        }
    }

    
    if(vars.aimbot.hitscan)
    {
        
        int highestdamage_hitbox = 0;
        int highestdamage = 0;
        
        for(auto hit : hitboxes)
        {
            Vector vDest = GetHitboxPosition(entity, hit);
            float thisDamage = 0.f;
            Autowall::FireBulletData data;
            
            thisDamage = Autowall::GetDamage(vDest, true, data);
            
            if(thisDamage > highestdamage)
            {
                highestdamage = thisDamage;
                highestdamage_hitbox = hit;
            }
            
        }
        
        return highestdamage_hitbox;
    }
    else
    {
        return vars.aimbot.hitbox;
    }
    
}

void DoAim(CUserCmd* pCmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, float& flForward, float& flSide, C_BasePlayer* player)
{
    
    if(!vars.aimbot.enabled)
        return;
    
    Vector eyepos = local->GetEyePosition();
    
    
    
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++)
    {
        auto* entity = pEntList->GetClientEntity(i);
        //int classID = entity->GetClientClass()->m_ClassID;
        
        if(!entity)
            continue;
        
        if(entity == local)
            continue;
        
        if(entity->GetTeam() == local->GetTeam())
            continue;
        
        if(entity->GetHealth() < 1)
            continue;
        
        if(entity->IsGhost()) ///<== Aimbot.cpp
            continue; // Ghost Fix
        
        if(entity->GetDormant())
            continue;
        
        if(entity->GetImmune())
            continue;
        
        if(weapon->GetAmmo() < 1)
            continue;
        
        if(entity->GetClientClass()->m_ClassID == (int)CHostage)
            continue;
        
        if(weapon->IsKnife() || weapon->IsBomb() || weapon->IsGrenade())
            continue;
        
        if(gCorrections[entity->GetIndex()].whitelist)
            continue;
        
        Vector vFrom, vTo;
        
        Aimbot::curTarget = entity;
        
        vFrom = GetHitboxPosition(entity, MakeHitscan(entity));
        
        vTo = CalcAngle(eyepos, vFrom);
        
        auto isVISIBLE = IsVisible(local, entity);
        float getdmg = 0.f;
        Autowall::FireBulletData data;
        bool canHit = false;
        
        if(vars.aimbot.autowall)
        {
            getdmg = Autowall::GetDamage(vFrom, true, data);
            canHit = getdmg >= vars.aimbot.mindmg;
        }
        
        atTargets = vTo;
        
        if(canHit || isVISIBLE)
        {
            if(GetFOV(pCmd->viewangles, local->GetEyePosition(), vFrom) <= vars.aimbot.FovToPlayer)
            {
                if(vars.aimbot.autoshoot)
                {
                    AutoShoot(local, weapon, pCmd);
                    AutoCock(pCmd, weapon);
                }
                
                if(vars.aimbot.autocrouch)
                {
                    pCmd->buttons |= IN_BULLRUSH | IN_DUCK;
                }
                
                if (vars.aimbot.autoscope && weapon->GetCSWpnData()->m_iZoomLevels > 0 && !local->IsScoped() && weapon->IsSnipScope()) //&& !local->IsScoped())
                {
                    pCmd->buttons |= IN_ATTACK2;
                    return;
                }
                if (vars.aimbot.autostop){
                    pCmd->forwardmove = 0.f;
                    pCmd->sidemove = 0.f;
                    pCmd->upmove = 0.f;
                
                }
                if(vars.aimbot.autoslow){
                    
                    C_BasePlayer* localplayer = (C_BasePlayer*) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
                    
                    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) pEntList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
                    if (!activeWeapon || activeWeapon->GetAmmo() == 0)
                        return;
                    
                    if( localplayer->GetVelocity().Length() > (activeWeapon->GetCSWpnData1()->GetMaxPlayerSpeed() / 3) )
                    {
                        pCmd->buttons |= IN_WALK;
                        pCmd->forwardmove = -pCmd->forwardmove;
                        pCmd->sidemove =  -pCmd->sidemove;
                        pCmd->upmove = 0.f;
                    }
                }
                if (pCmd->buttons & IN_ATTACK && vars.misc.logshots && player){
                    C_BasePlayer* localplayer = (C_BasePlayer*) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
                    LogShots::shots.push_back(Shots(player, localplayer->GetEyePosition(), pGlobals->curtime));
                }
                
                bool bAttack = true;
                
                if (weapon->GetNextPrimaryAttack() - pGlobals->interval_per_tick > local->GetTickBase() * pGlobals->interval_per_tick)
                    bAttack = false;
                
                if(pCmd->buttons & IN_ATTACK)
                {
                    pCmd->tick_count = TIME_TO_TICKS(entity->GetSimulationTime() + LagFix());
                    if(!vars.aimbot.silent)
                    {
                        pCmd->viewangles = vTo;
                        pEngine->SetViewAngles(pCmd->viewangles);
                    }
                    else
                    {
                        pCmd->viewangles = vTo;
                    }
                }
            }
            
        }
        
    }
    
}
    














