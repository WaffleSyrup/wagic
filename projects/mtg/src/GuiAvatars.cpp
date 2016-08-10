#include "PrecompiledHeader.h"

#include "CardSelector.h"
#include "GameApp.h"
#include "GuiAvatars.h"
#include "GameObserver.h"

#define LIB_GRAVE_OFFSET 230

GuiAvatars::GuiAvatars(DuelLayers* duelLayers) :
    GuiLayer(duelLayers), active(NULL)
{
    Add(self = NEW GuiAvatar(SCREEN_WIDTH, SCREEN_HEIGHT, false, mpDuelLayers->getRenderedPlayer(), GuiAvatar::BOTTOM_RIGHT, this));
    self->zoom = 0.9f;
    Add(selfGraveyard = NEW GuiGraveyard(SCREEN_WIDTH - GuiAvatar::Width - GuiGameZone::Width / 2 - 11, SCREEN_HEIGHT - GuiAvatar::Height - 1, false, mpDuelLayers->getRenderedPlayer(), this));
    Add(selfLibrary = NEW GuiLibrary(SCREEN_WIDTH - GuiAvatar::Width - GuiGameZone::Width / 2 - 11, SCREEN_HEIGHT - GuiAvatar::Height - 5 + GuiGameZone::Height + 5, false, mpDuelLayers->getRenderedPlayer(), this));
    //myexile
    Add(selfExile = NEW GuiExile(SCREEN_WIDTH - GuiAvatar::Width - GuiGameZone::Width / 2 - 11, SCREEN_HEIGHT - GuiAvatar::Height - 30, false, mpDuelLayers->getRenderedPlayer(), this));

    Add(opponent = NEW GuiAvatar(0, 0, false, mpDuelLayers->getRenderedPlayerOpponent(), GuiAvatar::TOP_LEFT, this));
    opponent->zoom = 0.9f;
    //opponentExile
    Add(opponentExile = NEW GuiExile(-30 + GuiAvatar::Width * 1.2f - GuiGameZone::Width / 2, 35 + GuiGameZone::Height - 10,
                    false, mpDuelLayers->getRenderedPlayerOpponent(), this));
    //opponentGraveyard
    Add(opponentGraveyard = NEW GuiGraveyard(5 + GuiAvatar::Width * 1.4f - GuiGameZone::Width / 2, 5, false,
                    mpDuelLayers->getRenderedPlayerOpponent(), this));
    //opponentHand
    Add(opponentHand = NEW GuiOpponentHand(-15 + GuiAvatar::Width * 1.4f - GuiGameZone::Width / 2, 35 + GuiGameZone::Height - 10, false,
                    mpDuelLayers->getRenderedPlayerOpponent(), this));
    //opponentLibrary
    Add(opponentLibrary = NEW GuiLibrary(5 + GuiAvatar::Width * 1.4f - GuiGameZone::Width / 2, 5 + GuiGameZone::Height + 5, false,
                    mpDuelLayers->getRenderedPlayerOpponent(), this));

    observer->getCardSelector()->Add(self);
    observer->getCardSelector()->Add(selfGraveyard);
    observer->getCardSelector()->Add(selfExile);
    observer->getCardSelector()->Add(selfLibrary);
    observer->getCardSelector()->Add(opponent);
    observer->getCardSelector()->Add(opponentGraveyard);
    observer->getCardSelector()->Add(opponentExile);
    observer->getCardSelector()->Add(opponentLibrary);
    observer->getCardSelector()->Add(opponentHand);
    selfGraveyard->alpha = selfExile->alpha = opponentExile->alpha = selfLibrary->alpha = opponentGraveyard->alpha = opponentLibrary->alpha = opponentHand->alpha = 0;
}

float GuiAvatars::LeftBoundarySelf()
{
    return SCREEN_WIDTH - 10;
}

GuiAvatars::~GuiAvatars()
{
}

