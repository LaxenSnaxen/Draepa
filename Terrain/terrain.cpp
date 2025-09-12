// ProceduralMeshPlayground.cpp
#include "ProceduralMeshPlayground.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h" // for debug
#include "FastNoiseLite.h"

// Sets default values
AProceduralMeshPlayground::AProceduralMeshPlayground()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Procedural Mesh Component and make it the Root
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    SetRootComponent(ProceduralMesh);

    // Enable collision and NavMesh if desired
    ProceduralMesh->bUseComplexAsSimpleCollision = false;
    ProceduralMesh->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void AProceduralMeshPlayground::BeginPlay()
{
    Super::BeginPlay();

    if (bCreateOnBeginPlay)
    {
        CreateMesh();
    }
}

void AProceduralMeshPlayground::CreateMesh()
{
    if (!ProceduralMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("ProceduralMesh is null"));
        return;
    }

    // clamp sizes to sane ranges
    NumX = FMath::Clamp(NumX, 2, 4096);
    NumY = FMath::Clamp(NumY, 2, 4096);
    GridSpacing = FMath::Max(1.0f, GridSpacing);

    // Initialize FastNoiseLite
    FastNoiseLite Noise;
    Noise.SetSeed(NoiseSeed);
    Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); // you can change to OpenSimplex2, Value, etc.
    Noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    Noise.SetFractalOctaves(NoiseOctaves);
    Noise.SetFractalLacunarity(NoiseLacunarity);
    Noise.SetFractalGain(NoiseGain);

    // Pre-allocate arrays
    const int32 VertexCount = NumX * NumY;
    TArray<FVector> Vertices;
    Vertices.Reserve(VertexCount);

    TArray<FVector2D> UVs;
    UVs.Reserve(VertexCount);

    // Create height values using noise
    // Use a normalized coordinate so noise tiling/scale behaves predictably
    const float invNumX = 1.0f / (NumX - 1);
    const float invNumY = 1.0f / (NumY - 1);

    for (int32 y = 0; y < NumY; ++y)
    {
        for (int32 x = 0; x < NumX; ++x)
        {
            // Normalized coordinates centered at 0
            float nx = (x * invNumX - 0.5f) * NoiseScale;
            float ny = (y * invNumY - 0.5f) * NoiseScale;

            // FastNoiseLite returns -1..+1 for GetNoise; rescale to 0..1 then to height
            float n = Noise.GetNoise(nx, ny); // -1..1
            n = n * 0.5f + 0.5f; // 0..1

            float height = n * HeightMultiplier;

            FVector Vertex = FVector(x * GridSpacing, y * GridSpacing, height);
            Vertices.Add(Vertex);

            // UV across the grid 0..1
            UVs.Add(FVector2D(x * invNumX, y * invNumY));
        }
    }

    // Triangles (two triangles per quad)
    TArray<int32> Triangles;
    Triangles.Reserve((NumX - 1) * (NumY - 1) * 6);

    for (int32 y = 0; y < NumY - 1; ++y)
    {
        for (int32 x = 0; x < NumX - 1; ++x)
        {
            int32 i00 = y * NumX + x;
            int32 i10 = y * NumX + (x + 1);
            int32 i01 = (y + 1) * NumX + x;
            int32 i11 = (y + 1) * NumX + (x + 1);

            // First triangle i00, i11, i10
            Triangles.Add(i00);
            Triangles.Add(i11);
            Triangles.Add(i10);

            // Second triangle i00, i01, i11
            Triangles.Add(i00);
            Triangles.Add(i01);
            Triangles.Add(i11);
        }
    }

    // Normals
    TArray<FVector> Normals;
    Normals.SetNumZeroed(Vertices.Num());
    CalculateNormals(Vertices, Triangles, Normals);

    // Tangents
    TArray<FProcMeshTangent> Tangents;
    Tangents.SetNumZeroed(Vertices.Num());
    CalculateTangents(Vertices, UVs, Triangles, Tangents);

    // Colors (optional)
    TArray<FLinearColor> VertexColors;
    VertexColors.SetNum(Vertices.Num());
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        // color by height - optional
        float h = Vertices[i].Z / HeightMultiplier; // 0..1
        VertexColors[i] = FLinearColor(h, h, h, 1.0f);
    }

    // Create the mesh section (index 0)
    ProceduralMesh->ClearAllMeshSections();
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

    // Enable collision
    ProceduralMesh->ContainsPhysicsTriMeshData(true); // ensure tri mesh data created for collision

    // Optionally create simple collision from mesh
    ProceduralMesh->UpdateCollision();

    UE_LOG(LogTemp, Log, TEXT("Created terrain mesh: %d verts, %d tris"), Vertices.Num(), Triangles.Num() / 3);
}

