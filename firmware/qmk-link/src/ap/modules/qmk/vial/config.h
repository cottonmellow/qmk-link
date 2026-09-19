#pragma once

// Vial 트리의 QMK 설정.
//
// 공용 설정은 keyboards/qmk-link/config.h 에 있다 (via 트리와 같은 것을 본다).
// 여기에는 Vial 고유의 것만 둔다.

#include QMK_BOARD_CONFIG_H


// ★ BUILD_ID — Vial 에서 EEPROM 유효성 매직으로 쓰인다 (via.c 의 via_eeprom_is_valid).
//
//   vial-qmk 의 util/build_id.py 는 이 값을 **빌드마다 난수로** 만든다.
//   그러면 다시 구울 때마다 EEPROM 이 무효가 되어 키맵이 초기화된다.
//   여기서는 고정값을 박는다 — EEPROM 배치를 바꿀 때만 손으로 올린다.
#define BUILD_ID ((uint32_t)0x00514C4B)   /* "QLK" */


// ★ 키보드 UID — Vial 앱이 장치를 구별하는 값이다.
//
//   vial-qmk 의 `vial generate-keyboard-uid` 가 8바이트 난수를 뽑는 것과 같다.
//   한 번 정하면 바꾸지 않는다. 바꾸면 앱이 다른 키보드로 본다.
#define VIAL_KEYBOARD_UID {0x6B, 0xF3, 0x8A, 0x88, 0x58, 0x4F, 0x4F, 0x39}


// ★ unlock 조합 — 이 키들을 몇 초 누르고 있어야 편집이 열린다.
//
//   좌표가 HID usage 라 물리 위치와 무관하다. 그래서 고르는 기준이 다르다 —
//   **어떤 키보드에나 있는 키**여야 한다. 60% 에는 오른쪽 Ctrl 이 없는 것도 있다.
//
//   좌우 Shift 로 정했다. 없는 키보드가 사실상 없고, 둘을 몇 초씩 함께 누르는 일도
//   실수로는 잘 일어나지 않는다.
//
//     LSFT usage 0xE1 -> row 14, col 1
//     RSFT usage 0xE5 -> row 14, col 5
#define VIAL_UNLOCK_COMBO_ROWS {14, 14}
#define VIAL_UNLOCK_COMBO_COLS { 1,  5}


// ★ QMK Settings 가 전제하는 두 기능.
//
//   qmk_settings.c 가 get_chordal_hold_default() 와 is_flow_tap_key() 를
//   가드 없이 부른다. 그 둘은 action_tapping.h 에서 각각
//   #ifdef CHORDAL_HOLD / #ifdef FLOW_TAP_TERM 안에 있어서,
//   안 켜면 implicit declaration 으로 컴파일이 깨진다.
//
//   QMK Settings 가 이 둘의 토글을 UI 로 내주므로 켜는 게 맞다.
//   실제 동작 여부는 런타임 값이 정한다 (QS_tapping_chordal_hold / QS.flow_tap_term).
#define CHORDAL_HOLD
#define FLOW_TAP_TERM               150

// ★ vial 트리에서는 TAPPING_TERM_PER_KEY 를 켠다 — via 와 반대다.
//
//   upstream vial.c 는 tap_dance_count() / tap_dance_get() 을 이 매크로 안에
//   넣어 뒀다 (#endif 중첩 실수다. VIAL_TAP_DANCE_ENABLE 로 감쌌어야 한다).
//   안 켜면 process_tap_dance.c 가 그 둘을 못 찾는다.
//
//   그래서 get_tapping_term() 의 주인이 vial.c 가 된다 -> QMK Settings 로 이어진다.
//   우리 via_port.c 의 훅은 #ifndef VIAL_ENABLE 로 빠진다.
#define TAPPING_TERM_PER_KEY

// Auto Shift — QMK Settings 가 set_autoshift_timeout() 을 무조건 부른다.
// 기본값은 꺼짐이다 (QS.auto_shift = 0). UI 에서 켤 수 있다.
#define AUTO_SHIFT_ENABLE


// Vial 기능별 엔트리 개수. 각 엔트리가 10바이트라 EEPROM 부담은 작다.
//
//   tap dance 16 x 10 = 160 B
//   combo     16 x 10 = 160 B
//   override   8 x 10 =  80 B
//   QMK Settings       =  40 B
//                       ------
//                        440 B   (동적 키맵 4096 B 와 함께 16KB 안에 들어간다)
//
// 안 적으면 vial.h 가 EEPROM 크기를 보고 알아서 정하는데, 우리 16KB 면
// 큰 값을 골라 매크로 버퍼를 잡아먹는다. 명시한다.
#define VIAL_TAP_DANCE_ENTRIES       32
#define VIAL_COMBO_ENTRIES           16
#define VIAL_KEY_OVERRIDE_ENTRIES    32
#define VIAL_ALT_REPEAT_KEY_ENTRIES   8
