// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapExtractor.generated.h"

/**
 * 
 */

class ALandscape;

UCLASS()
class MYRPG_API UMapExtractor : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Map Extraction")
	void ExportZCooldinates(UWorld* World, float GridSize);

private:
	void ExtractHeightmapData(ALandscape* Landscape, TArray<float>& OutHeightmapData, int32& Width, int32& Height);
	void SaveHeightmapToCSV(const TArray<float>& HeightmapData, int32 Width, int32 Height, const FString& FilePath);
};
