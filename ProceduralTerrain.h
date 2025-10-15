/*
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseLite.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrain.generated.h"

USTRUCT()
struct FChunkMesh
{
    GENERATED_BODY()
    FVector2D ChunkCoord;
    UProceduralMeshComponent* Mesh = nullptr;
};

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

    // === Terrain and chunk settings ===
    UPROPERTY(EditAnywhere, Category = "Terrain|Chunks")
    int32 ChunkSizeMeters = 1000;

    UPROPERTY(EditAnywhere, Category = "Terrain|Chunks")
    int32 PointsPerChunk = 64;

    UPROPERTY(EditAnywhere, Category = "Terrain|World")
    int32 WorldSizeChunks = 5;

    UPROPERTY(EditAnywhere, Category = "Terrain|Streaming")
    float LoadDistance = 2500.0f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Streaming")
    FString ChunkSaveFolder = TEXT("Saved/Chunks");

    // === Noise and shaping ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float Frequency = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float Exponent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float FudgeFactor = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float WaterLevel = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float HeightScale = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float CoreRadius = 100.0f;

private:
    FastNoiseLite ElevationNoise;
    FastNoiseLite MoistureNoise;
    FastNoiseLite TemperatureNoise;

    TSet<FVector2D> LoadedChunks;
    TArray<FChunkMesh> ActiveChunkMeshes;

    void UpdateLoadedChunks();
    void LoadOrGenerateChunk(const FVector2D& ChunkCoord);
    void GenerateChunk(const FVector2D& ChunkCoord, TArray<uint8>& OutData);
    void BuildChunkMesh(const FVector2D& ChunkCoord, const TArray<uint8>& HeightData);

    FString GetBiome(float Elevation, float Moisture, float Temperature);
    FLinearColor GetBiomeColor(const FString& Biome);

    FString GetChunkFilename(const FVector2D& ChunkCoord) const;
    bool LoadChunkFromDisk(const FVector2D& ChunkCoord, TArray<uint8>& OutData);
    void SaveChunkToDisk(const FVector2D& ChunkCoord, const TArray<uint8>& Data);
};
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseLite.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrain.generated.h"

USTRUCT()
struct FChunkMesh
{
    GENERATED_BODY()
    FVector2D ChunkCoord;
    UProceduralMeshComponent* Mesh = nullptr;
};

UCLASS()
class DRAEPA_API AProceduralTerrain : public AActor
{
    GENERATED_BODY()

public:
    AProceduralTerrain();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // === TERRAIN SETTINGS ===
    UPROPERTY(EditAnywhere, Category = "Terrain|Generation")
    int32 ChunkSizeMeters = 1000;

    UPROPERTY(EditAnywhere, Category = "Terrain|Generation")
    int32 PointsPerChunk = 64;

    UPROPERTY(EditAnywhere, Category = "Terrain|Streaming")
    int32 WorldSizeChunks = 5;

    UPROPERTY(EditAnywhere, Category = "Terrain|Streaming")
    float LoadDistance = 2500.0f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Height")
    float HeightScale = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Noise")
    float Frequency = 0.02f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Noise")
    float FudgeFactor = 1.2f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Noise")
    float Exponent = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Biome")
    float WaterLevel = 0.4f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Biome")
    float MoistureFrequency = 0.002f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Biome")
    float TemperatureFrequency = 0.002f;

    // === MATERIAL ===
    UPROPERTY(EditAnywhere, Category = "Terrain|Material")
    UMaterialInterface* TerrainMaterial;

private:
    FastNoiseLite ElevationNoise;
    FastNoiseLite MoistureNoise;
    FastNoiseLite TemperatureNoise;

    TSet<FVector2D> LoadedChunks;
    TArray<FChunkMesh> ActiveChunkMeshes;

    void UpdateLoadedChunks();
    void LoadOrGenerateChunk(const FVector2D& ChunkCoord);
    void GenerateChunk(const FVector2D& ChunkCoord, TArray<uint8>& OutData);
    void BuildChunkMesh(const FVector2D& ChunkCoord, const TArray<uint8>& HeightData);

    FString GetBiome(float Elevation, float Moisture, float Temperature) const;
};
