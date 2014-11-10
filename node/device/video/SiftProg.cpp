//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include "ccv.h"
#include "SiftProg.h"
#include "progblobs.h"
#include "pixy_init.h"
#include "camera.h"
#include "conncomp.h"
#include "serial.h"
#include "rcservo.h"


Program g_progTLD =
{
	"sift",
	"perform sift algorithm",
	siftSetup, 
	siftLoop
};



int siftSetup()
{
	//CCV Initialization
	::ccv_enable_default_cache();



	//
	//Below is PixyCam Initialization
	//


	uint8_t c;

	// setup camera mode
	cam_setMode(CAM_MODE1);
 	
	// load lut if we've grabbed any frames lately
	if (g_rawFrame.m_pixels)
		cc_loadLut();

	// setup qqueue and M0
	g_qqueue->flush();
	exec_runM0(0);

	// flush serial receive queue
	while(ser_getSerial()->receive(&c, 1));

	return 0;
}

int siftLoop()
{
	ccv_dense_matrix_t* image  = 0;
	ccv_read((const void*)g_rawFrame.m_pixels, &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);

	ccv_sift_param_t params = {
		.noctaves = 3,
		.nlevels = 6,
		.up2x = 1,
		.edge_threshold = 10,
		.norm_threshold = 0,
		.peak_threshold = 0,
	};

	ccv_array_t* image_keypoints = 0;
	ccv_dense_matrix_t* image_desc = 0;
	ccv_sift(image, &image_keypoints, &image_desc, 0, params);

	//ccv_array_free(image_keypoints);
	//ccv_matrix_free(image_desc);
	//ccv_matrix_free(image);

	return 0;
}
