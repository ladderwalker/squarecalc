//Make it so we don't need to include any other C files in our build.
#define CNFG_IMPLEMENTATION
#define RDUI_IMPLEMENTATION
#define ONLYINPUT_IMPLEMENTATION

//Optional: Use OpenGL to do rendering on appropriate systems.
#define CNFGOGL

#define HAS_XSHAPE
#define HAS_XINERAMA

#include <stdio.h>
#include <string.h>
#include "rawdraw/CNFG.h"
#include "rdui/RDUI.h"
#include "rdui/default-elements.h"
#include "onlyinput/onlyinput.h"

#include "rdui/util.h"

static struct RDUIMenu* menu;


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

void ButtonSecondClickedHandle(struct RDUIButtonData* data) {
	exit(0);
}

void ButtonClickedHandlerX(struct RDUIButtonData* data) {
	data->text = "Not again, please!";
	data->clicked_handler = ButtonSecondClickedHandle;
}

void ButtonClickedHandlerY(struct RDUIButtonData* data) {
	data->text = "Not again, please!";
	data->clicked_handler = ButtonSecondClickedHandle;
}


void OptionsBoxSelectHandler(struct RDUIOptionsBoxData* data) {

}

void FieldTypeHandler(struct RDUIFieldData* data) {
	if(!strcmp(data->value, "demo")) UtilStringChange(data->value, "example");
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

    struct RDUIButtonData button_data_x = {
		.text = "X",
		.font_size = 10,
		.padding = 10,
		.color = 0x333333ff,
		.font_color = 0xffffffff,
		.position = {
			.x = 243,
			.y = 310
		},

		.clicked_handler = ButtonClickedHandlerX
	};

    struct RDUIButtonData button_data_y = {
		.text = "Y",
		.font_size = 10,
		.padding = 10,
		.color = 0x333333ff,
		.font_color = 0xffffffff,
		.position = {
			.x = 530,
			.y = 610

		},

		.clicked_handler = ButtonClickedHandlerY
	};

    struct RDUINode* buttonX = RDUINewButton(&button_data_x);
    struct RDUINode* buttonY = RDUINewButton(&button_data_y);

    menu = RDUINewMenu(2, buttonX, buttonY);


	while(1)
	{
		short w, h;
		CNFGClearFrame();
		CNFGHandleInput();
		CNFGGetDimensions( &w, &h );

		//Change color to white.
		CNFGColor( 0xffffffff );



		CNFGPenX = 5; CNFGPenY = 5;
		CNFGDrawText( "SquareCalc", 20 );


        // Red Color Select
		CNFGColor( 0x001440FF );

		//Draw a triangle
		RDPoint points[3] = { { triABX, triAY}, {triABX, triBCY}, { triCX, triBCY} };
		CNFGTackPoly( points, 3 );

        //Black Color Select
		CNFGColor( 0x000000FF );

		//Draw 100x100 box starting at 100,50
		CNFGTackRectangle( 216, 300, 315, 400 );

        //Dark Red Color Select
		CNFGColor( 0x800000FF );

		//Draw 100x100 box starting at 100,50
		CNFGTackRectangle( 500, 600, 600, 700 );

        RDUIDispatchEvent(menu, RDUIEvent_render, NULL);

        CNFGSwapBuffers();
	}
}
