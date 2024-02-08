// copyright lololol

#include "Core/PiratePlayerState.h"

#include "Net/UnrealNetwork.h"

void APiratePlayerState::AddXP(float AddedXP)
{
	while (AddedXP > 0)
	{
		const float XPMul = GetCurrentXPMultiplier();
		float AdjustedXP = AddedXP * XPMul;
		if (AdjustedXP > (1 - this->XP))
		{
			Level++;
			AdjustedXP -= 1 - this->XP;
			this->XP = 0;
			AddedXP = AdjustedXP / XPMul;
		} else
		{
			this->XP += AdjustedXP;
			AddedXP = 0;
		}
	}

	OnXPUpdated.Broadcast(XP, Level);
}

void APiratePlayerState::OnRep_Level() const
{
	OnXPUpdated.Broadcast(XP, Level);
}

void APiratePlayerState::OnRep_XP() const
{
	OnXPUpdated.Broadcast(XP, Level);
}

void APiratePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiratePlayerState, Level);
	DOREPLIFETIME(APiratePlayerState, XP);
}
