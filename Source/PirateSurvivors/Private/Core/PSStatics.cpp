// copyright lololol

#include "Core/PSStatics.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

bool UPSStatics::InActualGame()
{
#if WITH_EDITOR
	return ( !GEditor || GEditor->bIsSimulatingInEditor || ( GEditor->PlayWorld != NULL ) );
#else
	return true;
#endif
}
