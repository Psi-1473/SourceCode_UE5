// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyRpg.h"
//#include "LevelEditor.h"
//#include "Toolkits/ToolkitManager.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/Commands.h"
#include "Misc/MessageDialog.h"
#include "Engine/World.h"
#include "Landscape.h"
#include "EngineUtils.h"
#include "MapExtractor.h"  // UMapExtractor 클래스 포함

// 개발이 완료되면 주석 해제할 것

// 1. 이거 주석하면 라이브 코딩 안된다.
// 2. IMPLEMENT_MODULE(MyEditorToolbarModule, MyRpg) 이걸 대신 쓰고 있었는데 .. 이걸 쓰니까 라이브 코딩이 안됐음
// 3. 대체 IMPLEMENT 매크로가 뭔지 공부할 것
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MyRpg, "MyRpg" );
//IMPLEMENT_MODULE(MyRpgbarModule, MyRpg)

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}

/*
	모듈
*/
//#define LOCTEXT_NAMESPACE "FMyEditorToolbarModule"
//void MyRpgbarModule::StartupModule()
//{
//	UE_LOG(LogTemp, Warning, TEXT("MyRpg Module Startup"));
//	TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);
//	MainMenuExtender->AddMenuExtension("LevelEditor",
//		EExtensionHook::After,
//		NULL,
//		FMenuExtensionDelegate::CreateRaw(this, &MyRpgbarModule::AddMenuExtension));
//
//	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
//	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
//}
//
//void MyRpgbarModule::ShutdownModule()
//{
//}
//
//void MyRpgbarModule::AddMenuExtension(FMenuBuilder& MenuBuilder)
//{
//	MenuBuilder.BeginSection("MyRpgEditor", LOCTEXT("MyRpgEditor", "My Editor"));
//	MenuBuilder.AddMenuEntry(
//		LOCTEXT("Export Height Map", "Export Height Map"),
//		LOCTEXT("Export Height Map", "Export height map from landscape"),
//		FSlateIcon(),
//		FUIAction(FExecuteAction::CreateRaw(this, &MyRpgbarModule::OnExportHeightMap))
//	);
//	MenuBuilder.EndSection();
//}
//
//void MyRpgbarModule::OnExportHeightMap()
//{
//	UWorld* World = GEditor->GetEditorWorldContext().World();
//	if (!World)
//	{
//		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("No valid world found."));
//		return;
//	}
//
//	// Find the first landscape actor
//	for (TActorIterator<ALandscape> It(World); It; ++It)
//	{
//		ALandscape* Landscape = *It;
//		if (Landscape)
//		{
//			// Create an instance of UMapExtractor and call ExportHeightmapData
//			UMapExtractor* MapExtractor = NewObject<UMapExtractor>();
//			MapExtractor->ExportZCooldinates(World, 503);
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Heightmap data exported successfully."));
//			break;
//		}
//	}
//}
//
//#undef LOCTEXT_NAMESPACE
