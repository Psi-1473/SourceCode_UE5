// Fill out your copyright notice in the Description page of Project Settings.


#include "MapExtractor.h"
#include "Landscape.h"
#include "LandscapeComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

void UMapExtractor::ExportZCooldinates(UWorld* World, float GridSize)
{

    // 이 함수를 약간 변형
    // -> (0,0) 그리드에 (-25100, 25100)의 z 값 저장하게끔 유도
    // -> (-25100, 25100)값부터 순서대로 텍스트로 저장
    // -> y값이 떨어질 때마다 line 변경
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null!"));
        return;
    }

    TArray<FString> Lines;

    const int Offset = 50;
    int StartX = -25150 + Offset;
    int StartY = -25150 + Offset;
    int EndX = 25150 - Offset;
    int EndY = 25150 - Offset;
    
    

    for (StartY; StartY <= EndY; StartY += 100)
    {
        for (int X = -25100; X <= EndX; X += 100)
        {
            FVector XYCoordinates = FVector(X, StartY, 0.0f);
            FVector Start = XYCoordinates;
            Start.Z = 10000.0f;
            FVector End = XYCoordinates;
            End.Z = -10000.0f;

            FHitResult HitResult;
            FCollisionQueryParams CollisionParams;

            if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
            {
                FVector Location = HitResult.Location;
                Lines.Add(FString::Printf(TEXT("%f"), Location.Z));
            }
            else
            {
                Lines.Add(FString::Printf(TEXT("0.0")));
            }
        }
    }

    FString FilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("../Server/MapFiles/ZCoordinates.txt"));
    FFileHelper::SaveStringArrayToFile(Lines, *FilePath);

    UE_LOG(LogTemp, Log, TEXT("Z coordinates saved to %s"), *FilePath);
}

void UMapExtractor::ExtractHeightmapData(ALandscape* Landscape, TArray<float>& OutHeightmapData, int32& Width, int32& Height)
{
    if (!Landscape)
    {
        return;
    }

    TArray<ULandscapeComponent*> LandscapeComponents;
    Landscape->GetComponents(LandscapeComponents);

    if (LandscapeComponents.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No landscape components found."));
        return;
    }

    UTexture2D* HeightmapTexture = nullptr;
    for (ULandscapeComponent* Component : LandscapeComponents)
    {
        if (Component)
        {
            HeightmapTexture = Component->GetHeightmap();
            if (HeightmapTexture)
            {
                break; // Found the heightmap texture, no need to continue
            }
        }
    }

    if (!HeightmapTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("No heightmap texture found."));
        return;
    }

    Width = HeightmapTexture->GetSizeX();
    Height = HeightmapTexture->GetSizeY();

    int32 DataSize = Width * Height;

    TArray<uint8> RawData;
    if (HeightmapTexture->PlatformData->Mips.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No mip data found in heightmap texture."));
        return;
    }


    FByteBulkData& MipBulkData = HeightmapTexture->PlatformData->Mips[0].BulkData;
    if (MipBulkData.GetBulkDataSize() < DataSize)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mip bulk data size is smaller than expected: %d, %d, %d"), MipBulkData.GetBulkDataSize(), Width, Height);
        return;
    }

    RawData.SetNumUninitialized(DataSize);
    MipBulkData.GetCopy((void**)&RawData);

    if (Width <= 0 || Height <= 0 || RawData.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid width, height, or RawData size."));
        return;
    }

    OutHeightmapData.SetNumUninitialized(DataSize);

    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            int32 Index = Y * Width + X;
            if (Index < 0 || Index >= RawData.Num())
            {
                UE_LOG(LogTemp, Warning, TEXT("Index out of bounds: %d"), Index);
                return;
            }
            OutHeightmapData[Index] = RawData[Index] / 255.0f; // Normalize height value
        }
    }
}

void UMapExtractor::SaveHeightmapToCSV(const TArray<float>& HeightmapData, int32 Width, int32 Height, const FString& FilePath)
{
    FString OutputString;
    
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            int32 Index = Y * Width + X;
            OutputString += FString::Printf(TEXT("%f,"), HeightmapData[Index]);
        }
        OutputString += TEXT("\n");
    }
    
    FFileHelper::SaveStringToFile(OutputString, *FilePath);
}
