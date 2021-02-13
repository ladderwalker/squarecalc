
//Optional: Use OpenGL to do rendering on appropriate systems.
#define CNFGOGL

#define HAS_XSHAPE
#define HAS_XINERAMA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CNFG_IMPLEMENTATION
#include "rawdraw/os_generic.h"
#include "rawdraw/CNFG.h"
#define RDUI_IMPLEMENTATION
#include "rdui/RDUI.h"
#include "rdui/default-elements.h"
#define ONLYINPUT_IMPLEMENTATION
#include "onlyinput/onlyinput.h"


#include "rdui/util.h"

static struct RDUIMenu* menu;

// Global variables
//------------------
char* xvalue;
char* yvalue;

char str1[3] = " ";
double side_x = 0;
double side_y = 0;
double side_x_dec = 0;
double side_y_dec = 0;
double hypote = 0;
double fracdec[] = { 0.0, 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.4375, 0.5, 0.5625,
0.625, 0.6875, 0.75, 0.8125, 0.875, 0.9375 };
char final_hypo[50];

void HandleKey( int keycode, int bDown ) {
    OIHandleKey(keycode, bDown);
	RDUIHandleKeyImpl(menu, keycode, bDown);

	printf("key %d\n", keycode);
}

void HandleButton( int x, int y, int button, int bDown ) {
    RDUIHandleButtonImpl(menu, x, y, button, bDown);
}

void HandleMotion( int x, int y, int mask ) {
    RDUIHandleMotionImpl(menu, x, y, mask);
}

void HandleDestroy() {}




void ButtonClickedHandler(struct RDUIButtonData* data) {
    //do the math
    side_x += side_x_dec;
    side_y += side_y_dec;
    hypote = sqrt(( side_x * side_x ) + ( side_y * side_y ));

    //convert to string
    char in[] = " inches";
    sprintf( final_hypo, "%0.2f", hypote );
    strcat( final_hypo, in );

    //clear the variables
    side_x_dec = 0;
    side_y_dec = 0;
    free(xvalue);
    free(yvalue);
}



void OptionsBoxSelectHandler_x(struct RDUIOptionsBoxData* data) {
    side_x_dec = fracdec[data->selected_index];
}

void OptionsBoxSelectHandler_y(struct RDUIOptionsBoxData* data) {
    side_y_dec = fracdec[data->selected_index];
}

void FieldTypeHandler_x(struct RDUIFieldData* data) {
    char *ptr;
    side_x = strtod(data->value, &ptr);

}

void FieldTypeHandler_y(struct RDUIFieldData* data) {
    char *ptr;
    side_y = strtod(data->value, &ptr);

}


int main(int argv, char* argc[]) {
    RDUIInit();

    CNFGBGColor = 0x444444ff; //Dark Grey Background

    int winWidth = 1440, winHeight = 777;


    int triABX = 412, triAY = 200, triBCY = 500, triCX = 812;
    int counter = 0;
    int dr = 4;

    //CNFGSetupFullscreen( "SquareCalc", 0 );
    CNFGSetup( "SquareCalc", winWidth, winHeight );

    char* options[] = {"None", "1/16", "1/8", "3/16", "1/4", "5/16", "3/8", "7/16", "1/2",
    "9/16", "5/8", "11/16", "3/4", "13/16", "7/8", "15/16", NULL};



    struct RDUIFieldData field_data_x = {
		.padding = 5,
		.font_size = 5,
		.min_width = 50,
		.max_width = 100,
		.position = {
			.x = 810,
			.y = 370
		},
		.font_color = 0x000000ff,
		.border_color = 0x000000ff,
		.value = "",
		.allowed_symbols = "0123456789",

		.type_handler = FieldTypeHandler_x
	};



	struct RDUIOptionsBoxData options_box_xdata = {
		.position = {
			.x = 915,
			.y = 370
		},
		.color = 0x555555ff,
		.font_color = 0xffffffff,
		.font_size = 4,
		.padding = 0,
		.options = options,

		.select_handler = OptionsBoxSelectHandler_x
	};

    struct RDUIFieldData field_data_y = {
		.padding = 5,
		.font_size = 5,
		.min_width = 50,
		.max_width = 300,
		.position = {
			.x = 1060,
			.y = 370
		},
		.font_color = 0x000000ff,
		.border_color = 0x000000ff,
		.value = "",
		.allowed_symbols = "0123456789",

		.type_handler = FieldTypeHandler_y
	};

	struct RDUIOptionsBoxData options_box_ydata = {
		.position = {
			.x = 1165,
			.y = 370
		},
		.color = 0x555555ff,
		.font_color = 0xffffffff,
		.font_size = 4,
		.padding = 0,
		.options = options,

		.select_handler = OptionsBoxSelectHandler_y
	};

    struct RDUIButtonData button_data = {
		.text = "Calculate",
		.font_size = 10,
		.padding = 10,
		.color = 0x333333ff,
		.font_color = 0xffffffff,
		.position = {
			.x = 25,
			.y = 420
		},

		.clicked_handler = ButtonClickedHandler
	};




    struct RDUINode* field_x = RDUINewField(&field_data_x);
	struct RDUINode* options_boxx = RDUINewOptionsBox(&options_box_xdata);
    struct RDUINode* field_y = RDUINewField(&field_data_y);
	struct RDUINode* options_boxy = RDUINewOptionsBox(&options_box_ydata);
    struct RDUINode* button = RDUINewButton(&button_data);


    menu = RDUINewMenu(5,field_x, options_boxx, field_y, options_boxy, button);

    RDUIFocusedField = &field_data_x;


	while(1)
	{

        printf("\n\n\n\n\n%s\n\n%lf\n%lf\n%lf\n", &final_hypo, hypote, side_x_dec, side_y_dec  );
        //scanf("%s", str1);
        short w, h;
		CNFGClearFrame();
		CNFGHandleInput();
		CNFGGetDimensions( &w, &h );

		//Change color to white.
		//CNFGColor( 0xFFFFFF );


        // Red Color Select
		CNFGColor( 0x001440FF );

		//Draw a triangle
		RDPoint points[3] = { { triABX, triAY }, { triABX, triBCY }, { triCX, triBCY } };
		CNFGTackPoly( points, 3 );

        //Black Color Select
		CNFGColor( 0x000000FF );

		//Draw Black Box
		CNFGTackRectangle( 216, 300, 316, 400 );

        //Dark Red Color Select
		CNFGColor( 0x800000FF );

		//Draw Red Rec
		CNFGTackRectangle( 500, 600, 600, 700 );

        RDUIDispatchEvent(menu, RDUIEvent_render, NULL);

        CNFGPenX = 5; CNFGPenY = 5;
		CNFGDrawText( final_hypo, 20 );

        CNFGSwapBuffers();
	}
    return 0;
}
