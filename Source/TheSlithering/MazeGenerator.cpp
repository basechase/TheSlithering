// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	WallMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh; 
	
	
}


void AMazeGenerator::RandomizeSeed()
{
	Seed = FMath::Rand();
	Generate();
}

void AMazeGenerator::Generate()
{
	FRandomStream Stream(Seed);
	
	CarveMaze(Stream);
	BuildWalls();
}

void AMazeGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Generate();
	
}


bool AMazeGenerator::InBounds(int32 X, int32 Y) const
{
	return X >=0 && Y >=0 && Y < Height && X < Width;
}

void AMazeGenerator::CarveMaze(FRandomStream& Stream)
{
	//reset the grid
	MazeCells.Init(FMazeCell(), Width * Height);
	
	//make the stack
	TArray<FIntPoint> Stack;
	MazeCells[Index(0,0)].bVisited = true;
	Stack.Push(FIntPoint(0,0));
	
	while (!Stack.IsEmpty())
	{
		FIntPoint Current = Stack.Last();
		
		//empty list for cell neighbors
		TArray<FIntPoint> Neighbors;
		
		//checks if neighbour is in bound and not visited
		
		/*
			 * NORTH = (X, Y + 1)
			 * 
			 *
			 * WEST = (X - 1, Y)
			 *
			 * EAST = (X + 1, Y)
			 * 
			 * SOUTH = (X, Y-1)
			 *
			 *
		 */
		
		
		
		
		//east check
		if (InBounds(Current.X + 1, Current.Y) && !MazeCells[Index(Current.X + 1, Current.Y)].bVisited)
		{
			Neighbors.Add(FIntPoint(Current.X + 1, Current.Y));
		}
		
		//west check
		if (InBounds(Current.X -1, Current.Y) && !MazeCells[Index(Current.X - 1, Current.Y)].bVisited)
		{
			Neighbors.Add(FIntPoint(Current.X - 1, Current.Y));
		}
		
		//north check
		if (InBounds(Current.X, Current.Y + 1) && !MazeCells[Index(Current.X, Current.Y + 1)].bVisited)
		{
			Neighbors.Add(FIntPoint(Current.X, Current.Y + 1));
		}
		
		//south check
		
		if (InBounds(Current.X, Current.Y - 1) && !MazeCells[Index(Current.X, Current.Y - 1)].bVisited)
		{
			Neighbors.Add(FIntPoint(Current.X, Current.Y - 1));
		}
		
		//check if dead end or no neighbor found. boxed in etc
		if (Neighbors.Num() == 0)
		{
			Stack.Pop();
			continue;
		}
		
		int32 Pick = Stream.RandRange(0, Neighbors.Num() - 1);
		FIntPoint Next = Neighbors[Pick];

		FIntPoint Dir = Next - Current;
		
		if (Dir.X ==1)
		{
			MazeCells[Index(Current.X, Current.Y)].bWallEast = false;
			MazeCells[Index(Next.X, Next.Y)].bWallWest = false;
		}
		
		if (Dir.X == -1)
		{
			MazeCells[Index(Current.X, Current.Y)].bWallWest = false;
			MazeCells[Index(Next.X, Next.Y)].bWallEast = false;
		}
		
		if (Dir.Y == 1)
		{
			MazeCells[Index(Current.X, Current.Y)].bWallNorth = false;
			MazeCells[Index(Next.X, Next.Y)].bWallSouth = false;
		}
		
		if (Dir.Y == -1)
		{
			MazeCells[Index(Current.X, Current.Y)].bWallSouth = false;
			MazeCells[Index(Next.X, Next.Y)].bWallNorth = false;
		}
		
		
		MazeCells[Index(Next.X, Next.Y)].bVisited = true;
		Stack.Push(Next);
	
	}
	
}

void AMazeGenerator::BuildWalls()
{
	//deletes old walls
	WallMesh->ClearInstances();
	
	//cube is 100m. scale = size/100
	//scaleX = wall running left-right along x. ScaleY = wall running up-down 
	FVector ScaleX(CellSize / 100.f, WallThickness / 100.f, WallHeight/100.f);
	FVector ScaleY(WallThickness / 100.f, CellSize / 100.f, WallHeight / 100.f);
	
	
	//visit every cell:  Y = Row X = Column
	for (int32 Y = 0; Y < Height; Y++)
	{
		for (int32 X = 0; X < Width; X++)
		{
			//southwall (every cell)
			if (MazeCells[Index(X,Y)].bWallSouth)
			{
				FVector Location((X + 0.5f) * CellSize, Y * CellSize, WallHeight/2.f);
				WallMesh->AddInstance(FTransform(FRotator::ZeroRotator, Location, ScaleX));
			}
			
			if (Y == Height -1 && MazeCells[Index(X,Y)].bWallNorth)
			{
				FVector Location((Y + 0.5f) * CellSize, (Y + 1) * CellSize, WallHeight/2.f);
				WallMesh->AddInstance(FTransform(FRotator::ZeroRotator, Location, ScaleX));
			}
			
			if (X == Width -1 && MazeCells[Index(X,Y)].bWallEast)
			{
				FVector Location((X - 1) * CellSize, (Y + 0.5f) * CellSize, WallHeight/2.f);
				WallMesh->AddInstance(FTransform(FRotator::ZeroRotator, Location, ScaleY));
			}
			
			if (MazeCells[Index(X,Y)].bWallWest)
			{
				FVector Location ( X * CellSize, (Y + 0.5f) * CellSize, WallHeight/2.f);
				WallMesh->AddInstance(FTransform(FRotator::ZeroRotator, Location, ScaleY));
			}
			
			
			
		}
	}
	
}

