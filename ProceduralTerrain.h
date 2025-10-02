#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseLite.h" // include FastNoiseLite header
#include "ProceduralTerrain.generated.h"

UCLASS()
class DRAEPA_API AProceduralTerrain : public AActor
{
    GENERATED_BODY()
    
public:	
    AProceduralTerrain();

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;

    // === Terrain parameters ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int32 TerrainWidth = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int32 TerrainHeight = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float Frequency = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float Exponent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float FudgeFactor = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float WaterLevel = 0.4f;

    // Core area that must remain the same across levels
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    FVector2D CoreCenter = FVector2D(0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float CoreRadius = 50.0f;

private:
    FastNoiseLite ElevationNoise;
    FastNoiseLite MoistureNoise;
    FastNoiseLite TemperatureNoise;

    void GenerateTerrain();

    FString GetBiome(float Elevation, float Moisture, float Temperature);
};
