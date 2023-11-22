#pragma once

/// @file gm_io_IRcodes.hpp
///
/// @brief supply IRremote codes as constexpr vars
///
/// +-------+-------+-------+
/// | POWER | VOLP  | FUNC  |
/// +-------+-------+-------+
/// | BACK  | PLAY  | NEXT  |
/// +-------+-------+-------+
/// | DOWN  | VOLM  | UP    |
/// +-------+-------+-------+
/// | ZERO  | EQ    | ST    |
/// +-------+-------+-------+
/// | ONE   | TWO   | THREE |
/// +-------+-------+-------+
/// | FOUR  | FIVE  | SIX   |
/// +-------+-------+-------+
/// | SEVEN | EIGHT | NINE  |
/// +-------+-------+-------+

namespace gm::io {

constexpr auto IR_POWER = 69; ///< @brief IR button POWER
constexpr auto IR_VOLP = 70;  ///< @brief IR button VOLP
constexpr auto IR_FUNC = 71;  ///< @brief IR button FUNC
constexpr auto IR_BACK = 68;  ///< @brief IR button BACK
constexpr auto IR_PLAY = 64;  ///< @brief IR button PLAY
constexpr auto IR_NEXT = 67;  ///< @brief IR button NEXT
constexpr auto IR_DOWN = 7;   ///< @brief IR button DOWN
constexpr auto IR_VOLM = 21;  ///< @brief IR button VOLM
constexpr auto IR_UP = 9;     ///< @brief IR button UP
constexpr auto IR_ZERO = 22;  ///< @brief IR button ZERO
constexpr auto IR_EQ = 25;    ///< @brief IR button EQ
constexpr auto IR_ST = 13;    ///< @brief IR button ST
constexpr auto IR_ONE = 12;   ///< @brief IR button ONE
constexpr auto IR_TWO = 24;   ///< @brief IR button TWO
constexpr auto IR_THREE = 94; ///< @brief IR button THREE
constexpr auto IR_FOUR = 8;   ///< @brief IR button FOUR
constexpr auto IR_FIVE = 28;  ///< @brief IR button FIVE
constexpr auto IR_SIX = 90;   ///< @brief IR button SIX
constexpr auto IR_SEVEN = 66; ///< @brief IR button SEVEN
constexpr auto IR_EIGHT = 82; ///< @brief IR button EIGHT
constexpr auto IR_NINE = 74;  ///< @brief IR button NINE

} // namespace gm