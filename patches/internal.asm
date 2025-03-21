.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
  .ifdef HookScriptMenuRequestCheck
  .org ShowKeyboard
    push {r3-r8,lr}
    bl HookKeyboardCheck

  .org ShowKeyboardTypeCase3
    b HookKeyboardCustomPrompt

  .org ShowKeyboardReturn
    pop {r3-r8,pc}

  .org PreprocessStringFromIdCallsite
    bl CustomPreprocessStringFromId
  .endif
.close

.open "overlay11.bin", overlay11_start
  .ifdef cotInternalTrampolineScriptSpecialProcessCall
  .org ScriptSpecialProcessCall
    b cotInternalTrampolineScriptSpecialProcessCall
  .endif

  .ifdef HookScriptMenuRequestCheck
  .org ScriptMenuRequestDefaultCase
    b HookScriptMenuRequestCheck
  
  .org ScriptMenuUpdateDefaultCase
    b HookScriptMenuUpdateCheck
  .endif

  .ifdef HookOpcodeCheck
  .org OpcodeCheck
    b HookOpcodeCheck
  
  .org GetParameterCount
    bl HookGetParameterCount
  .endif
.close 

.open "overlay29.bin", overlay29_start
  .ifdef CUSTOM_ITEM_EFFECTS
  .org ApplyItemEffectHookAddr
    b cotInternalTrampolineApplyItemEffect
  .endif
  .ifdef CUSTOM_MOVE_EFFECTS
  .org ApplyMoveEffectHookAddr
    b cotInternalTrampolineApplyMoveEffect
  .endif
.close
