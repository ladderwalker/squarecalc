#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define CNFG_IMPLEMENTATION
#define _CNFG_FANCYFONT

#include "rawdraw_sf.h"
#define CNFGOGL
#define HAS_XSHAPE
#define HAS_XINERAMA


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
char* options[] = {"None", "1/16", "1/8", "3/16", "1/4", "5/16", "3/8", "7/16", "1/2",
"9/16", "5/8", "11/16", "3/4", "13/16", "7/8", "15/16", NULL};
char final_hypo[50];
int tape_measure_LUT[10000];

void poulateTapeMeasureLUT() {
	for(int i = 0; i < 310; i++) {
		tape_measure_LUT[i] = 0;
	}
	for(int i = 311; i < 925; i++) {
		tape_measure_LUT[i] = 1;
	}
	for(int i = 926; i < 1560; i++) {
		tape_measure_LUT[i] = 2;
	}
	for(int i = 1561; i < 2180; i++) {
		tape_measure_LUT[i] = 3;
	}
	for(int i = 2181; i < 2810; i++) {
		tape_measure_LUT[i] = 4;
	}
	for(int i = 2811; i < 3430; i++) {
		tape_measure_LUT[i] = 5;
	}
	for(int i = 3431; i < 4060; i++) {
		tape_measure_LUT[i] = 6;
	}
	for(int i = 4061; i < 4680; i++) {
		tape_measure_LUT[i] = 7;
	}
	for(int i = 4681; i < 5310; i++) {
		tape_measure_LUT[i] = 8;
	}
	for(int i = 5311; i < 5930; i++) {
		tape_measure_LUT[i] = 9;
	}
	for(int i = 5931; i < 6560; i++) {
		tape_measure_LUT[i] = 10;
	}
	for(int i = 6561; i < 7180; i++) {
		tape_measure_LUT[i] = 11;
	}
	for(int i = 7181; i < 7810; i++) {
		tape_measure_LUT[i] = 12;
	}
	for(int i = 7811; i < 8430; i++) {
		tape_measure_LUT[i] = 13;
	}
	for(int i = 8431; i < 9060; i++) {
		tape_measure_LUT[i] = 14;
	}
	for(int i = 9061; i < 9700; i++) {
		tape_measure_LUT[i] = 15;
	}
}

int getFraction(double in) {
	double fractpart, fractfloor, intpart;
	int fractfloorint, result;

	fractpart = modf(in, &intpart);
	fractfloor = floor(fractpart * 10000);
	fractfloorint = (int)fractfloor;

	if(fractfloorint > 9700) {
		result = 16;
	} else {
		result = tape_measure_LUT[fractfloorint];
	}

	return result;
}

void HandleKey( int keycode, int bDown ) {
    OIHandleKey(keycode, bDown);
	RDUIHandleKeyImpl(menu, keycode, bDown);
    if( keycode == CNFG_KEY_ESCAPE ) {
        exit( 0 );
    }


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
    double adjacent, opposite;
    adjacent = side_x + side_x_dec;
    opposite = side_y + side_y_dec;
    hypote = sqrt(( adjacent * adjacent ) + ( opposite * opposite ));

    //convert to string
    int hfrc;
    hfrc = getFraction(hypote);

    int hypint = (int)floor(hypote);

    if(hfrc == 0) {
        sprintf(final_hypo, "%d inches", hypint);
    } else if(hfrc == 16) {
    	sprintf(final_hypo, "%d inches", hypint + 1);
    } else {
        sprintf(final_hypo, "%d %s inches", hypint, options[hfrc]);
    }

    //clear the variables
    hypote = 0;


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
    poulateTapeMeasureLUT();

    // Load image
	int width, height, channels;
	unsigned char *img = stbi_load("resources/calcbut.png", &width, &height, &channels, 0);
	if(img == NULL)
	{
		printf("image failed to load\n");
		exit(1);
	}

    CNFGBGColor = 0x4E545Fff; //Dark Grey Background

    int winWidth = 1440, winHeight = 777;


    int triABX = 412, triAY = 200, triBCY = 500, triCX = 812;
    int counter = 0;
    int dr = 4;

    //CNFGSetupFullscreen( "SquareCalc", 0 );
    CNFGSetup( "SquareCalc", winWidth, winHeight );



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
		.color = 0x18191bff,
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
			.x = 1165,
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
			.x = 1270,
			.y = 370
		},
		.color = 0x18191bff,
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
			.y = 600
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
        short w, h;
		CNFGClearFrame();
		CNFGHandleInput();
		CNFGGetDimensions( &w, &h );

		//Change color to Black.
		CNFGColor( 0x000000FF );

        //Draw a triangle
		RDPoint pointsblk[3] = { { triABX - 5, triAY + 5 }, { triABX - 5, triBCY + 5 }, { triCX - 5, triBCY + 5 } };
		CNFGTackPoly( pointsblk, 3 );

        // Blue Color Select
		CNFGColor( 0x001440FF );

		//Draw a triangle
		RDPoint points[3] = { { triABX, triAY }, { triABX, triBCY }, { triCX, triBCY } };
		CNFGTackPoly( points, 3 );

        //Black Color Select
		CNFGColor( 0x000000FF );

		//Draw Black Box
		CNFGTackRectangle( 211, 305, 311, 405 );

        //Tan? Color Select
		CNFGColor( 0x666862FF );

		//Draw Tan? Box
		CNFGTackRectangle( 216, 300, 316, 400 );


        //Black Color Select
		CNFGColor( 0x000000FF );

		//Draw Black Box
		CNFGTackRectangle( 495, 605, 595, 705 );


        //Dark Red Color Select
		CNFGColor( 0x800000FF );

		//Draw Red Rec
		CNFGTackRectangle( 500, 600, 600, 700 );

        //Black Color Select
		CNFGColor( 0x000000FF );

		//Draw Black Box
		CNFGTackRectangle( 20, 605, 320, 693 );

		//Draw Black Box
		CNFGTackRectangle( 790, 360, 1075, 430 );

        //Draw Black Box
		CNFGTackRectangle( 1135, 360, 1420, 430 );

        //Tan? Color Select
		CNFGColor( 0x666862FF );

		//Draw Tan? Box
		CNFGTackRectangle( 795, 355, 1080, 425 );

        //Dark Red Color Select
		CNFGColor( 0x800000FF );

		//Draw Red Rec
		CNFGTackRectangle( 1140, 355, 1425, 425 );

        CNFGColor( 0xffffffff );

        CNFGPenX = 995; CNFGPenY = 375;
		CNFGDrawNiceText( "inches", 3 );

        CNFGPenX = 1352; CNFGPenY = 375;
		CNFGDrawNiceText( "inches", 3 );

        RDUIDispatchEvent(menu, RDUIEvent_render, NULL);

        CNFGPenX = 5; CNFGPenY = 5;
		CNFGDrawNiceText( final_hypo, 16 );

        CNFGBlitImage((unsigned int*)img, 25, 600, width, height);

        CNFGSwapBuffers();
        usleep(10000);
	}

	free(xvalue);
    free(yvalue);
   	free(options);
   	free(menu);
   	free(field_x);
   	free(field_y);
	free(button);
	stbi_image_free(img);

   	return 0;
}
