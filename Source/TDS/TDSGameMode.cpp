#include "TDSGameMode.h"
#include "TDSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

//-------------------------------------------------------------------------------------------------------------
ATDSGameMode::ATDSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATDSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted Character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprint/Character/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
		PlayerControllerClass = PlayerControllerBPClass.Class;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSGameMode::BP_Dead_Implementation()
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------