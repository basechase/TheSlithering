// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class THESLITHERING_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	int32 Width = 10;
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	int32 Height = 10;	
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	float CellSize = 400.f;
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	float WallThickness = 20.f;
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	float WallHeight = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Maze")
	int32 Seed = 12345;
	
	UFUNCTION(CallInEditor, Category = "Maze")
	void RandomizeSeed();
	
	UPROPERTY(VisibleAnywhere, Category = "Maze")
	TObjectPtr<UInstancedStaticMeshComponent> WallMesh;
	
	UFUNCTION(CallInEditor, Category = "Maze")
	void Generate();
	

protected:
	// Called when the game starts or when spawned
	

	virtual void OnConstruction(const FTransform& Transform) override;
private:
	struct FMazeCell
	{
		bool bVisited = false;
		bool bWallNorth = true;
		bool bWallSouth = true;
		bool bWallEast = true;
		bool bWallWest = true;
	};
	
	TArray<FMazeCell> MazeCells;
	
	int32 Index(int32 X, int32 Y) const { return Y * Width + X; }
	bool InBounds(int32 X, int32 Y) const;
	
	void CarveMaze(FRandomStream& Stream);
	void BuildWalls();
	
	
public:	
	// Called every frame


};
