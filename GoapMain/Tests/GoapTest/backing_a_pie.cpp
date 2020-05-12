#include "backing_a_pie.h"
#include "newptr.h"

float backing_a_pie::REF_TEMP = 300;

backing_a_pie::backing_a_pie()
{
    _backingHelper = NewPtr<IState>();
    _backingHelper->assign({{"EggIsOnBowl", 1.f}, {"ButterIsOnBowl", 1.f}, {"FlourIsOnBowl", 1.f}, {"IngredientsAreMixed", 1.f}, {"PieIsBaked", 1.f}});
    _orderHelper = NewPtr<IState>();
    _orderHelper->assign({{"PieIsComing", 5}});

    backing_actions();
}
