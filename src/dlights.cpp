#include "dlights.h"

bool Settings::Dlights::enabled = false;
float Settings::Dlights::radius = 500.0f;

void Dlights::Paint()
{
	if (!Settings::ESP::enabled || !Settings::Dlights::enabled)
		return;

	if (!Settings::ESP::Filters::enemies && !Settings::ESP::Filters::allies)
		return;

	if (!engine->IsInGame())
		return;

	C_BasePlayer* localPlayer = (C_BasePlayer*)entitylist->GetClientEntity(engine->GetLocalPlayer());
	if (!localPlayer)
		return;

	for (int i = 1; i < engine->GetMaxClients(); ++i)
	{
		C_BaseEntity *entity = entitylist->GetClientEntity(i);
		if (!entity)
			continue;

		if (entity == localPlayer)
			continue;

		bool bIsVisible = false;
		if (Settings::ESP::Filters::visibility_check || Settings::ESP::Filters::legit)
		{
			bIsVisible = Entity::IsVisible(entity, BONE_HEAD);
			if (!bIsVisible && Settings::ESP::Filters::legit)
				continue;
		}

		Color color = Color::FromImColor(ESP::GetESPPlayerColor(entity, bIsVisible));

		dlight_t* dLight = effects->CL_AllocDlight(i);
		dLight->key = i;
		dLight->color.r = (unsigned char) color.r;
		dLight->color.g = (unsigned char) color.g;
		dLight->color.b = (unsigned char) color.b;
		dLight->color.exponent = true;
		dLight->flags = DLIGHT_NO_MODEL_ILLUMINATION;
		dLight->m_Direction = entity->GetVecOrigin();
		dLight->origin = entity->GetVecOrigin();
		dLight->radius = Settings::Dlights::radius;
		dLight->die = globalvars->curtime + 0.1f;
		dLight->decay = 20.0f;
	}
}
