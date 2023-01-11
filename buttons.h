
typedef struct {
    uint8_t frame_x;
    uint8_t frame_y;
    uint8_t frame_w;
    uint8_t frame_h;

    uint8_t text_x;
    uint8_t text_y;
    Font text_font;
    const char* text_text;

    int8_t row;
    int8_t column;

    // CalculatorFunction* function;
} CalculatorDisplayButton;

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberZero =
    &(CalculatorDisplayButton){
        1,
        49,
        18,
        10,
        8,
        58,
        FontSecondary,
        "0",
        4,
        0,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberOne = &(CalculatorDisplayButton){
    1,
    37,
    8,
    10,
    5,
    46,
    FontSecondary,
    "1",
    3,
    0,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberTwo = &(CalculatorDisplayButton){
    11,
    37,
    8,
    10,
    13,
    46,
    FontSecondary,
    "2",
    3,
    1,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberThree =
    &(CalculatorDisplayButton){
        21,
        37,
        8,
        10,
        23,
        46,
        FontSecondary,
        "3",
        3,
        2,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberFour =
    &(CalculatorDisplayButton){
        1,
        25,
        8,
        10,
        3,
        34,
        FontSecondary,
        "4",
        2,
        0,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberFive =
    &(CalculatorDisplayButton){
        11,
        25,
        8,
        10,
        13,
        34,
        FontSecondary,
        "5",
        2,
        1,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberSix = &(CalculatorDisplayButton){
    21,
    25,
    8,
    10,
    23,
    34,
    FontSecondary,
    "6",
    2,
    2,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberSeven =
    &(CalculatorDisplayButton){
        1,
        13,
        8,
        10,
        3,
        22,
        FontSecondary,
        "7",
        1,
        0,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberEight =
    &(CalculatorDisplayButton){
        11,
        13,
        8,
        10,
        13,
        22,
        FontSecondary,
        "8",
        1,
        1,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonNumberNine =
    &(CalculatorDisplayButton){
        21,
        13,
        8,
        10,
        23,
        22,
        FontSecondary,
        "9",
        1,
        2,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonClear = &(CalculatorDisplayButton){
    1,
    1,
    8,
    10,
    3,
    10,
    FontSecondary,
    "C",
    0,
    0,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonNegative = &(CalculatorDisplayButton){
    11,
    1,
    8,
    10,
    13,
    10,
    FontSecondary,
    "-",
    0,
    1,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonFunctionRemainder =
    &(CalculatorDisplayButton){
        21,
        1,
        8,
        10,
        23,
        10,
        FontSecondary,
        "%",
        0,
        2,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonFunctionDivide =
    &(CalculatorDisplayButton){
        31,
        1,
        8,
        10,
        32,
        10,
        FontSecondary,
        "/", // change to asset
        0,
        3,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonFunctionMultiply =
    &(CalculatorDisplayButton){
        31,
        13,
        8,
        10,
        33,
        23,
        FontSecondary,
        "*",
        1,
        3,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonFunctionMinus =
    &(CalculatorDisplayButton){
        31,
        25,
        8,
        10,
        33,
        34,
        FontSecondary,
        "-",
        2,
        3,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonFunctionAdd =
    &(CalculatorDisplayButton){
        31,
        37,
        8,
        10,
        33,
        46,
        FontSecondary,
        "+",
        3,
        3,
    };

CalculatorDisplayButton const* const CalculatorDisplayButtonCalculate = &(CalculatorDisplayButton){
    31,
    49,
    8,
    10,
    33,
    58,
    FontSecondary,
    "=",
    4,
    3,
};

CalculatorDisplayButton const* const CalculatorDisplayButtonDecimal = &(CalculatorDisplayButton){
    21,
    49,
    8,
    10,
    25,
    58,
    FontSecondary,
    ".",
    4,
    2,
};

CalculatorDisplayButton const* calculator_display_button_grid[][4] = {
    {
        CalculatorDisplayButtonClear,
        CalculatorDisplayButtonNegative,
        CalculatorDisplayButtonFunctionRemainder,
        CalculatorDisplayButtonFunctionDivide,
    },
    {
        CalculatorDisplayButtonNumberSeven,
        CalculatorDisplayButtonNumberEight,
        CalculatorDisplayButtonNumberNine,
        CalculatorDisplayButtonFunctionMultiply,
    },
    {
        CalculatorDisplayButtonNumberFour,
        CalculatorDisplayButtonNumberFive,
        CalculatorDisplayButtonNumberSix,
        CalculatorDisplayButtonFunctionMinus,
    },
    {
        CalculatorDisplayButtonNumberOne,
        CalculatorDisplayButtonNumberTwo,
        CalculatorDisplayButtonNumberThree,
        CalculatorDisplayButtonFunctionAdd,
    },
    {
        CalculatorDisplayButtonNumberZero,
        CalculatorDisplayButtonNumberZero,
        CalculatorDisplayButtonDecimal,
        CalculatorDisplayButtonCalculate,
    },
};

const size_t NUMBER_OF_ROWS_BUTTONS =
    sizeof(calculator_display_button_grid) / sizeof(calculator_display_button_grid[0]);

const size_t NUMBER_OF_COLUMNS_BUTTONS =
    sizeof(calculator_display_button_grid[0]) / sizeof(calculator_display_button_grid[0][0]);