void AProceduralMeshPlayground::CalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals)
{
    // Zero normals
    for (int32 i = 0; i < OutNormals.Num(); ++i)
    {
        OutNormals[i] = FVector::ZeroVector;
    }

    // Accumulate face normals
    const int32 TriCount = Triangles.Num() / 3;
    for (int32 t = 0; t < TriCount; ++t)
    {
        int32 i0 = Triangles[t * 3 + 0];
        int32 i1 = Triangles[t * 3 + 1];
        int32 i2 = Triangles[t * 3 + 2];

        const FVector& v0 = Vertices[i0];
        const FVector& v1 = Vertices[i1];
        const FVector& v2 = Vertices[i2];

        FVector Edge1 = v1 - v0;
        FVector Edge2 = v2 - v0;
        FVector FaceNormal = FVector::CrossProduct(Edge2, Edge1); // note winding: cross(Edge2, Edge1) gives correct up for our triangle ordering
        FaceNormal.Normalize();

        OutNormals[i0] += FaceNormal;
        OutNormals[i1] += FaceNormal;
        OutNormals[i2] += FaceNormal;
    }

    // Normalize
    for (int32 i = 0; i < OutNormals.Num(); ++i)
    {
        OutNormals[i].Normalize();
    }
}

void AProceduralMeshPlayground::CalculateTangents(const TArray<FVector>& Vertices, const TArray<FVector2D>& UVs, const TArray<int32>& Triangles, TArray<FProcMeshTangent>& OutTangents)
{
    // Basic tangent calc: accumulate per triangle then orthonormalize
    TArray<FVector> TangentAcc;
    TangentAcc.SetNumZeroed(Vertices.Num());

    const int32 TriCount = Triangles.Num() / 3;
    for (int32 t = 0; t < TriCount; ++t)
    {
        int32 i0 = Triangles[t * 3 + 0];
        int32 i1 = Triangles[t * 3 + 1];
        int32 i2 = Triangles[t * 3 + 2];

        const FVector& v0 = Vertices[i0];
        const FVector& v1 = Vertices[i1];
        const FVector& v2 = Vertices[i2];

        const FVector2D& uv0 = UVs[i0];
        const FVector2D& uv1 = UVs[i1];
        const FVector2D& uv2 = UVs[i2];

        FVector Edge1 = v1 - v0;
        FVector Edge2 = v2 - v0;
        FVector2D dUV1 = uv1 - uv0;
        FVector2D dUV2 = uv2 - uv0;

        float r = (dUV1.X * dUV2.Y - dUV1.Y * dUV2.X);
        if (FMath::Abs(r) < KINDA_SMALL_NUMBER)
        {
            // fallback
            continue;
        }
        r = 1.0f / r;

        FVector Tangent = (Edge1 * dUV2.Y - Edge2 * dUV1.Y) * r;
        TangentAcc[i0] += Tangent;
        TangentAcc[i1] += Tangent;
        TangentAcc[i2] += Tangent;
    }

    for (int32 i = 0; i < OutTangents.Num(); ++i)
    {
        FVector T = TangentAcc[i];
        T.Normalize();
        OutTangents[i] = FProcMeshTangent(T, false);
    }
}
