/* Starter Typesanity
Disabled (default): Hero and partner cannot share a type (vanilla behavior).
Enabled: Hero and partner can share types. Note that the game is not balanced around this!
Starter Options:
Choose: Skip quiz, choose hero. Choose partner as normal.
Override (default): Do quiz, with option to override quiz result for hero. Choose partner as normal.
Vanilla: Do quiz, be stuck with the hero quiz gives you. Choose partner as normal.
Random: Be randomly assigned a hero and partner from the starter pool.*/

#include <pmdsky.h>
#include <cot.h>

void __attribute__((naked)) TypesanityCheck() {
    asm("ldr r0,=apSettings");
    asm("ldr r0,[r0,#0x0]");
    asm("tst r0,#0b0000000000010000");
    asm("bxne lr");
    asm("b SameTypeUnhook");
}

// Clever hook to initalize the question count to the last question and roll
// the player at the same time.
void __attribute__((naked)) ForcedPlayerCheck() {
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000000001100000");
    asm("lsr r1,r1,#0x5");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM 
    asm("ldr r1,[r0,#0x0]");
    asm("movne r3,#0"); // 0 if normal
    asm("moveq r3,#9"); // Initalize to 9 questions if forced player.
    asm("str r3,[r1,#0x24]");
    asm("bne QuizForcedPlayerUnhook");
    asm("stmdb sp!,{r0,r1,r2,r3,r4,r5,r12}");
    asm("ldr r2,=apSlotName");
    asm("ldr r3,=apSeed");
    asm("ldr r4,[r2,#0x0]");
    asm("ldr r5,[r2,#0x4]");
    asm("eor r4,r4,r5");
    asm("lsl r4,r4,#0x2");
    asm("ldr r5,[r2,#0x8]");
    asm("eor r4,r4,r5");
    asm("ldr r5,[r2,#0xC]");
    asm("eor r4,r4,r5");
    asm("ldr r5,[r3,#0x0]");
    asm("add r4,r4,r5");
    asm("ldr r5,[r3,#0x4]");
    asm("eor r4,r4,r5");
    asm("and r4,r4,#0b1111"); // Personality 0-15
    asm("ldr r1,[r0,#0x0]");
    asm("add r1,r1,r4");
    asm("mov r3,#99"); // Set score for personality to be 99.
    asm("strb r3,[r1,#0x34]");
    asm("ldmia sp!,{r0,r1,r2,r3,r4,r5,r12}");
    asm("b QuizForcedPlayerUnhook");
}

// Lazy hook to force a specific partner by... replacing all entries with the
// same monster.
void __attribute__((naked)) ForcedPartnerCheck() {
    asm("mov r7,#0x0"); // Original Instruction
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000000001100000");
    asm("lsr r1,r1,#0x5");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("bxne lr");
    asm("ldr r0,=apSlotName");
    asm("ldr r1,=apSeed");
    asm("ldr r2,[r1,#0x0]");
    asm("ror r2,r2,#0x7");
    asm("ldr r3,[r0,#0x0]");
    asm("ldr r1,[r0,#0x4]");
    asm("eor r3,r3,r1");
    asm("ldr r1,[r0,#0x8]");
    asm("and r3,r3,r1");
    asm("ldr r1,[r0,#0xC]");
    asm("eor r3,r3,r1");
    asm("eor r3,r3,r2");
    asm("and r3,r3,#0b1111");
    asm("ldr r0,=OVERLAY13_PTR_NA_238CEA0");
    asm("ldr r0,[r0,#0x0]");
    asm("add r0,r0,#0x300");
    asm("lsl r2,r3,#0x1");
    asm("ldr r1,=STARTERS_PARTNERS_IDS");
    asm("ldrsh r3,[r1,r2]");
    asm("force_partner_fill_entry_loop:");
    asm("add r1,r0,r7,lsl #0x1");
    asm("strh r3,[r1,#0x74]"); // #0x374
    asm("add r7,r7,#1");
    asm("cmp r7,#0x15");
    asm("blt force_partner_fill_entry_loop");
    asm("str r7,[r0,#0x70]"); // 0x370
    asm("b QuizForcedPartnerUnhook");
}

// Incompatible with Choose Starter. Wip
void QuizCustomStateHandler(void* quizData, int state) {
    switch (state) {
        case 0x42: // Forced Selected Partner
            break;
    }
    return;
}