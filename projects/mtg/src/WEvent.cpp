#include "PrecompiledHeader.h"

#include "WEvent.h"
#include "MTGCardInstance.h"
#include "MTGGameZones.h"
#include "Damage.h"
#include "PhaseRing.h"
#include "AllAbilities.h"

WEvent::WEvent(int type) :
    type(type)
{
}

WEventZoneChange::WEventZoneChange(MTGCardInstance * card, MTGGameZone * from, MTGGameZone *to, bool forcetrigger) :
    WEvent(CHANGE_ZONE), card(card), from(from), to(to), forcetrigger(forcetrigger) //Added a force trigger for modal double faced card (eg. Zendikar Rising)
{
}

WEventDamage::WEventDamage(Damage *damage) :
    WEvent(DAMAGE), damage(damage)
{
}

WEventLife::WEventLife(Player * player, int amount, MTGCardInstance* source) :
    WEvent(), player(player), amount(amount), source(source)
{
}

WEventDamageStackResolved::WEventDamageStackResolved() :
    WEvent()
{
}

WEventGameStateBasedChecked::WEventGameStateBasedChecked() :
    WEvent()
{
}

WEventCardUpdate::WEventCardUpdate(MTGCardInstance * card) :
    WEvent(), card(card)
{
}
;

WEventCounters::WEventCounters(Counters *counter,string name,int power,int toughness,bool added,bool removed, MTGCardInstance* source) :
WEvent(),counter(counter),name(name),power(power),toughness(toughness),added(added),removed(removed),source(source)
{
}

WEventPhaseChange::WEventPhaseChange(Phase * from, Phase * to) :
    WEvent(CHANGE_PHASE), from(from), to(to)
{
}

WEventPhasePreChange::WEventPhasePreChange(Phase * from, Phase * to) :
WEvent(CHANGE_PHASE), from(from), to(to)
{
    eventChanged = false;
}

WEventCardTap::WEventCardTap(MTGCardInstance * card, bool before, bool after) :
    WEventCardUpdate(card), before(before), after(after)
{
    noTrigger = false;
}

WEventCardTappedForMana::WEventCardTappedForMana(MTGCardInstance * card, bool before, bool after) :
    WEventCardUpdate(card), before(before), after(after)
{
}

