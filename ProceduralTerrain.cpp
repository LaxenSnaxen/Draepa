/*
#include "ProceduralTerrain.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

AProceduralTerrain::AProceduralTerrain()
{
    PrimaryActorTick.bCanEverTick = true;

    ElevationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ElevationNoise.SetFrequency(Frequency);

    MoistureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    MoistureNoise.SetFrequency(0.002f);

    TemperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    TemperatureNoise.SetFrequency(0.002f);
}

void AProceduralTerrain::BeginPlay()
{
    Super::BeginPlay();
    UpdateLoadedChunks();
}

void AProceduralTerrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateLoadedChunks();
}

void AProceduralTerrain::UpdateLoadedChunks()
{
    FVector PlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    int32 PlayerChunkX = FMath::FloorToInt(PlayerPos.X / ChunkSizeMeters);
    int32 PlayerChunkY = FMath::FloorToInt(PlayerPos.Y / ChunkSizeMeters);

    for (int32 y = 0; y < WorldSizeChunks; y++)
    {
        for (int32 x = 0; x < WorldSizeChunks; x++)
        {
            FVector2D ChunkCoord(x, y);
            FVector ChunkCenter = FVector((x + 0.5f) * ChunkSizeMeters, (y + 0.5f) * ChunkSizeMeters, 0);
            float Dist = FVector::Dist2D(PlayerPos, ChunkCenter);

            if (Dist <= LoadDistance && !LoadedChunks.Contains(ChunkCoord))
            {
                TArray<uint8> Data;
                if (!LoadChunkFromDisk(ChunkCoord, Data))
                {
                    GenerateChunk(ChunkCoord, Data);
                    SaveChunkToDisk(ChunkCoord, Data);
                }
                BuildChunkMesh(ChunkCoord, Data);
                LoadedChunks.Add(ChunkCoord);
            }
        }
    }
}

void AProceduralTerrain::GenerateChunk(const FVector2D& ChunkCoord, TArray<uint8>& OutData)
{
    const int NumPoints = PointsPerChunk;
    OutData.SetNum(NumPoints * NumPoints);

    for (int y = 0; y < NumPoints; y++)
    {
        for (int x = 0; x < NumPoints; x++)
        {
            float wx = ChunkCoord.X * ChunkSizeMeters + (x / (float)(NumPoints - 1)) * ChunkSizeMeters;
            float wy = ChunkCoord.Y * ChunkSizeMeters + (y / (float)(NumPoints - 1)) * ChunkSizeMeters;
            float Nx = (wx / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
            float Ny = (wy / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;

            float E = ElevationNoise.GetNoise(Nx * 100, Ny * 100) * 0.5f + 0.5f;
            E += 0.5f * (ElevationNoise.GetNoise(Nx * 200, Ny * 200) * 0.5f + 0.5f);
            E += 0.25f * (ElevationNoise.GetNoise(Nx * 400, Ny * 400) * 0.5f + 0.5f);
            E /= (1.0f + 0.5f + 0.25f);
            float Elevation = FMath::Pow(E * FudgeFactor, Exponent);
            OutData[y * NumPoints + x] = (uint8)(FMath::Clamp(Elevation, 0.0f, 1.0f) * 255.0f);
        }
    }
}

void AProceduralTerrain::BuildChunkMesh(const FVector2D& ChunkCoord, const TArray<uint8>& HeightData)
{
    const int NumPoints = PointsPerChunk;
    const float Step = ChunkSizeMeters / (float)(NumPoints - 1);

    FString Name = FString::Printf(TEXT("ChunkMesh_%d_%d"), (int)ChunkCoord.X, (int)ChunkCoord.Y);
    UProceduralMeshComponent* Mesh = NewObject<UProceduralMeshComponent>(this, *Name);
    Mesh->RegisterComponent();
    Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    ActiveChunkMeshes.Add({ChunkCoord, Mesh});

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> Colors;

    Vertices.Reserve(NumPoints * NumPoints);

    for (int y = 0; y < NumPoints; y++)
    {
        for (int x = 0; x < NumPoints; x++)
        {
            float Elev = HeightData[y * NumPoints + x] / 255.0f;
            float wx = ChunkCoord.X * ChunkSizeMeters + x * Step;
            float wy = ChunkCoord.Y * ChunkSizeMeters + y * Step;

            // Sample biome noise again
            float Nx = (wx / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
            float Ny = (wy / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
            float Moisture = MoistureNoise.GetNoise(Nx * 80, Ny * 80) * 0.5f + 0.5f;
            float Temperature = TemperatureNoise.GetNoise(Nx * 60, Ny * 60) * 0.5f + 0.5f;

            FString Biome = GetBiome(Elev, Moisture, Temperature);
            FLinearColor Color = GetBiomeColor(Biome);

            Vertices.Add(FVector(wx, wy, Elev * HeightScale));
            UVs.Add(FVector2D((float)x / (NumPoints - 1), (float)y / (NumPoints - 1)));
            Colors.Add(Color);
        }
    }

    for (int y = 0; y < NumPoints - 1; y++)
    {
        for (int x = 0; x < NumPoints - 1; x++)
        {
            int i0 = y * NumPoints + x;
            int i1 = y * NumPoints + x + 1;
            int i2 = (y + 1) * NumPoints + x;
            int i3 = (y + 1) * NumPoints + x + 1;

            Triangles.Add(i0); Triangles.Add(i2); Triangles.Add(i1);
            Triangles.Add(i1); Triangles.Add(i2); Triangles.Add(i3);
        }
    }

    Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, {}, UVs, Colors, {}, true);
    Mesh->bUseComplexAsSimpleCollision = true;
    Mesh->ContainsPhysicsTriMeshData(true);
    Mesh->SetMobility(EComponentMobility::Static);
    Mesh->SetMaterial(0, nullptr);
}

bool AProceduralTerrain::LoadChunkFromDisk(const FVector2D& ChunkCoord, TArray<uint8>& OutData)
{
    FString FilePath = GetChunkFilename(ChunkCoord);
    return FFileHelper::LoadFileToArray(OutData, *FilePath);
}

void AProceduralTerrain::SaveChunkToDisk(const FVector2D& ChunkCoord, const TArray<uint8>& Data)
{
    FString FilePath = GetChunkFilename(ChunkCoord);
    IFileManager::Get().MakeDirectory(*FPaths::ProjectDir().Append(ChunkSaveFolder), true);
    FFileHelper::SaveArrayToFile(Data, *FilePath);
}

FString AProceduralTerrain::GetChunkFilename(const FVector2D& ChunkCoord) const
{
    return FPaths::ProjectDir() / ChunkSaveFolder / FString::Printf(TEXT("chunk_%d_%d.bin"), (int)ChunkCoord.X, (int)ChunkCoord.Y);
}

// === Biome logic ===
FString AProceduralTerrain::GetBiome(float Elevation, float Moisture, float Temperature)
{
    if (Elevation < WaterLevel) return "OCEAN";
    if (Elevation < WaterLevel * 1.1f) return "BEACH";
    if (Moisture > 0.7f && Elevation < 0.3f) return "SWAMP";
    if (Elevation < 0.4f && Moisture > 0.4f) return "PLAINS";
    if (Elevation < 0.6f && Moisture > 0.5f && Temperature > 0.4f) return "FOREST";
    if (Elevation < 0.7f && Moisture > 0.5f && Temperature <= 0.4f) return "TAIGA";
    if (Elevation >= 0.7f && Elevation < 0.85f) return "MOUNTAIN";
    if (Elevation >= 0.85f && Temperature < 0.5f) return "TUNDRA";
    if (FMath::FRand() < 0.001f) return "GRUMS";
    return "PLAINS";
}

FLinearColor AProceduralTerrain::GetBiomeColor(const FString& Biome)
{
    if (Biome == "OCEAN") return FLinearColor(0.0f, 0.2f, 1.0f);
    if (Biome == "BEACH") return FLinearColor(1.0f, 1.0f, 0.5f);
    if (Biome == "SWAMP") return FLinearColor(0.0f, 0.3f, 0.0f);
    if (Biome == "PLAINS") return FLinearColor(0.2f, 0.8f, 0.2f);
    if (Biome == "FOREST") return FLinearColor(0.0f, 0.5f, 0.0f);
    if (Biome == "TAIGA") return FLinearColor(0.0f, 0.6f, 0.6f);
    if (Biome == "MOUNTAIN") return FLinearColor(0.5f, 0.5f, 0.5f);
    if (Biome == "TUNDRA") return FLinearColor(0.8f, 0.9f, 1.0f);
    if (Biome == "GRUMS") return FLinearColor::Red;
    return FLinearColor::White;
}
*/
#include "ProceduralTerrain.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/GameplayStatics.h"

