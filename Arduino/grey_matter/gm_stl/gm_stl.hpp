#pragma once

/// @mainpage Grey Matter Embedded API
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
/// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
/// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be
/// included in all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
/// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
/// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
/// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///
/// Example:
/// @code
/// 
/// #define GML_STL 1
/// #include "gm_stl/gm_stl.hpp"
/// #include "gm_gui_GUI.hpp"
/// #include "gm_io_IRcodes.hpp"
/// #include <IRremote.hpp>
/// 
/// #define PREV 68
/// #define NEXT 67
/// 
/// gm::gui::GUI gui;
/// 
/// void any_callback(long, gm::gui::CallbackAction);
/// 
/// void gm_main()
/// {
///     Serial.begin(9600);
///     gui.begin();
///     IrReceiver.begin(IR_PIN);
/// 
///     gui.callbacks.push_back(any_callback);
/// 
///     auto now = millis();
///     gui.loading_screen("initializing sensors...", [&]() -> bool {
///         // one second delay per spec
///         if (millis() >= now + 5000)
///             return true;
/// 
///         for (size_t i = 0; i < gui.callbacks.size(); i++)
///             gui.call(i, gm::gui::CallbackAction::Idle);
/// 
///         return false;
///     });
/// 
///     size_t index = 0;
///     gui.call(index, gm::gui::CallbackAction::Redraw);
/// 
///     gm_loop()
///     {
///         for (size_t i = 0; i < gui.callbacks.size(); i++)
///         {
///             i == index
///                 ? gui.call(i, gm::gui::CallbackAction::Draw)
///                 : gui.call(i, gm::gui::CallbackAction::Idle);
///         }
/// 
///         if (IrReceiver.decode())
///         {
///             switch (IrReceiver.decodedIRData.command)
///             {
///             case gm::io::BACK:
///                 index = (index - 1 + gui.callbacks.size()) % gui.callbacks.size();
///                 gui.call(index, gm::gui::CallbackAction::Redraw);
///                 break;
///             case gm::io::NEXT:
///                 index = (index + 1) % gui.callbacks.size();
///                 gui.call(index, gm::gui::CallbackAction::Redraw);
///                 break;
///             }
///             IrReceiver.resume();
///         }
///     }
/// }
/// 
/// void any_callback(long ms, gm::gui::CallbackAction action)
/// {
///     static gm::gui::Waveform waveform(gui.lcd, 2000, "Title", "X Unit", "Y Unit", "Sym");
/// 
///     if (action == gm::gui::CallbackAction::Redraw)
///         waveform.redraw({ 0, 100 });
/// 
///     if (waveform.should_update(ms))
///     {
///         waveform.append(readAnySensor());
///         waveform.export_last("Sensor-Name");
/// 
///         if (action != gm::gui::CallbackAction::Idle)
///         {
///             waveform.draw({0, 100});
///         }
///     }
/// }
///
/// @endcode

#if GML_STL
#   include "algorithm.hpp"
#   include "initializer_list.hpp"
#   include "iterator.hpp"
#   include "vector.hpp"
#endif // GML_STL

/// @def gm_main
/// @brief main entry point for Grey Matter Embedded

/// @def gm_loop
/// @brief Grey Matter Event loop


#define gm_main() loop(){} void setup()
#define gm_loop(_Expr) for (;;_Expr)
