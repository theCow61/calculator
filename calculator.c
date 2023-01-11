#include <furi.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include "calculator_functions.h"
#include "buttons.h"

#define LIMIT_OF_CHAIN 10

// Functions are evaluated immidietely and so are division and multiplication.

typedef enum {
    CalculatorStatusAwaitingInput,
} CalculatorStatus;

typedef struct {
    uint8_t row;
    uint8_t column;
} CalculatorDisplayElementGridLocation;

// This will either be evaluated and chained or just chained
typedef struct {
    // CalculatorFunctionType type;
    // CalculatorCalculateFunctionOneInput* function_one;
    // CalculatorCalculateFunctionTwoInput* function_two;
    const CalculatorFunction* function;
    double value;
} CalculatorCalculation;

typedef struct {
    double result;
    FuriMessageQueue* operation_queue; // To be added all together Type CalculatorCalculation
} Calculator;

typedef struct {
    ViewPort* vp;
    Gui* gui;
    FuriMessageQueue* msq;
    // TODO: Mutex
    Calculator* calculator;

    FuriMutex* mutex;

    // CalculatorDisplayElementGridLocation selected_grid_element;
    CalculatorDisplayButton const* selected_button;
} CalculatorApp;

void draw_buttons(Canvas* canvas, CalculatorApp* clc_app) {
    for(size_t i = 0; i < NUMBER_OF_ROWS_BUTTONS; i++) {
        for(size_t j = 0; j < NUMBER_OF_COLUMNS_BUTTONS; j++) {
            CalculatorDisplayButton const* button = calculator_display_button_grid[i][j];
            if(button == clc_app->selected_button) {
                canvas_invert_color(canvas);
            }

            elements_frame(
                canvas, button->frame_x, button->frame_y, button->frame_w, button->frame_h);

            canvas_draw_str(canvas, button->text_x, button->text_y, button->text_text);

            if(button == clc_app->selected_button) {
                canvas_invert_color(canvas);
            }
        }
    }
}

void draw_callback(Canvas* canvas, void* ctx) {
    // UNUSED(ctx);
    // UNUSED(canvas);
    CalculatorApp* clc_app = ctx;
    furi_check(furi_mutex_acquire(clc_app->mutex, FuriWaitForever) == FuriStatusOk);

    // char buffer[8];
    // snprintf(buffer, sizeof(buffer) + 1, "%lf", clc_app->calculator->result);

    // elements_bubble_str(canvas, 128 / 2, 64 / 2, "b", AlignCenter, AlignCenter);
    // elements_bubble_str(canvas, 128 / 2, 64 / 2, buffer, AlignCenter, AlignCenter);

    // elements_frame(canvas, 2, 40, 124, 22);
    // elements_bubble_str(canvas, 2, 40, )

    // switch(clc_app->selected_grid_element) {
    // case(CalculatorDisplayElementGridLocation){.column = 0, .row = 0}:
    // }

    draw_buttons(canvas, clc_app);

    furi_mutex_release(clc_app->mutex);
}

void input_callback(InputEvent* evt, void* ctx) {
    CalculatorApp* clc_app = ctx;
    furi_message_queue_put(clc_app->msq, evt, FuriWaitForever);
}

void calculator_reset(Calculator* clc) {
    clc->result = 0;
    furi_message_queue_reset(clc->operation_queue);
}

void calculator_add_calculator_calculation(Calculator* clc, CalculatorCalculation* calculation) {
    if(calculation->function->type == CalculatorFunctionTypeTwoInput) {
        CalculatorCalculation previous_calculation;
        // Hopefully message_queue_get removes element from queue when getted
        // If they just presed divide as first thing without any number before
        if(furi_message_queue_get(clc->operation_queue, &previous_calculation, FuriWaitForever) ==
           FuriStatusError) {
            calculator_reset(clc);
            return;
        }

        // calculation->value = previous_calculation.value * calculation->value;
        calculation->value =
            calculation->function->two_param_func(previous_calculation.value, calculation->value);

    } else if(calculation->function->type == CalculatorFunctionTypeOneInput) {
        calculation->value = calculation->function->one_param_func(calculation->value);
    }

    // calculation->function->type =
    // CalculatorFunctionTypePassive; // May be issue here, shouldn't change
    furi_message_queue_put(clc->operation_queue, calculation, FuriWaitForever);
}

CalculatorCalculation* calculator_calculation_alloc(const CalculatorFunction* func, double value) {
    CalculatorCalculation* calc_calc = malloc(sizeof(CalculatorCalculation));
    calc_calc->function = func;
    calc_calc->value = value;
    return calc_calc;
}

Calculator* calculator_alloc() {
    Calculator* clc = malloc(sizeof(Calculator));
    clc->operation_queue = furi_message_queue_alloc(LIMIT_OF_CHAIN, sizeof(CalculatorCalculation));
    clc->result = 0;
    return clc;
}

void calculator_free(Calculator* clc) {
    furi_message_queue_free(clc->operation_queue);
    free(clc);
}

CalculatorApp* calculator_app_alloc() {
    CalculatorApp* clc_app = malloc(sizeof(CalculatorApp));

    clc_app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    clc_app->selected_button = calculator_display_button_grid[0][0];

    clc_app->calculator = calculator_alloc();
    clc_app->vp = view_port_alloc();
    view_port_enabled_set(clc_app->vp, true);
    view_port_draw_callback_set(clc_app->vp, draw_callback, clc_app);
    view_port_input_callback_set(clc_app->vp, input_callback, clc_app);

    clc_app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(clc_app->gui, clc_app->vp, GuiLayerFullscreen);

    clc_app->msq = furi_message_queue_alloc(8, sizeof(InputEvent));

    return clc_app;
}

