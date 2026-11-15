#include "AstraGameMode.h"
#include "AstraExplorerCharacter.h"

AAstraGameMode::AAstraGameMode()
{
    DefaultPawnClass = AAstraExplorerCharacter::StaticClass();
}