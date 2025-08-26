// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Draepa/VoronoiTerrainActor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeVoronoiTerrainActor() {}

// ********** Begin Cross Module References ********************************************************
DRAEPA_API UClass* Z_Construct_UClass_AVoronoiTerrainActor();
DRAEPA_API UClass* Z_Construct_UClass_AVoronoiTerrainActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_Draepa();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVoronoiTerrainActor *****************************************************
void AVoronoiTerrainActor::StaticRegisterNativesAVoronoiTerrainActor()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AVoronoiTerrainActor;
UClass* AVoronoiTerrainActor::GetPrivateStaticClass()
{
	using TClass = AVoronoiTerrainActor;
	if (!Z_Registration_Info_UClass_AVoronoiTerrainActor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("VoronoiTerrainActor"),
			Z_Registration_Info_UClass_AVoronoiTerrainActor.InnerSingleton,
			StaticRegisterNativesAVoronoiTerrainActor,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AVoronoiTerrainActor.InnerSingleton;
}
UClass* Z_Construct_UClass_AVoronoiTerrainActor_NoRegister()
{
	return AVoronoiTerrainActor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVoronoiTerrainActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VoronoiTerrainActor.h" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mesh_MetaData[] = {
		{ "Category", "Terrain" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Mesh\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Mesh" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NumQuadsX_MetaData[] = {
		{ "Category", "Terrain|Grid" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Settings\n" },
#endif
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Settings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NumQuadsY_MetaData[] = {
		{ "Category", "Terrain|Grid" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_QuadSize_MetaData[] = {
		{ "Category", "Terrain|Grid" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Amplitude_MetaData[] = {
		{ "Category", "Terrain|Height" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CellsX_MetaData[] = {
		{ "Category", "Terrain|Voronoi" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Voronoi\n" },
#endif
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Voronoi" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CellsY_MetaData[] = {
		{ "Category", "Terrain|Voronoi" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Seed_MetaData[] = {
		{ "Category", "Terrain|Voronoi" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RidgeSharpness_MetaData[] = {
		{ "Category", "Terrain|Voronoi" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VoronoiWeight_MetaData[] = {
		{ "Category", "Terrain|Noise" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Noise Weights\n" },
#endif
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Noise Weights" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PerlinWeight_MetaData[] = {
		{ "Category", "Terrain|Noise" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FbmWeight_MetaData[] = {
		{ "Category", "Terrain|Noise" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PerlinScale_MetaData[] = {
		{ "Category", "Terrain|Noise" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Noise Settings\n" },
#endif
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Noise Settings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FbmScale_MetaData[] = {
		{ "Category", "Terrain|Noise" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FbmOctaves_MetaData[] = {
		{ "Category", "Terrain|Noise" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FbmPersistence_MetaData[] = {
		{ "Category", "Terrain|Noise" },
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TerrainMaterial_MetaData[] = {
		{ "Category", "Terrain|Material" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material\n" },
#endif
		{ "ModuleRelativePath", "VoronoiTerrainActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Mesh;
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumQuadsX;
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumQuadsY;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_QuadSize;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Amplitude;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CellsX;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CellsY;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Seed;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RidgeSharpness;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_VoronoiWeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PerlinWeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FbmWeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PerlinScale;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FbmScale;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FbmOctaves;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FbmPersistence;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TerrainMaterial;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVoronoiTerrainActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Mesh = { "Mesh", nullptr, (EPropertyFlags)0x00400000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, Mesh), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mesh_MetaData), NewProp_Mesh_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_NumQuadsX = { "NumQuadsX", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, NumQuadsX), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NumQuadsX_MetaData), NewProp_NumQuadsX_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_NumQuadsY = { "NumQuadsY", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, NumQuadsY), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NumQuadsY_MetaData), NewProp_NumQuadsY_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_QuadSize = { "QuadSize", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, QuadSize), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_QuadSize_MetaData), NewProp_QuadSize_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Amplitude = { "Amplitude", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, Amplitude), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Amplitude_MetaData), NewProp_Amplitude_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_CellsX = { "CellsX", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, CellsX), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CellsX_MetaData), NewProp_CellsX_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_CellsY = { "CellsY", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, CellsY), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CellsY_MetaData), NewProp_CellsY_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Seed = { "Seed", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, Seed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Seed_MetaData), NewProp_Seed_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_RidgeSharpness = { "RidgeSharpness", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, RidgeSharpness), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RidgeSharpness_MetaData), NewProp_RidgeSharpness_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_VoronoiWeight = { "VoronoiWeight", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, VoronoiWeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VoronoiWeight_MetaData), NewProp_VoronoiWeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_PerlinWeight = { "PerlinWeight", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, PerlinWeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PerlinWeight_MetaData), NewProp_PerlinWeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmWeight = { "FbmWeight", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, FbmWeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FbmWeight_MetaData), NewProp_FbmWeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_PerlinScale = { "PerlinScale", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, PerlinScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PerlinScale_MetaData), NewProp_PerlinScale_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmScale = { "FbmScale", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, FbmScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FbmScale_MetaData), NewProp_FbmScale_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmOctaves = { "FbmOctaves", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, FbmOctaves), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FbmOctaves_MetaData), NewProp_FbmOctaves_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmPersistence = { "FbmPersistence", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, FbmPersistence), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FbmPersistence_MetaData), NewProp_FbmPersistence_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_TerrainMaterial = { "TerrainMaterial", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVoronoiTerrainActor, TerrainMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TerrainMaterial_MetaData), NewProp_TerrainMaterial_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AVoronoiTerrainActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Mesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_NumQuadsX,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_NumQuadsY,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_QuadSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Amplitude,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_CellsX,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_CellsY,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_Seed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_RidgeSharpness,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_VoronoiWeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_PerlinWeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmWeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_PerlinScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmOctaves,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_FbmPersistence,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVoronoiTerrainActor_Statics::NewProp_TerrainMaterial,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVoronoiTerrainActor_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AVoronoiTerrainActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_Draepa,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVoronoiTerrainActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVoronoiTerrainActor_Statics::ClassParams = {
	&AVoronoiTerrainActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AVoronoiTerrainActor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AVoronoiTerrainActor_Statics::PropPointers),
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVoronoiTerrainActor_Statics::Class_MetaDataParams), Z_Construct_UClass_AVoronoiTerrainActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AVoronoiTerrainActor()
{
	if (!Z_Registration_Info_UClass_AVoronoiTerrainActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVoronoiTerrainActor.OuterSingleton, Z_Construct_UClass_AVoronoiTerrainActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVoronoiTerrainActor.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AVoronoiTerrainActor);
AVoronoiTerrainActor::~AVoronoiTerrainActor() {}
// ********** End Class AVoronoiTerrainActor *******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_maxim_Documents_Unreal_Projects_Draepa_Source_Draepa_VoronoiTerrainActor_h__Script_Draepa_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVoronoiTerrainActor, AVoronoiTerrainActor::StaticClass, TEXT("AVoronoiTerrainActor"), &Z_Registration_Info_UClass_AVoronoiTerrainActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVoronoiTerrainActor), 1820750264U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_maxim_Documents_Unreal_Projects_Draepa_Source_Draepa_VoronoiTerrainActor_h__Script_Draepa_1701984616(TEXT("/Script/Draepa"),
	Z_CompiledInDeferFile_FID_Users_maxim_Documents_Unreal_Projects_Draepa_Source_Draepa_VoronoiTerrainActor_h__Script_Draepa_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_maxim_Documents_Unreal_Projects_Draepa_Source_Draepa_VoronoiTerrainActor_h__Script_Draepa_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
