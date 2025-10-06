#include "ProceduralTerrain.h"
#include "DrawDebugHelpers.h"

/*AProceduralTerrain::AProceduralTerrain()
{
    PrimaryActorTick.bCanEverTick = true;

    // Configure noise generators
    ElevationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ElevationNoise.SetFrequency(0.01f);

    MoistureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    MoistureNoise.SetFrequency(0.02f);

    TemperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    TemperatureNoise.SetFrequency(0.02f);
}

void AProceduralTerrain::BeginPlay()
{
    Super::BeginPlay();
    GenerateTerrain();
}

void AProceduralTerrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProceduralTerrain::GenerateTerrain()
{
    for (int32 Y = 0; Y < TerrainHeight; Y++)
    {
        for (int32 X = 0; X < TerrainWidth; X++)
        {
            // Normalize coordinates to [-0.5, 0.5]
            double Nx = static_cast<double>(X) / TerrainWidth - 0.5;
            double Ny = static_cast<double>(Y) / TerrainHeight - 0.5;

            // Elevation (multi-octave)
            double E = ElevationNoise.GetNoise(Nx * 100, Ny * 100) * 0.5 + 0.5;
            E += 0.5 * (ElevationNoise.GetNoise(Nx * 200, Ny * 200) * 0.5 + 0.5);
            E += 0.25 * (ElevationNoise.GetNoise(Nx * 400, Ny * 400) * 0.5 + 0.5);
            E /= (1 + 0.5 + 0.25);
            E = FMath::Pow(E * FudgeFactor, Exponent);

            // Core lock: keep consistent center
            FVector2D Pos(X - TerrainWidth / 2, Y - TerrainHeight / 2);
            if (FVector2D::Distance(Pos, CoreCenter) < CoreRadius)
            {
                E = 0.6f; // plateau in center
            }

            // Moisture & temperature
            float Moisture = MoistureNoise.GetNoise(Nx * 80, Ny * 80) * 0.5f + 0.5f;
            float Temperature = TemperatureNoise.GetNoise(Nx * 60, Ny * 60) * 0.5f + 0.5f;

            // Classify biome
            FString Biome = GetBiome(E, Moisture, Temperature);

            // Debug visualization
            FVector WorldPos = GetActorLocation() + FVector(X * 100, Y * 100, E * 500);
            FColor Color = FColor::White;

            if (Biome == "OCEAN") Color = FColor::Blue;                     // BLÅ
            else if (Biome == "BEACH") Color = FColor::Yellow;              // GUL
            else if (Biome == "SWAMP") Color = FColor(0, 100, 0);           // MÖRK GRÖN
            else if (Biome == "PLAINS") Color = FColor::Green;              // LJUS GRÖN
            else if (Biome == "FOREST") Color = FColor(34, 139, 34);        // MÖRK GRÖN
            else if (Biome == "TAIGA") Color = FColor(0, 128, 128);         // TURKOS
            else if (Biome == "MOUNTAIN") Color = FColor(139, 137, 137);    // GRÅ
            else if (Biome == "TUNDRA") Color = FColor(198, 226, 255);      // LJUS BLÅ
            else if (Biome == "GRUMS") Color = FColor::Red; // The deadliest biome

            DrawDebugPoint(GetWorld(), WorldPos, 8.0f, Color, true);
        }
    }
}

FString AProceduralTerrain::GetBiome(float Elevation, float Moisture, float Temperature)
{
    // Ocean & coast
    if (Elevation < WaterLevel) return "OCEAN";
    if (Elevation > WaterLevel && Elevation < WaterLevel * 1.1f) return "BEACH";

    // Wet lowlands
    if (Moisture > 0.7f && Elevation < 0.3f) return "SWAMP";

    // Grasslands / meadows
    if (Elevation < 0.4f && Moisture > 0.4f) return "PLAINS";

    // Forests
    if (Elevation < 0.6f && Moisture > 0.5f && Temperature > 0.4f) return "FOREST";

    // Taiga / boreal forest (colder, wetter mid-elevations)
    if (Elevation < 0.7f && Moisture > 0.5f && Temperature <= 0.4f) return "TAIGA";

    // Mountains
    if (Elevation >= 0.7f && Elevation < 0.85f) return "MOUNTAIN";

    // High cold plateau
    if (Elevation >= 0.85f && Temperature < 0.5f) return "TUNDRA";

    // The cursed biome of Grums (rare, easter egg condition)
    if (FMath::FRand() < 0.001f) return "GRUMS";

    // Default fallback
    return "PLAINS";
}
*/

