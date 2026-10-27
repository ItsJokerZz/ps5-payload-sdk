
asm(".global FinalizeImeModule\n"
    ".type FinalizeImeModule @function\n"
    "FinalizeImeModule:\n");

asm(".global InitializeImeModule\n"
    ".type InitializeImeModule @function\n"
    "InitializeImeModule:\n");

asm(".global sceImeCheckFilterText\n"
    ".type sceImeCheckFilterText @function\n"
    "sceImeCheckFilterText:\n");

asm(".global sceImeCheckRemoteEventParam\n"
    ".type sceImeCheckRemoteEventParam @function\n"
    "sceImeCheckRemoteEventParam:\n");

asm(".global sceImeCheckUpdateTextInfo\n"
    ".type sceImeCheckUpdateTextInfo @function\n"
    "sceImeCheckUpdateTextInfo:\n");

asm(".global sceImeClose\n"
    ".type sceImeClose @function\n"
    "sceImeClose:\n");

asm(".global sceImeConfigGet\n"
    ".type sceImeConfigGet @function\n"
    "sceImeConfigGet:\n");

asm(".global sceImeConfigSet\n"
    ".type sceImeConfigSet @function\n"
    "sceImeConfigSet:\n");

asm(".global sceImeConfirmCandidate\n"
    ".type sceImeConfirmCandidate @function\n"
    "sceImeConfirmCandidate:\n");

asm(".global sceImeDicAddWord\n"
    ".type sceImeDicAddWord @function\n"
    "sceImeDicAddWord:\n");

asm(".global sceImeDicDeleteLearnDics\n"
    ".type sceImeDicDeleteLearnDics @function\n"
    "sceImeDicDeleteLearnDics:\n");

asm(".global sceImeDicDeleteUserDics\n"
    ".type sceImeDicDeleteUserDics @function\n"
    "sceImeDicDeleteUserDics:\n");

asm(".global sceImeDicDeleteWord\n"
    ".type sceImeDicDeleteWord @function\n"
    "sceImeDicDeleteWord:\n");

asm(".global sceImeDicGetWords\n"
    ".type sceImeDicGetWords @function\n"
    "sceImeDicGetWords:\n");

asm(".global sceImeDicReplaceWord\n"
    ".type sceImeDicReplaceWord @function\n"
    "sceImeDicReplaceWord:\n");

asm(".global sceImeDisableController\n"
    ".type sceImeDisableController @function\n"
    "sceImeDisableController:\n");

asm(".global sceImeFilterText\n"
    ".type sceImeFilterText @function\n"
    "sceImeFilterText:\n");

asm(".global sceImeForTestFunction\n"
    ".type sceImeForTestFunction @function\n"
    "sceImeForTestFunction:\n");

asm(".global sceImeGetPanelPositionAndForm\n"
    ".type sceImeGetPanelPositionAndForm @function\n"
    "sceImeGetPanelPositionAndForm:\n");

asm(".global sceImeGetPanelSize\n"
    ".type sceImeGetPanelSize @function\n"
    "sceImeGetPanelSize:\n");

asm(".global sceImeKeyboardClose\n"
    ".type sceImeKeyboardClose @function\n"
    "sceImeKeyboardClose:\n");

asm(".global sceImeKeyboardGetInfo\n"
    ".type sceImeKeyboardGetInfo @function\n"
    "sceImeKeyboardGetInfo:\n");

asm(".global sceImeKeyboardGetResourceId\n"
    ".type sceImeKeyboardGetResourceId @function\n"
    "sceImeKeyboardGetResourceId:\n");

asm(".global sceImeKeyboardOpen\n"
    ".type sceImeKeyboardOpen @function\n"
    "sceImeKeyboardOpen:\n");

asm(".global sceImeKeyboardOpenInternal\n"
    ".type sceImeKeyboardOpenInternal @function\n"
    "sceImeKeyboardOpenInternal:\n");

asm(".global sceImeKeyboardSetMode\n"
    ".type sceImeKeyboardSetMode @function\n"
    "sceImeKeyboardSetMode:\n");

asm(".global sceImeKeyboardUpdate\n"
    ".type sceImeKeyboardUpdate @function\n"
    "sceImeKeyboardUpdate:\n");

asm(".global sceImeOpen\n"
    ".type sceImeOpen @function\n"
    "sceImeOpen:\n");

asm(".global sceImeParamInit\n"
    ".type sceImeParamInit @function\n"
    "sceImeParamInit:\n");

asm(".global sceImeSetCandidateIndex\n"
    ".type sceImeSetCandidateIndex @function\n"
    "sceImeSetCandidateIndex:\n");

asm(".global sceImeSetCaret\n"
    ".type sceImeSetCaret @function\n"
    "sceImeSetCaret:\n");

asm(".global sceImeSetConsecutiveClauseConversionEnabled\n"
    ".type sceImeSetConsecutiveClauseConversionEnabled @function\n"
    "sceImeSetConsecutiveClauseConversionEnabled:\n");

asm(".global sceImeSetEditEventWithRemotePlayEnabled\n"
    ".type sceImeSetEditEventWithRemotePlayEnabled @function\n"
    "sceImeSetEditEventWithRemotePlayEnabled:\n");

