/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/
#include <stdlib.h>
#include <stdio.h>
#include "arm_cnn_inference.h"
#include "mnist.h"
#include "cnn_api_c.h"

int mnist_cnn_eval(
    unsigned int *test_images,    // test_images[IMAGE_ROWS][IMAGE_COLUMNS]
	unsigned long idx,
    unsigned int *result
) {
    static layer_structure lay;

    unsigned int conv_mode;

//    unsigned int workspace_inout = MNIST_TEST_BASE + MNIST_WORKSPACE_BASE + 0x18000 * idx;
    unsigned int workspace_inout = WORK_IMAGE_X(idx);
    unsigned int workspace_layer1 = workspace_inout + 0x1000;
    unsigned int workspace_layer2 = workspace_inout + 0xB000;
    unsigned int workspace_layer3 = workspace_inout + 0xE000;
    unsigned int workspace_layer4 = workspace_inout + 0x10000;
    unsigned int workspace_layer5 = workspace_inout + 0x11000;
    unsigned int workspace_output = workspace_inout + 0x11300;

	conv_mode = *CONVMODE;
	if (!conv_mode) {
		conv_mode = 2;  // default mode
	}

    mnist_pre_proc(
        test_images,
	    (float*)workspace_inout
    );

    // keras_lay[0]
    lay.input_channel = 1;
    lay.input_rows = 28;
    lay.input_columns = 28;
    lay.filter_rows = 5;
    lay.filter_columns = 5;
    lay.output_channel = 16;
    lay.output_rows = 24;
    lay.output_columns = 24;
    lay.relu_activation = 1;    // Activation:ReLU
    if (conv_mode == 2) {
    	convolution_conv2(
    			&lay,
				(float*)workspace_inout,
				(float*)workspace_layer1,
				(float*)KERASLAYER0_WEIGHTS,
				(float*)KERASLAYER0_BIASES
    	);
    }
#ifdef CNN_CONV_3
    else if (conv_mode == 3) {
    	convolution_conv3(
    			&lay,
				(float*)workspace_inout,
				(float*)workspace_layer1,
				(float*)KERASLAYER0_WEIGHTS,
				(float*)KERASLAYER0_BIASES
    	);
    }
#endif
    else {
    	convolution(
    			&lay,
				(float*)workspace_inout,
				(float*)workspace_layer1,
				(float*)KERASLAYER0_WEIGHTS,
				(float*)KERASLAYER0_BIASES
    	);
    }

    // keras_lay[1]
    lay.input_channel = 16;
    lay.input_rows = 24;
    lay.input_columns = 24;
    lay.filter_rows = 2;
    lay.filter_columns = 2;
    lay.output_channel = 16;
    lay.output_rows = 12;
    lay.output_columns = 12;
    lay.relu_activation = 0;
    max_pooling(
        &lay,
        (float*)workspace_layer1,
        (float*)workspace_layer2
    );

    // keras_lay[2]
    lay.input_channel = 16;
    lay.input_rows = 12;
    lay.input_columns = 12;
    lay.filter_rows = 5;
    lay.filter_columns = 5;
    lay.output_channel = 32;
    lay.output_rows = 8;
    lay.output_columns = 8;
    lay.relu_activation = 1;    // Activation:ReLU
    if (conv_mode == 2) {
    	convolution_conv2(
    			&lay,
				(float*)workspace_layer2,
				(float*)workspace_layer3,
				(float*)KERASLAYER2_WEIGHTS,
				(float*)KERASLAYER2_BIASES
    	);
    }
#ifdef CNN_CONV_3
    else if (conv_mode == 3) {
    	convolution_conv3(
    			&lay,
				(float*)workspace_layer2,
				(float*)workspace_layer3,
				(float*)KERASLAYER2_WEIGHTS,
				(float*)KERASLAYER2_BIASES
    	);
    }
#endif
    else {
    	convolution(
    			&lay,
				(float*)workspace_layer2,
				(float*)workspace_layer3,
				(float*)KERASLAYER2_WEIGHTS,
				(float*)KERASLAYER2_BIASES
    	);
    }

    // keras_lay[3]
    lay.input_channel = 32;
    lay.input_rows = 8;
    lay.input_columns = 8;
    lay.filter_rows = 2;
    lay.filter_columns = 2;
    lay.output_channel = 32;
    lay.output_rows = 4;
    lay.output_columns = 4;
    lay.relu_activation = 0;
    max_pooling(
        &lay,
        (float*)workspace_layer3,
        (float*)workspace_layer4
    );

    // keras_lay[4]

    // keras_lay[5]

    // keras_lay[6]
    lay.input_channel = 512;
    lay.input_rows = 0;
    lay.input_columns = 0;
    lay.filter_rows = 0;
    lay.filter_columns = 0;
    lay.output_channel = 128;
    lay.output_rows = 0;
    lay.output_columns = 0;
    lay.relu_activation = 1;    // Activation:ReLU
    fully_connected(
        &lay,
        (float*)workspace_layer4,
        (float*)workspace_layer5,
        (float*)KERASLAYER6_WEIGHTS,
        (float*)KERASLAYER6_BIASES
    );

    // keras_lay[7]

    // keras_lay[8]
    lay.input_channel = 128;
    lay.input_rows = 0;
    lay.input_columns = 0;
    lay.filter_rows = 0;
    lay.filter_columns = 0;
    lay.output_channel = 10;
    lay.output_rows = 0;
    lay.output_columns = 0;
    lay.relu_activation = 0;
    fully_connected(
        &lay,
        (float*)workspace_layer5,
        (float*)workspace_output,
        (float*)KERASLAYER8_WEIGHTS,
        (float*)KERASLAYER8_BIASES
    );

    *result = post_proc((float*)workspace_output, lay.output_channel);

	printf("Conv_mode: %d", conv_mode);

    return 0;
}