#include "ProceduralTerrain.h"
#include "DrawDebugHelpers.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "ProceduralTerrain.h"
#include "DrawDebugHelpers.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

AProceduralTerrain::AProceduralTerrain()
{
    PrimaryActorTick.bCanEverTick = true;

    ElevationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ElevationNoise.SetFrequency(1.0f);

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

    // Convert player pos to chunk coord
    int32 PlayerChunkX = FMath::FloorToInt(PlayerPos.X / ChunkSizeMeters);
    int32 PlayerChunkY = FMath::FloorToInt(PlayerPos.Y / ChunkSizeMeters);

    for (int32 y = 0; y < WorldSizeChunks; y++)
    {
        for (int32 x = 0; x < WorldSizeChunks; x++)
        {
            FVector2D ChunkCoord(x, y);
            FVector ChunkCenter = FVector(
                (x + 0.5f) * ChunkSizeMeters,
                (y + 0.5f) * ChunkSizeMeters,
                0
            );

            float Dist = FVector::Dist2D(PlayerPos, ChunkCenter);

            // Only load chunks within load distance (4x4 accessible, 5th is visual buffer)
            if (Dist <= LoadDistance)
            {
                if (!LoadedChunks.Contains(ChunkCoord))
                {
                    LoadOrGenerateChunk(ChunkCoord);
                    LoadedChunks.Add(ChunkCoord);
                }
            }
        }
    }
}

