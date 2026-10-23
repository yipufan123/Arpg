// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex,int32,WidgetSwitcherIndex);


/**
 * 
 */
UCLASS()
class ARPG_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;



	
	void InitializeSlot();


	// Begin Setter
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);
	void SetSlotIndex(int32 InSlotIndex);
	// End Setter

	// Begin Getter
	FString GetLoadSlotName() const{return LoadSlotName;}
	FString GetPlayerName() const{return PlayerName;}
	int32 GetSlotIndex() const{return SlotIndex;}
	// End Getter
		

private:
	/**
	 * Field Notifies
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true))
	FString PlayerName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true))
	int32 SlotIndex;
};