void calculator_app_free(CalculatorApp* clc_app) {
    calculator_free(clc_app->calculator);
    furi_message_queue_free(clc_app->msq);
    gui_remove_view_port(clc_app->gui, clc_app->vp);
    furi_record_close(RECORD_GUI);
    view_port_enabled_set(clc_app->vp, false);
    view_port_free(clc_app->vp);
    furi_mutex_free(clc_app->mutex);
    free(clc_app);
}

int32_t calculator_main(void* p) {
    UNUSED(p);

    FURI_LOG_E("Calculator", "Yo");
    CalculatorApp* clc_app = calculator_app_alloc();
    // furi_mutex_acquire(clc_app->mutex, FuriWaitForever);
    CalculatorCalculation* first = calculator_calculation_alloc(&CalculatorFunctionAdd, 10);
    CalculatorCalculation* second = calculator_calculation_alloc(&CalculatorFunctionDivide, 5);
    calculator_add_calculator_calculation(clc_app->calculator, first);
    calculator_add_calculator_calculation(clc_app->calculator, second);

    CalculatorCalculation calc_calc;

    furi_assert(
        furi_message_queue_get(
            clc_app->calculator->operation_queue, &calc_calc, FuriWaitForever) == FuriStatusOk);
    // TODO: instead of above, have a .solve() that sets something to result or something whichs sums up the message queue

    furi_assert(calc_calc.value == 2); // VALUE IS CORRECT. calc_calc.value = 2!!

    clc_app->calculator->result = calc_calc.value;
    // furi_mutex_release(clc_app->mutex);

    bool running = true;

    InputEvent evt;

    while(running && furi_message_queue_get(clc_app->msq, &evt, FuriWaitForever) == FuriStatusOk) {
        // if(furi_message_queue_get(clc_app->msq, &evt, FuriWaitForever) == FuriStatusOk) {
        if(evt.type == InputTypePress) {
            switch(evt.key) {
            case InputKeyUp:
                if(clc_app->selected_button->row == 0) {
                    clc_app->selected_button =
                        calculator_display_button_grid[NUMBER_OF_ROWS_BUTTONS - 1]
                                                      [clc_app->selected_button->column];
                    break;
                }

                if(clc_app->selected_button ==
                   calculator_display_button_grid[clc_app->selected_button->row - 1]
                                                 [clc_app->selected_button->column]) {
                    if(clc_app->selected_button->row - 2 < 0) {
                        clc_app->selected_button =
                            calculator_display_button_grid[NUMBER_OF_ROWS_BUTTONS - 1]
                                                          [clc_app->selected_button->column];
                        break;
                    }
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row - 2]
                                                      [clc_app->selected_button->column];
                    break;
                }

                clc_app->selected_button =
                    calculator_display_button_grid[clc_app->selected_button->row - 1]
                                                  [clc_app->selected_button->column];
                break;
            case InputKeyDown:
                if(clc_app->selected_button->row == NUMBER_OF_ROWS_BUTTONS - 1) {
                    clc_app->selected_button =
                        calculator_display_button_grid[0][clc_app->selected_button->column];
                    break;
                }

                if(clc_app->selected_button ==
                   calculator_display_button_grid[clc_app->selected_button->row + 1]
                                                 [clc_app->selected_button->column]) {
                    if(clc_app->selected_button->row + 2 >= (int)NUMBER_OF_ROWS_BUTTONS) {
                        clc_app->selected_button =
                            calculator_display_button_grid[0][clc_app->selected_button->column];
                        break;
                    }
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row + 2]
                                                      [clc_app->selected_button->column];
                    break;
                }

                clc_app->selected_button =
                    calculator_display_button_grid[clc_app->selected_button->row + 1]
                                                  [clc_app->selected_button->column];
                break;
            case InputKeyLeft:
                if(clc_app->selected_button->column == 0) {
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row]
                                                      [NUMBER_OF_COLUMNS_BUTTONS - 1];
                    break;
                }

                if(clc_app->selected_button ==
                   calculator_display_button_grid[clc_app->selected_button->row]
                                                 [clc_app->selected_button->column - 1]) {
                    if(clc_app->selected_button->column - 2 < 0) {
                        clc_app->selected_button =
                            calculator_display_button_grid[clc_app->selected_button->row]
                                                          [NUMBER_OF_COLUMNS_BUTTONS - 1];
                        break;
                    }
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row]
                                                      [clc_app->selected_button->column - 2];
                    break;
                }

                clc_app->selected_button =
                    calculator_display_button_grid[clc_app->selected_button->row]
                                                  [clc_app->selected_button->column - 1];
                break;
            case InputKeyRight:
                if(clc_app->selected_button->column == NUMBER_OF_COLUMNS_BUTTONS - 1) {
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row][0];
                    break;
                }
                if(clc_app->selected_button ==
                   calculator_display_button_grid[clc_app->selected_button->row]
                                                 [clc_app->selected_button->column + 1]) {
                    if(clc_app->selected_button->column + 2 >= (int)NUMBER_OF_COLUMNS_BUTTONS) {
                        clc_app->selected_button =
                            calculator_display_button_grid[clc_app->selected_button->row][0];
                        break;
                    }
                    clc_app->selected_button =
                        calculator_display_button_grid[clc_app->selected_button->row]
                                                      [clc_app->selected_button->column + 2];
                    break;
                }
                clc_app->selected_button =
                    calculator_display_button_grid[clc_app->selected_button->row]
                                                  [clc_app->selected_button->column + 1];
                break;
            case InputKeyOk:
                break;
            case InputKeyBack:
                running = false;
                break;
            default:
                break;
            }
            // }
        }
        view_port_update(clc_app->vp);
    }

    calculator_app_free(clc_app);

    return 0;
}