void AProceduralTerrain::LoadOrGenerateChunk(const FVector2D& ChunkCoord)
{
    TArray<uint8> Data;
    if (LoadChunkFromDisk(ChunkCoord, Data))
    {
        const int NumPoints = PointsPerChunk;

        for (int y = 0; y < NumPoints; y++)
        {
            for (int x = 0; x < NumPoints; x++)
            {
                int idx = y * NumPoints + x;
                if (idx >= Data.Num()) continue;

                // Elevation from saved height
                float Elevation = static_cast<float>(Data[idx]) / 255.0f;

                // Recompute world position
                float wx = ChunkCoord.X * ChunkSizeMeters + (x / (float)NumPoints) * ChunkSizeMeters;
                float wy = ChunkCoord.Y * ChunkSizeMeters + (y / (float)NumPoints) * ChunkSizeMeters;

                // Recompute moisture & temperature (deterministic)
                float Nx = (wx / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
                float Ny = (wy / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
                float Moisture = MoistureNoise.GetNoise(Nx * 80, Ny * 80) * 0.5f + 0.5f;
                float Temperature = TemperatureNoise.GetNoise(Nx * 60, Ny * 60) * 0.5f + 0.5f;

                FString Biome = GetBiome(Elevation, Moisture, Temperature);

                FColor Color = FColor::White;
                if (Biome == "OCEAN") Color = FColor::Blue;
                else if (Biome == "BEACH") Color = FColor::Yellow;
                else if (Biome == "SWAMP") Color = FColor(0, 100, 0);
                else if (Biome == "PLAINS") Color = FColor::Green;
                else if (Biome == "FOREST") Color = FColor(34, 139, 34);
                else if (Biome == "TAIGA") Color = FColor(0, 128, 128);
                else if (Biome == "MOUNTAIN") Color = FColor(139, 137, 137);
                else if (Biome == "TUNDRA") Color = FColor(198, 226, 255);
                else if (Biome == "GRUMS") Color = FColor::Red;

                FVector WorldPos(wx, wy, Elevation * 1500.0f); // exaggerate vertical scale
                DrawDebugPoint(GetWorld(), WorldPos, 8.0f, Color, true);
            }
        }
    }
    else
    {
        GenerateChunk(ChunkCoord);
    }
}


void AProceduralTerrain::GenerateChunk(const FVector2D& ChunkCoord)
{
    const int NumPoints = PointsPerChunk;
    TArray<uint8> Data;
    Data.SetNum(NumPoints * NumPoints);

    // Identify core chunk
    FVector2D CoreChunk(WorldSizeChunks / 2, WorldSizeChunks / 2);
    // float CoreRadius; // meters

    for (int y = 0; y < NumPoints; y++)
    {
        for (int x = 0; x < NumPoints; x++)
        {
            float wx = ChunkCoord.X * ChunkSizeMeters + (x / (float)NumPoints) * ChunkSizeMeters;
            float wy = ChunkCoord.Y * ChunkSizeMeters + (y / (float)NumPoints) * ChunkSizeMeters;

            // Base coordinates normalized to [-0.5, 0.5]
            float Nx = (wx / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;
            float Ny = (wy / (WorldSizeChunks * ChunkSizeMeters)) - 0.5f;

            // Multi-octave elevation noise for more interesting terrain
            float E = ElevationNoise.GetNoise(Nx * 100, Ny * 100) * 0.5f + 0.5f;
            E += 0.5f * (ElevationNoise.GetNoise(Nx * 200, Ny * 200) * 0.5f + 0.5f);
            E += 0.25f * (ElevationNoise.GetNoise(Nx * 400, Ny * 400) * 0.5f + 0.5f);
            E /= (1 + 0.5f + 0.25f);

            // Apply fudge/exponent
            float Elevation = FMath::Pow(E * FudgeFactor, Exponent);
            Elevation = FMath::Clamp(Elevation, 0.0f, 1.0f);


            // Flatten core center if this is the core chunk
            if (ChunkCoord == CoreChunk)
            {
                float ChunkCenterX = CoreChunk.X * ChunkSizeMeters + ChunkSizeMeters / 2.0f;
                float ChunkCenterY = CoreChunk.Y * ChunkSizeMeters + ChunkSizeMeters / 2.0f;
                float DistToCenter = FVector2D(wx - ChunkCenterX, wy - ChunkCenterY).Size();

                if (DistToCenter <= CoreRadius)
                {
                    Elevation = 0.6f;
                }
                else if (DistToCenter <= CoreRadius * 1.2f)
                {
                    float t = (DistToCenter - CoreRadius) / (CoreRadius * 0.2f);
                    float NoiseVal = ElevationNoise.GetNoise(Nx, Ny) * 0.5f + 0.5f;
                    Elevation = FMath::Lerp(0.6f, NoiseVal, t);
                }
            }

            Elevation = FMath::Clamp(Elevation, 0.0f, 1.0f);

            // Save elevation as byte
            Data[y * NumPoints + x] = (uint8)(Elevation * 255.0f);

            // Moisture & temperature for biome
            float Moisture = MoistureNoise.GetNoise(Nx * 80, Ny * 80) * 0.5f + 0.5f;
            float Temperature = TemperatureNoise.GetNoise(Nx * 60, Ny * 60) * 0.5f + 0.5f;

            FString Biome = GetBiome(Elevation, Moisture, Temperature);

            FColor Color = FColor::White;
            if (Biome == "OCEAN") Color = FColor::Blue;
            else if (Biome == "BEACH") Color = FColor::Yellow;
            else if (Biome == "SWAMP") Color = FColor(0, 100, 0);
            else if (Biome == "PLAINS") Color = FColor::Green;
            else if (Biome == "FOREST") Color = FColor(34, 139, 34);
            else if (Biome == "TAIGA") Color = FColor(0, 128, 128);
            else if (Biome == "MOUNTAIN") Color = FColor(139, 137, 137);
            else if (Biome == "TUNDRA") Color = FColor(198, 226, 255);
            else if (Biome == "GRUMS") Color = FColor::Red;

            FVector WorldPos(wx, wy, Elevation * 1500.0f); // exaggerate vertical scale

            DrawDebugPoint(GetWorld(), WorldPos, 8.0f, Color, true);
        }
    }

    SaveChunkToDisk(ChunkCoord, Data);
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
    return FPaths::ProjectDir() / ChunkSaveFolder /
    FString::Printf(TEXT("chunk_%d_%d.bin"), (int)ChunkCoord.X, (int)ChunkCoord.Y);
}



FString AProceduralTerrain::GetBiome(float Elevation, float Moisture, float Temperature)
{
    // Ocean & coast
    if (Elevation < WaterLevel) return "OCEAN";
    if (Elevation > WaterLevel && Elevation < WaterLevel * 1.1f) return "BEACH";

    // Wet lowlands
    if (Moisture > 0.7f && Elevation < 0.3f) return "SWAMP";

    // Grasslands / meadows
    if (Elevation < 0.4f && Moisture > 0.4f) return "PLAINS";

    // Forests
    if (Elevation < 0.6f && Moisture > 0.5f && Temperature > 0.4f) return "FOREST";

    // Taiga / boreal forest (colder, wetter mid-elevations)
    if (Elevation < 0.7f && Moisture > 0.5f && Temperature <= 0.4f) return "TAIGA";

    // Mountains
    if (Elevation >= 0.7f && Elevation < 0.85f) return "MOUNTAIN";

    // High cold plateau
    if (Elevation >= 0.85f && Temperature < 0.5f) return "TUNDRA";

    // The cursed biome of Grums (rare, easter egg condition)
    if (FMath::FRand() < 0.001f) return "GRUMS";

    // Default fallback
    return "PLAINS";
}