WEventCardAttacked::WEventCardAttacked(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardAttackedAlone::WEventCardAttackedAlone(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardAttackedNotBlocked::WEventCardAttackedNotBlocked(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardAttackedBlocked::WEventCardAttackedBlocked(MTGCardInstance * card,MTGCardInstance * opponent) :
    WEventCardUpdate(card),opponent(opponent)
{
}

WEventCardBlocked::WEventCardBlocked(MTGCardInstance * card,MTGCardInstance * opponent) :
    WEventCardUpdate(card),opponent(opponent)
{
}

WEventcardDraw::WEventcardDraw(Player * player, int nb_cards) :
    player(player), nb_cards(nb_cards)
{
}

    WEventDraw::WEventDraw(Player * player, int nb_cards,MTGAbility * cardDraw) ://happens at the call for drawfromlibrary
player(player), nb_cards(nb_cards),drawAbility(cardDraw)
{
}

WEventCardSacrifice::WEventCardSacrifice(MTGCardInstance * card, MTGCardInstance * after) :
    WEventCardUpdate(card),cardAfter(after)
{
}

WEventCardDiscard::WEventCardDiscard(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
    card->discarded = true;
}

WEventCardCycle::WEventCardCycle(MTGCardInstance * card) :
WEventCardUpdate(card)
{
}

WEventCardExerted::WEventCardExerted(MTGCardInstance * card) :
WEventCardUpdate(card)
{
}

WEventVampire::WEventVampire(MTGCardInstance * card,MTGCardInstance * source,MTGCardInstance * victem) :
    WEventCardUpdate(card),source(source),victem(victem)
{
}

WEventTarget::WEventTarget(MTGCardInstance * card,MTGCardInstance * source) :
    WEventCardUpdate(card),card(card),source(source)
{
}

WEventCardChangeType::WEventCardChangeType(MTGCardInstance * card, int type, bool before, bool after) :
    WEventCardUpdate(card), type(type), before(before), after(after)
{
}

WEventCreatureAttacker::WEventCreatureAttacker(MTGCardInstance * card, Targetable * before, Targetable * after) :
    WEventCardUpdate(card), before(before), after(after)
{
}

WEventCreatureBlocker::WEventCreatureBlocker(MTGCardInstance * card, MTGCardInstance * from, MTGCardInstance * to) :
    WEventCardUpdate(card), before(from), after(to)
{
}

WEventCreatureBlockerRank::WEventCreatureBlockerRank(MTGCardInstance * card, MTGCardInstance * exchangeWith,
                MTGCardInstance * attacker) :
    WEventCardUpdate(card), exchangeWith(exchangeWith), attacker(attacker)
{
}

WEventEngageManaExtra::WEventEngageManaExtra(int color, MTGCardInstance* card, ManaPool * destination) :
    WEvent(), color(color), card(card), destination(destination)
{//controller snow
    if(color == 1 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaG += 1;
    if(color == 2 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaU += 1;
    if(color == 3 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaR += 1;
    if(color == 4 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaB += 1;
    if(color == 5 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaW += 1;
    if((color == 0 || color == 6) && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaC += 1;
    //opponent snow
    if(color == 1 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaG += 1;
    if(color == 2 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaU += 1;
    if(color == 3 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaR += 1;
    if(color == 4 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaB += 1;
    if(color == 5 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaW += 1;
    if((color == 0 || color == 6) && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaC += 1;
}
WEventEngageMana::WEventEngageMana(int color, MTGCardInstance* card, ManaPool * destination) :
    WEvent(), color(color), card(card), destination(destination)
{//controller snow
    if(color == 1 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaG += 1;
    if(color == 2 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaU += 1;
    if(color == 3 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaR += 1;
    if(color == 4 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaB += 1;
    if(color == 5 && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaW += 1;
    if((color == 0 || color == 6) && card->controller()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->snowManaC += 1;
    //opponent snow
    if(color == 1 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaG += 1;
    if(color == 2 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaU += 1;
    if(color == 3 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaR += 1;
    if(color == 4 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaB += 1;
    if(color == 5 && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaW += 1;
    if((color == 0 || color == 6) && card->controller()->opponent()->getManaPool() == destination && card->hasType("snow"))
        card->controller()->opponent()->snowManaC += 1;
}
WEventConsumeMana::WEventConsumeMana(int color, ManaPool * source) :
    WEvent(), color(color), source(source)
{
}
WEventEmptyManaPool::WEventEmptyManaPool(ManaPool * source) :
    WEvent(), source(source)
{
}

WEventCardUnattached::WEventCardUnattached(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardEquipped::WEventCardEquipped(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardControllerChange::WEventCardControllerChange(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardPhasesOut::WEventCardPhasesOut(MTGCardInstance * card, int turn) :
    WEventCardUpdate(card)
{
    if(card->getPhasedOutAbility().size())
    {
        AbilityFactory af(card->getObserver());
        MTGAbility * a = af.parseMagicLine(card->getPhasedOutAbility(), card->getObserver()->mLayers->actionLayer()->getMaxId(), NULL, card->clone());
        MTGAbility * poA = a->clone();
        SAFE_DELETE(a);
        poA->oneShot = true;
        poA->canBeInterrupted = false;
        MTGAbility *gatg = NEW GenericAddToGame(card->getObserver(), card->getObserver()->mLayers->actionLayer()->getMaxId(), card,NULL,poA->clone());
        SAFE_DELETE(poA);
        gatg->fireAbility();
        //SAFE_DELETE(gatg);
    }
}

WEventCardPhasesIn::WEventCardPhasesIn(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardFaceUp::WEventCardFaceUp(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardTransforms::WEventCardTransforms(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCardCopiedACard::WEventCardCopiedACard(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventCombatStepChange::WEventCombatStepChange(CombatStep step) :
    WEvent(), step(step)
{
}

WEventplayerEnergized::WEventplayerEnergized(Player * player, int nb_count) :
    player(player), nb_count(nb_count)
{
}

WEventCardMutated::WEventCardMutated(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}

WEventTokenCreated::WEventTokenCreated(MTGCardInstance * card) :
    WEventCardUpdate(card)
{
}
;

Targetable * WEventDamage::getTarget(int target)
{
    switch (target)
    {
    case TARGET_TO:
        return damage->target;
    case TARGET_FROM:
        return damage->source;
    }
    return NULL;
}

int WEventDamage::getValue()
{
    return damage->damage;
}

Targetable * WEventLife::getTarget(int target)
{
    if (target)
    {
        return player;
    }
    return NULL;
}

Targetable * WEventCounters::getTarget()
{
    return targetCard;
}

Targetable * WEventVampire::getTarget(int target)
{
    switch (target)
    {
    case TARGET_TO:
        return victem->next;
    case TARGET_FROM:
        return source;
    }
    return NULL;
}

Targetable * WEventTarget::getTarget(int target)
{
    switch (target)
    {
    case TARGET_TO:
        return card;
    case TARGET_FROM:
        return source;
    }
    return NULL;
}

Targetable * WEventZoneChange::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardAttacked::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardAttackedAlone::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardSacrifice::getTarget(int target)
{
    if (target)
    {
            return cardAfter;
    }
    return NULL;
}

Targetable * WEventCardDiscard::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardCycle::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardExerted::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardAttackedNotBlocked::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardAttackedBlocked::getTarget(int target)
{
    switch (target)
    {
    case TARGET_TO:
        return card;
    case TARGET_FROM:
        return opponent;
    }
    return NULL;
}

Targetable * WEventCardBlocked::getTarget(int target)
{
    switch (target)
    {
    case TARGET_TO:
        return card;
    case TARGET_FROM:
        return opponent;
    }
    return NULL;
}

Targetable * WEventCardTap::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardTappedForMana::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventcardDraw::getTarget(Player * player)
{
    if (player) return player;
    return NULL;
}

Targetable * WEventCardUnattached::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardEquipped::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardControllerChange::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardPhasesOut::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardPhasesIn::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardFaceUp::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardTransforms::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardCopiedACard::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventCardMutated::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventTokenCreated::getTarget(int target)
{
    if (target) return card;
    return NULL;
}

Targetable * WEventplayerEnergized::getTarget(Player * player)
{
    if (player) return player;
    return NULL;
}

std::ostream& WEvent::toString(std::ostream& out) const
{
    return out << "EVENT";
}
std::ostream& WEventZoneChange::toString(std::ostream& out) const
{
    return out << "ZONEEVENT " << *card << " : " << *from << " -> " << *to;
}
std::ostream& WEventDamage::toString(std::ostream& out) const
{
    if (MTGCardInstance* m = dynamic_cast<MTGCardInstance*>(damage->target))
        return out << "DAMAGEEVENT " << damage->damage << " >> " << *m;
    else
        return out << "DAMAGEEVENT " << damage->damage << " >> " << damage->target;
}
std::ostream& operator<<(std::ostream& out, const WEvent& m)
{
    return m.toString(out);
}