AProceduralTerrain::AProceduralTerrain()
{
    PrimaryActorTick.bCanEverTick = true;

    // Noise setup
    ElevationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ElevationNoise.SetFrequency(Frequency);

    MoistureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    MoistureNoise.SetFrequency(MoistureFrequency);

    TemperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    TemperatureNoise.SetFrequency(TemperatureFrequency);
}

void AProceduralTerrain::BeginPlay()
{
    Super::BeginPlay();
    UpdateLoadedChunks();
}

void AProceduralTerrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateLoadedChunks();
}

void AProceduralTerrain::UpdateLoadedChunks()
{
    FVector PlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    int32 PlayerChunkX = FMath::FloorToInt(PlayerPos.X / ChunkSizeMeters);
    int32 PlayerChunkY = FMath::FloorToInt(PlayerPos.Y / ChunkSizeMeters);

    for (int32 y = 0; y < WorldSizeChunks; y++)
    {
        for (int32 x = 0; x < WorldSizeChunks; x++)
        {
            FVector2D ChunkCoord(x, y);
            FVector ChunkCenter((x + 0.5f) * ChunkSizeMeters, (y + 0.5f) * ChunkSizeMeters, 0);

            float Dist = FVector::Dist2D(PlayerPos, ChunkCenter);
            if (Dist <= LoadDistance && !LoadedChunks.Contains(ChunkCoord))
            {
                TArray<uint8> Data;
                GenerateChunk(ChunkCoord, Data);
                BuildChunkMesh(ChunkCoord, Data);
                LoadedChunks.Add(ChunkCoord);
            }
        }
    }
}

