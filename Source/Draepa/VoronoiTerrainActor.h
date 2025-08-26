#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VoronoiTerrainActor.generated.h"

UCLASS()
class DRAEPA_API AVoronoiTerrainActor : public AActor
{
    GENERATED_BODY()

public:
    AVoronoiTerrainActor();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // Mesh
    UPROPERTY(VisibleAnywhere, Category="Terrain")
    UProceduralMeshComponent* Mesh;

    // Settings
    UPROPERTY(EditAnywhere, Category="Terrain|Grid")
    int32 NumQuadsX = 256;

    UPROPERTY(EditAnywhere, Category="Terrain|Grid")
    int32 NumQuadsY = 256;

    UPROPERTY(EditAnywhere, Category="Terrain|Grid")
    float QuadSize = 3200.f;

    UPROPERTY(EditAnywhere, Category="Terrain|Height")
    float Amplitude = 800.f;

    // Voronoi
    UPROPERTY(EditAnywhere, Category="Terrain|Voronoi")
    int32 CellsX = 20;

    UPROPERTY(EditAnywhere, Category="Terrain|Voronoi")
    int32 CellsY = 20;

    UPROPERTY(EditAnywhere, Category="Terrain|Voronoi")
    int32 Seed = 1337;

    UPROPERTY(EditAnywhere, Category="Terrain|Voronoi")
    float RidgeSharpness = 1.2f;

    // Noise Weights
    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float VoronoiWeight = 0.35f;

    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float PerlinWeight = 0.25f;

    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float FbmWeight = 0.4f;

    // Noise Settings
    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float PerlinScale = 0.002f;

    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float FbmScale = 0.01f;

    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    int32 FbmOctaves = 5;

    UPROPERTY(EditAnywhere, Category="Terrain|Noise")
    float FbmPersistence = 0.45f;

    // Material
    UPROPERTY(EditAnywhere, Category="Terrain|Material")
    UMaterialInterface* TerrainMaterial;

    // Functions
    void GenerateMesh();
    float GetHeightAt(const FVector2D& Pos) const;
    float PerlinNoise2D(const FVector2D& P) const;
    float FBmNoise2D(const FVector2D& P, int Octaves, float Persistence) const;
    float WorleyF1(const FVector2D& P) const;
    float ComputeBaseRadius() const;
};



/*#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoronoiTerrainActor.generated.h"

class UProceduralMeshComponent;

UENUM(BlueprintType)
enum class EHeightFunction : uint8
{
    Ridge       UMETA(DisplayName="Ridge (mountains)"),
    Inverted    UMETA(DisplayName="Inverted (basins)"),
    Border      UMETA(DisplayName="Cell Borders")
};

UCLASS(Blueprintable)
class DRAEPA_API AVoronoiTerrainActor : public AActor
{
    GENERATED_BODY()

public:
    AVoronoiTerrainActor();

    // ----- Mesh resolution -----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
    int32 NumQuadsX = 256;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
    int32 NumQuadsY = 256;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
    float QuadSize = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
    bool bCreateCollision = true;

    // ----- Voronoi controls -----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    int32 CellsX = 32;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    int32 CellsY = 32;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    int32 Seed = 1337;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    float Amplitude = 3000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    float RidgeSharpness = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    bool bSecondOctave = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi", meta=(EditCondition="bSecondOctave"))
    float SecondOctaveScale = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi", meta=(EditCondition="bSecondOctave"))
    float SecondOctaveWeight = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voronoi")
    EHeightFunction HeightMode = EHeightFunction::Ridge;

    // Regenerate on move in editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generation")
    bool bRegenerateOnConstruction = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rendering")
    UMaterialInterface* Material = nullptr;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UProceduralMeshComponent> ProcMesh;

    TArray<FVector2D> Sites;

    float WorldSizeX() const { return NumQuadsX * QuadSize; }
    float WorldSizeY() const { return NumQuadsY * QuadSize; }

    void GenerateSites();
    FORCEINLINE int32 SiteIndex(int32 Cx, int32 Cy) const;
    float WorleyF1(const FVector2D& P) const;
    float WorleyF2(const FVector2D& P) const;

    float GetHeightAt(const FVector2D& P) const;
    void BuildMesh();
};
*/