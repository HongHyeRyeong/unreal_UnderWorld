#include "ItemBase.h"

AItemBase::AItemBase()
{
    if (!CollisionBox)
    {
        CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
        RootComponent = CollisionBox;
    }
}