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
///#define GML_STL 1
///#include "gm_stl/gm_stl.hpp"
///#include "gm_gui_GUI.hpp"
///#include "gm_gui_Calibration.hpp"
///
///#define IR_PIN      22
///#define FOO_PIN     42
///
///gm::gui::GUI gui;
///
///void foo_callback(long, gm::gui::CallbackAction);
///
///void gm_main()
///{
///    Serial.begin(9600);
///    gui.begin();
///    IrReceiver.begin(IR_PIN);
///
///    gui.callbacks.push_back(foo_callback);
///
///    auto now = millis();
///    gui.loading_screen("initializing sensors...", [&]() -> bool {
///        // one second delay per spec
///        if (millis() >= now + 5000)
///            return true;
///
///        for (size_t i = 0; i < gui.callbacks.size(); i++)
///            gui.call(i, gm::gui::CallbackAction::Idle);
///
///        return false;
///    });
///
///    gm_loop()
///    {
///        gui.update();
///
///        if (IrReceiver.decode())
///        {
///            gui.on_input(IrReceiver.decodedIRData.command);
///            IrReceiver.resume();
///        }
///    }
///}
///
///void foo_callback(long ms, gm::gui::CallbackAction action)
///{
///    static gm::gui::Waveform waveform(gui.lcd, 2000, "Title", "x-unit", "y-unit", "symbol");
///
///    static float calibration_variable;
///    float calibration_default = 99.0f;
///
///    static gm::gui::Calibration calibration(gui.lcd, {
///        {
///            .title = "Var Name",
///            .value = &calibration_variable,
///            .default_value = calibration_default,
///        }
///    });
///
///    auto calc_foo = [](float x) { return x + calibration_variable; };
///    int min = 0;
///    int max = 100;
///
///    switch (action)
///    {
///        case gm::gui::CallbackAction::Redraw:
///            calibration.set_active(false);
///            waveform.redraw({ min, max });
///            break;
///        case gm::gui::CallbackAction::Calibrate:
///            calibration.set_active(true);
///            calibration.redraw();
///            break;
///        default:
///            calibration.handle_ir_action(action);
///            break;
///    }
///
///    if (waveform.should_update(ms))
///    {
///        waveform.append(calc_foo(query_foo()));
///        waveform.export_last("foo_json_key");
///
///        if (action != gm::gui::CallbackAction::Idle && !calibration.is_active())
///            waveform.draw({min, max});
///    }
///}
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
