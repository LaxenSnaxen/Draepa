#include "VoronoiTerrainActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

// Constructor
AVoronoiTerrainActor::AVoronoiTerrainActor()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void AVoronoiTerrainActor::OnConstruction(const FTransform& Transform)
{
    GenerateMesh();
}

// -------------------- Noise Functions --------------------

float AVoronoiTerrainActor::PerlinNoise2D(const FVector2D& P) const
{
    return FMath::PerlinNoise2D(P);
}

float AVoronoiTerrainActor::FBmNoise2D(const FVector2D& P, int Octaves, float Persistence) const
{
    float Total = 0.f;
    float Frequency = 1.f;
    float Amp = 1.f;   // renamed to avoid conflict
    float MaxValue = 0.f;

    for (int i = 0; i < Octaves; i++)
    {
        Total += FMath::PerlinNoise2D(P * Frequency) * Amp;
        MaxValue += Amp;
        Amp *= Persistence;
        Frequency *= 2.f;
    }

    return Total / MaxValue;
}

float AVoronoiTerrainActor::WorleyF1(const FVector2D& P) const
{
    FRandomStream Rand(Seed);
    float CellW = NumQuadsX * QuadSize / CellsX;
    float CellH = NumQuadsY * QuadSize / CellsY;

    int CellX = FMath::FloorToInt(P.X / CellW);
    int CellY = FMath::FloorToInt(P.Y / CellH);

    float MinDist = FLT_MAX;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int NeighborX = CellX + dx;
            int NeighborY = CellY + dy;

            Rand.Initialize(Seed + NeighborX * 92821 + NeighborY * 68917);
            FVector2D FeaturePoint(
                (NeighborX + Rand.FRand()) * CellW,
                (NeighborY + Rand.FRand()) * CellH
            );

            float Dist = FVector2D::Distance(P, FeaturePoint);
            MinDist = FMath::Min(MinDist, Dist);
        }
    }

    return MinDist / FMath::Max(CellW, CellH);
}

// -------------------- Terrain Generation --------------------

float AVoronoiTerrainActor::GetHeightAt(const FVector2D& Pos) const
{
        // 1) Large-scale Voronoi for basins & ridges
    float voro = WorleyF1(Pos * 0.001f); // very low frequency → big catchments
    voro = FMath::Pow(1.0f - voro, RidgeSharpness); // emphasize ridges/valleys

    // 2) Medium-scale Perlin for hills
    float perlin = PerlinNoise2D(Pos * PerlinScale);

    // 3) fBm for detail (small variations, rocky/marshy ground)
    float fbm = FBmNoise2D(Pos * FbmScale, FbmOctaves, FbmPersistence);

    // 4) Domain warping (use fBm to distort Perlin coords → more variation)
    FVector2D warpedPos = Pos * PerlinScale + FVector2D(fbm * 20.0f, fbm * 20.0f);
    float warpedPerlin = PerlinNoise2D(warpedPos);

    // Blend weights
    float height01 = VoronoiWeight * voro
                   + PerlinWeight * warpedPerlin
                   + FbmWeight * fbm;

    // Normalize (optional)
    //height01 = FMath::Clamp(height01, -1.0f, 1.0f);

    // Apply amplitude
    return height01 * Amplitude;
}

void AVoronoiTerrainActor::GenerateMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    int32 NumVertsX = NumQuadsX + 1;
    int32 NumVertsY = NumQuadsY + 1;

    Vertices.SetNum(NumVertsX * NumVertsY);
    UVs.SetNum(NumVertsX * NumVertsY);
    VertexColors.SetNum(NumVertsX * NumVertsY);

    for (int y = 0; y < NumVertsY; y++)
    {
        for (int x = 0; x < NumVertsX; x++)
        {
            int Index = x + y * NumVertsX;
            FVector2D Pos2D(x * QuadSize, y * QuadSize);
            float Height = GetHeightAt(Pos2D);
            Vertices[Index] = FVector(Pos2D.X, Pos2D.Y, Height);

            UVs[Index] = FVector2D((float)x / NumQuadsX, (float)y / NumQuadsY);
            VertexColors[Index] = FColor::MakeRedToGreenColorFromScalar(Height / Amplitude);
        }
    }

    for (int y = 0; y < NumQuadsY; y++)
    {
        for (int x = 0; x < NumQuadsX; x++)
        {
            int i0 = x + y * NumVertsX;
            int i1 = (x + 1) + y * NumVertsX;
            int i2 = x + (y + 1) * NumVertsX;
            int i3 = (x + 1) + (y + 1) * NumVertsX;

            Triangles.Add(i0);
            Triangles.Add(i2);
            Triangles.Add(i1);

            Triangles.Add(i1);
            Triangles.Add(i2);
            Triangles.Add(i3);
        }
    }

    Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
    Mesh->ContainsPhysicsTriMeshData(true);

    if (TerrainMaterial)
    {
        Mesh->SetMaterial(0, TerrainMaterial);
    }
}


