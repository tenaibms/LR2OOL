#include "gamestate.h"
#include "features/hiterror.h"
#include "hooks/srcnumber.h"

GameState::GameState()
{
    m_mid_hooks.push_back(safetyhook::create_mid(m_offsets.change_gamestate, OnGameStateChange));
}

void GameState::OnGameStateChange(safetyhook::Context& ctx)
{
    GameState& game_state = hooks::game_state;

    game_state.m_current_state = static_cast<StateList>(ctx.eax);
    if (ctx.eax == 13) game_state.m_current_state = StateList::result;
    if (game_state.m_current_state == StateList::playing) {
        hiterror::Reset();
        hooks::src_number.Reset();
    }
}
