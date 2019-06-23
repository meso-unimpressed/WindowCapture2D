// Copyright 2019 ayumax. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include <WinUser.h>
#else
using HBITMAP = void*;
using HDC = void*;
using HWND = void*;
#endif

#include "CaptureMachineProperties.generated.h"

USTRUCT(BlueprintType)
struct FIntVector2D
{	
	GENERATED_BODY()

	int32 X;
	int32 Y;
	
	FIntVector2D()
		: X(0), Y(0)
	{

	}
	FIntVector2D(int32 _X, int32 _Y)
		: X(_X), Y(_Y)
	{
	}

	bool operator != (const FIntVector2D& obj)
	{
		return X != obj.X || Y != obj.Y;
	}
};

UENUM(BlueprintType)
enum class ETitleMatchingWindowSearch : uint8
{
	PerfectMatch,
	ForwardMatch,
	PartialMatch,
	BackwardMatch,
	RegularExpression
};



USTRUCT(BlueprintType)
struct WINDOWCAPTURE2D_API FCaptureMachineProperties 
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	FString CaptureTargetTitle = TEXT("WindowCapture2D");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	ETitleMatchingWindowSearch TitleMatchingWindowSearch = ETitleMatchingWindowSearch::ForwardMatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	int32 FrameRate = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	bool CheckWindowSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	bool CutShadow = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WindowCapture2D)
	bool TransferMouse = false;

};


UENUM(BlueprintType)
enum class EMouseInputTransferType : uint8
{
	MouseDown,
	MouseMove,
	MouseUp,
	MouseClick
};