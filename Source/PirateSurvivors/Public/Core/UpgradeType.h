// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UpgradeType.generated.h"

// Used to determine how the stat should be upgraded
UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	Add       UMETA(DisplayName = "Add"),
	Multiply  UMETA(DisplayName = "Multiply")
};