asm(".global sceImeSetText\n"
    ".type sceImeSetText @function\n"
    "sceImeSetText:\n");

asm(".global sceImeSetTextGeometry\n"
    ".type sceImeSetTextGeometry @function\n"
    "sceImeSetTextGeometry:\n");

asm(".global sceImeSetVoiceDictationEnabled\n"
    ".type sceImeSetVoiceDictationEnabled @function\n"
    "sceImeSetVoiceDictationEnabled:\n");

asm(".global sceImeUpdate\n"
    ".type sceImeUpdate @function\n"
    "sceImeUpdate:\n");

asm(".global sceImeVshCheckAutoReConvertingPreeditText\n"
    ".type sceImeVshCheckAutoReConvertingPreeditText @function\n"
    "sceImeVshCheckAutoReConvertingPreeditText:\n");

asm(".global sceImeVshClearPreedit\n"
    ".type sceImeVshClearPreedit @function\n"
    "sceImeVshClearPreedit:\n");

asm(".global sceImeVshClose\n"
    ".type sceImeVshClose @function\n"
    "sceImeVshClose:\n");

asm(".global sceImeVshConfirmPreedit\n"
    ".type sceImeVshConfirmPreedit @function\n"
    "sceImeVshConfirmPreedit:\n");

asm(".global sceImeVshGetPanelPositionAndForm\n"
    ".type sceImeVshGetPanelPositionAndForm @function\n"
    "sceImeVshGetPanelPositionAndForm:\n");

asm(".global sceImeVshInformConfirmdString\n"
    ".type sceImeVshInformConfirmdString @function\n"
    "sceImeVshInformConfirmdString:\n");

asm(".global sceImeVshInformConfirmdString2\n"
    ".type sceImeVshInformConfirmdString2 @function\n"
    "sceImeVshInformConfirmdString2:\n");

asm(".global sceImeVshNotifyCurrentTextInfo\n"
    ".type sceImeVshNotifyCurrentTextInfo @function\n"
    "sceImeVshNotifyCurrentTextInfo:\n");

asm(".global sceImeVshNotifyCurrentTextInfoForReconvertText\n"
    ".type sceImeVshNotifyCurrentTextInfoForReconvertText @function\n"
    "sceImeVshNotifyCurrentTextInfoForReconvertText:\n");

asm(".global sceImeVshNotifyDeleteText\n"
    ".type sceImeVshNotifyDeleteText @function\n"
    "sceImeVshNotifyDeleteText:\n");

asm(".global sceImeVshNotifyPreeditMetaInfo\n"
    ".type sceImeVshNotifyPreeditMetaInfo @function\n"
    "sceImeVshNotifyPreeditMetaInfo:\n");

asm(".global sceImeVshNotifySetText\n"
    ".type sceImeVshNotifySetText @function\n"
    "sceImeVshNotifySetText:\n");

asm(".global sceImeVshNotifyUpdateCaretText\n"
    ".type sceImeVshNotifyUpdateCaretText @function\n"
    "sceImeVshNotifyUpdateCaretText:\n");

asm(".global sceImeVshOpen\n"
    ".type sceImeVshOpen @function\n"
    "sceImeVshOpen:\n");

asm(".global sceImeVshSendTextInfo\n"
    ".type sceImeVshSendTextInfo @function\n"
    "sceImeVshSendTextInfo:\n");

asm(".global sceImeVshSetCaretGeometry\n"
    ".type sceImeVshSetCaretGeometry @function\n"
    "sceImeVshSetCaretGeometry:\n");

asm(".global sceImeVshSetCaretIndexInPreedit\n"
    ".type sceImeVshSetCaretIndexInPreedit @function\n"
    "sceImeVshSetCaretIndexInPreedit:\n");

asm(".global sceImeVshSetPanelPosition\n"
    ".type sceImeVshSetPanelPosition @function\n"
    "sceImeVshSetPanelPosition:\n");

asm(".global sceImeVshSetParam\n"
    ".type sceImeVshSetParam @function\n"
    "sceImeVshSetParam:\n");

asm(".global sceImeVshSetPreeditGeometry\n"
    ".type sceImeVshSetPreeditGeometry @function\n"
    "sceImeVshSetPreeditGeometry:\n");

asm(".global sceImeVshSetSelectGeometry\n"
    ".type sceImeVshSetSelectGeometry @function\n"
    "sceImeVshSetSelectGeometry:\n");

asm(".global sceImeVshSetSelectionText\n"
    ".type sceImeVshSetSelectionText @function\n"
    "sceImeVshSetSelectionText:\n");

asm(".global sceImeVshUpdate\n"
    ".type sceImeVshUpdate @function\n"
    "sceImeVshUpdate:\n");

asm(".global sceImeVshUpdateContext\n"
    ".type sceImeVshUpdateContext @function\n"
    "sceImeVshUpdateContext:\n");

asm(".global sceImeVshUpdateContext2\n"
    ".type sceImeVshUpdateContext2 @function\n"
    "sceImeVshUpdateContext2:\n");
