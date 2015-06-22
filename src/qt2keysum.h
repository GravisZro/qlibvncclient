#ifndef __QT2KEYSYM_H__
#define __QT2KEYSYM_H__

#include <Qt>
#include <rfb/rfb.h>
#include <rfb/keysym.h>

static rfbKeySym qt2keysym(int qtkey)
{
    // FIXME: incomplete (but mostly done)
    switch (qtkey) {
    case Qt::Key_Up: return XK_Up;
    case Qt::Key_Down: return XK_Down;
    case Qt::Key_Left: return XK_Left;
    case Qt::Key_Right: return XK_Right;
    case Qt::Key_Escape: return XK_Escape;
    case Qt::Key_Tab: return XK_Tab;
    case Qt::Key_Space: return XK_space;
    case Qt::Key_Backspace: return XK_BackSpace;
    case Qt::Key_Delete: return XK_Delete;
    case Qt::Key_Insert: return XK_Insert;
    case Qt::Key_CapsLock: return XK_Caps_Lock;
    case Qt::Key_NumLock: return XK_Num_Lock;
    case Qt::Key_ScrollLock: return XK_Scroll_Lock;
    case Qt::Key_Home: return XK_Home;
    case Qt::Key_End: return XK_End;
    case Qt::Key_Print: return XK_Print;
    case Qt::Key_Return: return XK_Return;
    case Qt::Key_Enter: return XK_KP_Enter;
    case Qt::Key_Pause: return XK_Pause;
    case Qt::Key_Shift: return XK_Shift_L;
    case Qt::Key_Control: return XK_Control_L;
    case Qt::Key_Alt: return XK_Alt_L;
    case Qt::Key_AltGr: return XK_Meta_R;
    case Qt::Key_PageDown: return XK_Page_Down;
    case Qt::Key_PageUp: return XK_Page_Up;
    case Qt::Key_Exclam: return XK_exclam;
    case Qt::Key_QuoteDbl: return XK_quotedbl;
    case Qt::Key_NumberSign: return XK_numbersign;
    case Qt::Key_Dollar: return XK_dollar;
    case Qt::Key_Percent: return XK_percent;
    case Qt::Key_Ampersand: return XK_ampersand;
    case Qt::Key_Apostrophe: return XK_apostrophe;
    case Qt::Key_ParenLeft: return XK_parenleft;
    case Qt::Key_ParenRight: return XK_parenright;
    case Qt::Key_Asterisk: return XK_asterisk;
    case Qt::Key_Plus: return XK_plus;
    case Qt::Key_Minus: return XK_minus;
    case Qt::Key_Comma: return XK_comma;
    case Qt::Key_Period: return XK_period;
    case Qt::Key_Slash: return XK_slash;
    case Qt::Key_Backslash: return XK_backslash;
    case Qt::Key_Colon: return XK_colon;
    case Qt::Key_Semicolon: return XK_semicolon;
    case Qt::Key_Less: return XK_less;
    case Qt::Key_Equal: return XK_equal;
    case Qt::Key_Greater: return XK_greater;
    case Qt::Key_Question: return XK_question;
    case Qt::Key_At: return XK_at;
    case Qt::Key_BracketLeft: return XK_bracketleft;
    case Qt::Key_BracketRight: return XK_bracketright;
    case Qt::Key_Underscore: return XK_underscore;
    case Qt::Key_QuoteLeft: return XK_quoteleft;
    case Qt::Key_BraceLeft: return XK_braceleft;
    case Qt::Key_BraceRight: return XK_braceright;
    case Qt::Key_AsciiCircum: return XK_dead_circumflex;
    case Qt::Key_AsciiTilde: return XK_dead_tilde;
    case Qt::Key_questiondown: return XK_questiondown;
    case Qt::Key_exclamdown: return XK_exclamdown;
    case Qt::Key_1: return XK_1;
    case Qt::Key_2: return XK_2;
    case Qt::Key_3: return XK_3;
    case Qt::Key_4: return XK_4;
    case Qt::Key_5: return XK_5;
    case Qt::Key_6: return XK_6;
    case Qt::Key_7: return XK_7;
    case Qt::Key_8: return XK_8;
    case Qt::Key_9: return XK_9;
    case Qt::Key_0: return XK_0;
    case Qt::Key_A: return XK_a;
    case Qt::Key_B: return XK_b;
    case Qt::Key_C: return XK_c;
    case Qt::Key_D: return XK_d;
    case Qt::Key_E: return XK_e;
    case Qt::Key_F: return XK_f;
    case Qt::Key_G: return XK_g;
    case Qt::Key_H: return XK_h;
    case Qt::Key_I: return XK_i;
    case Qt::Key_J: return XK_j;
    case Qt::Key_K: return XK_k;
    case Qt::Key_L: return XK_l;
    case Qt::Key_M: return XK_m;
    case Qt::Key_N: return XK_n;
    case Qt::Key_O: return XK_o;
    case Qt::Key_P: return XK_p;
    case Qt::Key_Q: return XK_q;
    case Qt::Key_R: return XK_r;
    case Qt::Key_S: return XK_s;
    case Qt::Key_T: return XK_t;
    case Qt::Key_U: return XK_u;
    case Qt::Key_V: return XK_v;
    case Qt::Key_W: return XK_w;
    case Qt::Key_X: return XK_x;
    case Qt::Key_Y: return XK_y;
    case Qt::Key_Z: return XK_z;
    case Qt::Key_F1: return XK_F1;
    case Qt::Key_F2: return XK_F2;
    case Qt::Key_F3: return XK_F3;
    case Qt::Key_F4: return XK_F4;
    case Qt::Key_F5: return XK_F5;
    case Qt::Key_F6: return XK_F6;
    case Qt::Key_F7: return XK_F7;
    case Qt::Key_F8: return XK_F8;
    case Qt::Key_F9: return XK_F9;
    case Qt::Key_F10: return XK_F10;
    case Qt::Key_F11: return XK_F11;
    case Qt::Key_F12: return XK_F12;
    case Qt::Key_F13: return XK_F13;
    case Qt::Key_F14: return XK_F14;
    case Qt::Key_F15: return XK_F15;
    case Qt::Key_F16: return XK_F16;
    case Qt::Key_F17: return XK_F17;
    case Qt::Key_F18: return XK_F18;
    case Qt::Key_F19: return XK_F19;
    case Qt::Key_F20: return XK_F20;
    case Qt::Key_F21: return XK_F21;
    case Qt::Key_F22: return XK_F22;
    case Qt::Key_F23: return XK_F23;
    case Qt::Key_F24: return XK_F24;
    case Qt::Key_F25: return XK_F25;
    case Qt::Key_F26: return XK_F26;
    case Qt::Key_F27: return XK_F27;
    case Qt::Key_F28: return XK_F28;
    case Qt::Key_F29: return XK_F29;
    case Qt::Key_F30: return XK_F30;
    case Qt::Key_F31: return XK_F31;
    case Qt::Key_F32: return XK_F32;
    case Qt::Key_F33: return XK_F33;
    case Qt::Key_F34: return XK_F34;
    case Qt::Key_F35: return XK_F35;

    default: return -1;
    }
}

#endif