void AProceduralTerrain::GenerateChunk(const FVector2D& ChunkCoord, TArray<uint8>& OutData)
{
    const int NumPoints = PointsPerChunk;
    OutData.SetNum(NumPoints * NumPoints);

    for (int y = 0; y < NumPoints; y++)
    {
        for (int x = 0; x < NumPoints; x++)
        {
            float wx = ChunkCoord.X * ChunkSizeMeters + (x / (float)(NumPoints - 1)) * ChunkSizeMeters;
            float wy = ChunkCoord.Y * ChunkSizeMeters + (y / (float)(NumPoints - 1)) * ChunkSizeMeters;
            float Nx = wx / (WorldSizeChunks * ChunkSizeMeters) - 0.5f;
            float Ny = wy / (WorldSizeChunks * ChunkSizeMeters) - 0.5f;

            float E = ElevationNoise.GetNoise(Nx * 100, Ny * 100) * 0.5f + 0.5f;
            E += 0.5f * (ElevationNoise.GetNoise(Nx * 200, Ny * 200) * 0.5f + 0.5f);
            E += 0.25f * (ElevationNoise.GetNoise(Nx * 400, Ny * 400) * 0.5f + 0.5f);
            E /= (1.0f + 0.5f + 0.25f);

            float Elevation = FMath::Pow(E * FudgeFactor, Exponent);
            OutData[y * NumPoints + x] = (uint8)(FMath::Clamp(Elevation, 0.0f, 1.0f) * 255.0f);
        }
    }
}