/*
#include "VoronoiTerrainActor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

AVoronoiTerrainActor::AVoronoiTerrainActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    RootComponent = ProcMesh;
    ProcMesh->bUseAsyncCooking = true;
}

void AVoronoiTerrainActor::OnConstruction(const FTransform& Transform)
{
    if (!ProcMesh) return;

    if (bRegenerateOnConstruction)
    {
        GenerateSites();
        BuildMesh();
    }

    if (Material)
    {
        ProcMesh->SetMaterial(0, Material);
    }
}

FORCEINLINE int32 AVoronoiTerrainActor::SiteIndex(int32 Cx, int32 Cy) const
{
    const int32 WX = ((Cx % CellsX) + CellsX) % CellsX;
    const int32 WY = ((Cy % CellsY) + CellsY) % CellsY;
    return WY * CellsX + WX;
}

void AVoronoiTerrainActor::GenerateSites()
{
    Sites.SetNumUninitialized(CellsX * CellsY);

    const float CellSizeX = WorldSizeX() / CellsX;
    const float CellSizeY = WorldSizeY() / CellsY;

    for (int32 cy = 0; cy < CellsY; ++cy)
    {
        for (int32 cx = 0; cx < CellsX; ++cx)
        {
            const int32 I = SiteIndex(cx, cy);
            FRandomStream R(Seed ^ (cx * 73856093) ^ (cy * 19349663));

            const float JitterX = R.FRand();
            const float JitterY = R.FRand();

            Sites[I] = FVector2D((cx + JitterX) * CellSizeX, (cy + JitterY) * CellSizeY);
        }
    }
}

float AVoronoiTerrainActor::WorleyF1(const FVector2D& P) const
{
    const float CellSizeX = WorldSizeX() / CellsX;
    const float CellSizeY = WorldSizeY() / CellsY;

    const int32 Cx = FMath::FloorToInt(P.X / CellSizeX);
    const int32 Cy = FMath::FloorToInt(P.Y / CellSizeY);

    float MinDistSq = TNumericLimits<float>::Max();

    for (int32 dy = -1; dy <= 1; ++dy)
    {
        for (int32 dx = -1; dx <= 1; ++dx)
        {
            const FVector2D Site = Sites[SiteIndex(Cx + dx, Cy + dy)];
            MinDistSq = FMath::Min(MinDistSq, FVector2D::DistSquared(P, Site));
        }
    }
    return FMath::Sqrt(MinDistSq);
}

float AVoronoiTerrainActor::WorleyF2(const FVector2D& P) const
{
    const float CellSizeX = WorldSizeX() / CellsX;
    const float CellSizeY = WorldSizeY() / CellsY;

    const int32 Cx = FMath::FloorToInt(P.X / CellSizeX);
    const int32 Cy = FMath::FloorToInt(P.Y / CellSizeY);

    float Best = TNumericLimits<float>::Max();
    float Second = TNumericLimits<float>::Max();

    for (int32 dy = -1; dy <= 1; ++dy)
    {
        for (int32 dx = -1; dx <= 1; ++dx)
        {
            const FVector2D Site = Sites[SiteIndex(Cx + dx, Cy + dy)];
            float d = FVector2D::DistSquared(P, Site);

            if (d < Best)
            {
                Second = Best;
                Best = d;
            }
            else if (d < Second)
            {
                Second = d;
            }
        }
    }
    return FMath::Sqrt(Second);
}

float AVoronoiTerrainActor::GetHeightAt(const FVector2D& P) const
{
    const float CellSizeX = WorldSizeX() / CellsX;
    const float CellSizeY = WorldSizeY() / CellsY;
    const float BaseRadius = 0.5f * FMath::Sqrt(CellSizeX * CellSizeX + CellSizeY * CellSizeY);

    auto Ridge = [&](float d, float radius) -> float
    {
        float t = 1.f - FMath::Clamp(d / radius, 0.f, 1.f);
        return FMath::Pow(t, RidgeSharpness);
    };

    float h = 0.f;

    switch (HeightMode)
    {
        case EHeightFunction::Ridge:
            h = Ridge(WorleyF1(P), BaseRadius);
            break;
        case EHeightFunction::Inverted:
            h = 1.f - Ridge(WorleyF1(P), BaseRadius);
            break;
        case EHeightFunction::Border:
            h = FMath::Clamp((WorleyF2(P) - WorleyF1(P)) / BaseRadius, 0.f, 1.f);
            break;
    }

    if (bSecondOctave)
    {
        FVector2D P2 = P + FVector2D(123.456f, 987.654f);
        float d2 = WorleyF1(P2) * SecondOctaveScale;
        float r2 = Ridge(d2, BaseRadius * SecondOctaveScale);
        h = FMath::Clamp(h + SecondOctaveWeight * r2, 0.f, 1.f);
    }

    return h * Amplitude;
}

void AVoronoiTerrainActor::BuildMesh()
{
    TArray<FVector> Vertices;
    TArray<int32>   Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    const int32 VertsX = NumQuadsX + 1;
    const int32 VertsY = NumQuadsY + 1;

    Vertices.SetNumUninitialized(VertsX * VertsY);
    UVs.SetNumUninitialized(VertsX * VertsY);
    Colors.SetNumUninitialized(VertsX * VertsY);

    for (int32 y = 0; y < VertsY; ++y)
    {
        for (int32 x = 0; x < VertsX; ++x)
        {
            const int32 i = y * VertsX + x;
            const float Px = x * QuadSize;
            const float Py = y * QuadSize;
            const float Height = GetHeightAt(FVector2D(Px, Py));

            Vertices[i] = FVector(Px, Py, Height);
            UVs[i] = FVector2D((float)x / NumQuadsX, (float)y / NumQuadsY);
            Colors[i] = FLinearColor(Height / Amplitude, 0.f, 0.f, 1.f);
        }
    }

    Triangles.Reserve(NumQuadsX * NumQuadsY * 6);
    for (int32 y = 0; y < NumQuadsY; ++y)
    {
        for (int32 x = 0; x < NumQuadsX; ++x)
        {
            const int32 i0 =  y      * VertsX + x;
            const int32 i1 =  y      * VertsX + (x+1);
            const int32 i2 = (y + 1) * VertsX + x;
            const int32 i3 = (y + 1) * VertsX + (x+1);

            Triangles.Add(i0); Triangles.Add(i3); Triangles.Add(i1);
            Triangles.Add(i0); Triangles.Add(i2); Triangles.Add(i3);
        }
    }

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);

    ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, bCreateCollision);
}
*/