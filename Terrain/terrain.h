// ProceduralMeshPlayground.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h" // make sure this header is available in your include paths
#include "ProceduralMeshPlayground.generated.h"

UCLASS(Blueprintable, BlueprintType)
class YOURMODULE_API AProceduralMeshPlayground : public AActor
{
    GENERATED_BODY()

public:
    AProceduralMeshPlayground();

    // If true, the mesh will be created in BeginPlay
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    bool bCreateOnBeginPlay = true;

    // Procedural Mesh Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Procedural Terrain")
    UProceduralMeshComponent* ProceduralMesh;

    // Grid size (number of vertices along X and Y)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain", meta = (ClampMin = "2"))
    int32 NumX = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain", meta = (ClampMin = "2"))
    int32 NumY = 128;

    // Spacing between vertices
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    float GridSpacing = 100.0f;

    // Height scale for noise output
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    float HeightMultiplier = 300.0f;

    // Noise parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    float NoiseScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    int32 NoiseSeed = 1337;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    int32 NoiseOctaves = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    float NoiseLacunarity = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Terrain")
    float NoiseGain = 0.5f;

    // Create terrain mesh (callable from Blueprints)
    UFUNCTION(BlueprintCallable, Category = "Procedural Terrain")
    void CreateMesh();

protected:
    virtual void BeginPlay() override;

private:
    // internal helper to compute normals
    void CalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals);

    // compute tangents (simple approach)
    void CalculateTangents(const TArray<FVector>& Vertices, const TArray<FVector2D>& UVs, const TArray<int32>& Triangles, TArray<FProcMeshTangent>& OutTangents);
};
