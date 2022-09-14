// Copyright 2022 Ivan Baktenkov. All Rights Reserved.

#include "LockOnTargetEditor.h"
#include "TargetingHelperComponent.h"
#include "LockOnTargetComponent.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateStyleRegistry.h"

#include "LockOnComponentDetails.h"
#include "Editor/UnrealEdEngine.h"

#include "GameplayDebugger.h"
#include "GDC_LockOnTargetComponent.h"

DEFINE_LOG_CATEGORY(LogLockOnTargetEditor);

IMPLEMENT_MODULE(FLockOnTargetEditorModule, LockOnTargetEditor)

#define LOCTEXT_NAMESPACE "FLockOnTargetModule"

void FLockOnTargetEditorModule::StartupModule()
{
	UE_LOG(LogLockOnTargetEditor, Log, TEXT("LockOnTargetEditor module startup."));

	RegisterStyles();

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	PropertyModule.RegisterCustomClassLayout(ULockOnTargetComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FLockOnComponentDetails::MakeInstance));

#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GDModule = IGameplayDebugger::Get();
	GDModule.RegisterCategory(TEXT("LockOnTarget"), IGameplayDebugger::FOnGetCategory::CreateStatic(&FGDC_LockOnTarget::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGame);
	GDModule.NotifyCategoriesChanged();
#endif
}

void FLockOnTargetEditorModule::ShutdownModule()
{
	UE_LOG(LogLockOnTargetEditor, Log, TEXT("LockOnTargetEditor module shutdown."));

	UnregisterStyles();

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	PropertyModule.UnregisterCustomClassLayout(ULockOnTargetComponent::StaticClass()->GetFName());

#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable()) 
	{
		IGameplayDebugger& GDModule = IGameplayDebugger::Get();
		GDModule.UnregisterCategory(TEXT("LockOnTarget"));
		GDModule.NotifyCategoriesChanged();
	}
#endif
}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(LockOnTargetStyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

void FLockOnTargetEditorModule::RegisterStyles()
{
	const FVector2D Icon16x{ 16.f, 16.f };
	const FVector2D Icon64x{ 64.f, 64.f };

	LockOnTargetStyleSet = MakeShared<FSlateStyleSet>(TEXT("LOT_StyleSet"));
	LockOnTargetStyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate/Icons/AssetIcons"));

	LockOnTargetStyleSet->Set("ClassIcon.LockOnTargetComponent",			new IMAGE_BRUSH(TEXT("SpectatorPawn_16x"), Icon16x));
	LockOnTargetStyleSet->Set("ClassThumbnail.LockOnTargetComponent",		new IMAGE_BRUSH(TEXT("SpectatorPawn_64x"), Icon64x));
	LockOnTargetStyleSet->Set("ClassIcon.TargetingHelperComponent",			new IMAGE_BRUSH(TEXT("TargetPoint_16x"), Icon16x));
	LockOnTargetStyleSet->Set("ClassThumbnail.TargetingHelperComponent",	new IMAGE_BRUSH(TEXT("TargetPoint_64x"), Icon64x));
	LockOnTargetStyleSet->Set("ClassIcon.LockOnTargetModuleBase",			new IMAGE_BRUSH(TEXT("MassiveLODOverrideVolume_16x"), Icon16x));
	LockOnTargetStyleSet->Set("ClassThumbnail.LockOnTargetModuleBase",		new IMAGE_BRUSH(TEXT("MassiveLODOverrideVolume_64x"), Icon64x));
	LockOnTargetStyleSet->Set("ClassIcon.TargetHandlerBase",				new IMAGE_BRUSH(TEXT("PoseAsset_16x"), Icon16x));
	LockOnTargetStyleSet->Set("ClassThumbnail.TargetHandlerBase",			new IMAGE_BRUSH(TEXT("PoseAsset_64x"), Icon64x));
	LockOnTargetStyleSet->Set("ClassIcon.RotationModeBase",					new IMAGE_BRUSH(TEXT("RotatingMovementComponent_16x"), Icon16x));
	LockOnTargetStyleSet->Set("ClassThumbnail.RotationModeBase",			new IMAGE_BRUSH(TEXT("RotatingMovementComponent_64x"), Icon64x));

	FSlateStyleRegistry::RegisterSlateStyle(*LockOnTargetStyleSet);
}

void FLockOnTargetEditorModule::UnregisterStyles()
{
	check(LockOnTargetStyleSet.IsValid());
	FSlateStyleRegistry::UnRegisterSlateStyle(*LockOnTargetStyleSet);
}

#undef IMAGE_BRUSH
#undef LOCTEXT_NAMESPACE