void AProceduralTerrain::BuildChunkMesh(const FVector2D& ChunkCoord, const TArray<uint8>& HeightData)
{
    const int NumPoints = PointsPerChunk;
    const float Step = ChunkSizeMeters / (float)(NumPoints - 1);

    FString MeshName = FString::Printf(TEXT("ChunkMesh_%d_%d"), (int)ChunkCoord.X, (int)ChunkCoord.Y);
    UProceduralMeshComponent* Mesh = NewObject<UProceduralMeshComponent>(this, *MeshName);
    Mesh->RegisterComponent();
    Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    ActiveChunkMeshes.Add({ChunkCoord, Mesh});

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> Colors;

    Vertices.Reserve(NumPoints * NumPoints);

    for (int y = 0; y < NumPoints; y++)
    {
        for (int x = 0; x < NumPoints; x++)
        {
            float Elev = HeightData[y * NumPoints + x] / 255.0f;
            float wx = ChunkCoord.X * ChunkSizeMeters + x * Step;
            float wy = ChunkCoord.Y * ChunkSizeMeters + y * Step;

            float Nx = wx / (WorldSizeChunks * ChunkSizeMeters) - 0.5f;
            float Ny = wy / (WorldSizeChunks * ChunkSizeMeters) - 0.5f;
            float Moisture = MoistureNoise.GetNoise(Nx * 80, Ny * 80) * 0.5f + 0.5f;
            float Temperature = TemperatureNoise.GetNoise(Nx * 60, Ny * 60) * 0.5f + 0.5f;

            FString Biome = GetBiome(Elev, Moisture, Temperature);

            // Assign smooth biome weights (so texture transitions are soft)
            FLinearColor BiomeWeights(0, 0, 0, 0);
            if (Biome == "OCEAN" || Biome == "BEACH")
            {
                BiomeWeights.R = 1.0f; // Sand
            }
            else if (Biome == "PLAINS" || Biome == "FOREST")
            {
                BiomeWeights.G = 1.0f; // Grass
            }
            else if (Biome == "MOUNTAIN" || Biome == "TAIGA")
            {
                BiomeWeights.B = 1.0f; // Rock
            }
            else if (Biome == "TUNDRA")
            {
                BiomeWeights.A = 1.0f; // Snow
            }

            Vertices.Add(FVector(wx, wy, Elev * HeightScale));
            UVs.Add(FVector2D((float)x / (NumPoints - 1), (float)y / (NumPoints - 1)));
            Colors.Add(BiomeWeights);
        }
    }

    for (int y = 0; y < NumPoints - 1; y++)
    {
        for (int x = 0; x < NumPoints - 1; x++)
        {
            int i0 = y * NumPoints + x;
            int i1 = y * NumPoints + x + 1;
            int i2 = (y + 1) * NumPoints + x;
            int i3 = (y + 1) * NumPoints + x + 1;

            // Correct winding order (clockwise when viewed from above)
            Triangles.Add(i0);
            Triangles.Add(i2);
            Triangles.Add(i1);

            Triangles.Add(i1);
            Triangles.Add(i2);
            Triangles.Add(i3);
        }
    }

    Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, {}, UVs, Colors, {}, true);

    // Apply Quixel material
    if (TerrainMaterial)
    {
        Mesh->SetMaterial(0, TerrainMaterial);
    }

    Mesh->bUseComplexAsSimpleCollision = true;
    Mesh->SetMobility(EComponentMobility::Static);
}

FString AProceduralTerrain::GetBiome(float Elevation, float Moisture, float Temperature) const
{
    if (Elevation < WaterLevel * 0.9f) return "OCEAN";
    if (Elevation < WaterLevel * 1.05f) return "BEACH";
    if (Elevation < 0.5f && Moisture > 0.5f) return "PLAINS";
    if (Elevation < 0.7f && Moisture > 0.4f && Temperature > 0.4f) return "FOREST";
    if (Elevation < 0.8f) return "MOUNTAIN";
    if (Elevation >= 0.8f && Temperature < 0.5f) return "TUNDRA";
    return "PLAINS";
}
