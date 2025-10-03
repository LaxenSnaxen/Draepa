#include "ProceduralTerrain.h"
#include "DrawDebugHelpers.h"

AProceduralTerrain::AProceduralTerrain()
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
                E = 0.5f; // plateau in center
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
            else if (Biome == "SWAMP") Color = FColor(255, 140, 0);           // ORANGE
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
    if (Moisture > 0.7f && Elevation < 0.4f) return "SWAMP";

    // Grasslands / meadows
    if (Elevation < 0.5f && Moisture > 0.4f) return "PLAINS";

    // Forests
    if (Elevation < 0.6f && Moisture > 0.5f && Temperature > 0.4f) return "FOREST";

    // Taiga / boreal forest (colder, wetter mid-elevations)
    if (Elevation < 0.75f && Moisture > 0.5f && Temperature <= 0.4f) return "TAIGA";

    // Mountains
    if (Elevation >= 0.9f && Elevation < 0.85f) return "MOUNTAIN";

    // High cold plateau
    if (Elevation >= 0.85f && Temperature < 0.5f) return "TUNDRA";

    // The cursed biome of Grums (rare, easter egg condition)
    if (FMath::FRand() < 0.001f) return "GRUMS";

    // Default fallback
    return "PLAINS";
}