void GuiAvatars::Activate(PlayGuiObject* c)
{
    c->zoom = 1.2f;
    c->mHasFocus = true;

    if ((opponentGraveyard == c) || (opponentExile == c) || (opponentLibrary == c) || (opponent == c) || (opponentHand == c))
    {
        opponentGraveyard->alpha = opponentExile->alpha = opponentLibrary->alpha = opponentHand->alpha = 128.0f;
        active = opponent;
        opponent->zoom = 1.2f;
    }
    else if ((selfGraveyard == c) || (selfExile == c) || (selfLibrary == c) || (self == c))
    {
        selfGraveyard->alpha = selfExile->alpha = selfLibrary->alpha = 128.0f;
        self->zoom = 1.0f;
        active = self;
    }
    if (opponent != c && self != c)
        c->alpha = 255.0f;
}
void GuiAvatars::Deactivate(PlayGuiObject* c)
{
    c->zoom = 1.0;
    c->mHasFocus = false;
    if ((opponentGraveyard == c) || (opponentExile == c) || (opponentLibrary == c) || (opponentHand == c) || (opponent == c))
    {
        opponentGraveyard->alpha = opponentExile->alpha = opponentLibrary->alpha = opponentHand->alpha = 0;
        opponent->zoom = 0.9f;
        active = NULL;
    }
    else if ((selfGraveyard == c) || (selfExile == c) || (selfLibrary == c) || (self == c))
    {
        selfGraveyard->alpha = selfExile->alpha = selfLibrary->alpha = 0;
        self->zoom = 0.5f;
        active = NULL;
    }
}

int GuiAvatars::receiveEventPlus(WEvent* e)
{
    return selfGraveyard->receiveEventPlus(e) | selfExile->receiveEventPlus(e) | opponentExile->receiveEventPlus(e) | opponentGraveyard->receiveEventPlus(e) | opponentHand->receiveEventPlus(e);
}

int GuiAvatars::receiveEventMinus(WEvent* e)
{
    selfGraveyard->receiveEventMinus(e);
    selfExile->receiveEventMinus(e);
    opponentGraveyard->receiveEventMinus(e);
    opponentExile->receiveEventMinus(e);
    opponentHand->receiveEventMinus(e);
    return 1;
}

bool GuiAvatars::CheckUserInput(JButton key)
{
    if (self->CheckUserInput(key))
        return true;
    if (opponent->CheckUserInput(key))
        return true;
    if (selfGraveyard->CheckUserInput(key))
        return true;
    if (selfExile->CheckUserInput(key))
        return true;
    if (opponentGraveyard->CheckUserInput(key))
        return true;
    if (opponentExile->CheckUserInput(key))
        return true;
    if (opponentHand->CheckUserInput(key))
        return true;
    if (selfLibrary->CheckUserInput(key))
        return true;
    if (opponentLibrary->CheckUserInput(key))
        return true;
    return false;
}

void GuiAvatars::Update(float dt)
{
    self->Update(dt);
    opponent->Update(dt);
    selfGraveyard->Update(dt);
    selfExile->Update(dt);
    opponentHand->Update(dt);
    opponentGraveyard->Update(dt);
    opponentExile->Update(dt);
    selfLibrary->Update(dt);
    opponentLibrary->Update(dt);
}

void GuiAvatars::Render()
{
    JRenderer * r = JRenderer::GetInstance();
    float w = 52;
    float h = 52;
    if (opponent == active)
    {
        r->FillRect(opponent->actX, opponent->actY, 34 * opponent->actZ, h+22 * opponent->actZ, ARGB(200,0,0,0));
        r->FillRect(opponent->actX, opponent->actY, (w * opponent->actZ)-1, (h * opponent->actZ)+2, ARGB(200,0,0,0));
    }
    else if (self == active)
    {
        r->FillRect(self->actX - w * self->actZ - 4.5f, self->actY - h-28 * self->actZ, 24 * self->actZ, h+28 * self->actZ, ARGB(200,0,0,0));
        r->FillRect(self->actX - w * self->actZ - 4.5f, self->actY - h * self->actZ, w * self->actZ, h * self->actZ, ARGB(200,0,0,0));
    }
    GuiLayer::Render();

}

GuiAvatar* GuiAvatars::GetSelf()
{
    return self;
}

GuiAvatar* GuiAvatars::GetOpponent()
{
    return opponent;
}
