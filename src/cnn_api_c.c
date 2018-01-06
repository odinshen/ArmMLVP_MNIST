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

#define CHECK_VALUE 100
#define DEBUG while(1)

#ifdef CNN_CONV_3
static float *con_filter_inputs  = 0;
static float *con_filter_outputs  = 0;
static unsigned int con_filter_input_channel = 0;
static unsigned int con_filter_intput_columns = 0;
static unsigned int con_filter_output_channel = 0;
static unsigned int con_filter_output_columns = 0;
static unsigned int con_filter_filter_rows = 0;
static unsigned int con_filter_filter_cols = 0;
static float *con_filter_weights = 0;
static float *con_filter_biases = 0;
static char  con_filter_relu_activation = 0;
#endif

float relu(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }
    return value;
}

#ifdef CNN_CONV_2
int convolution_filter2(
    float *inputs,
    float *outputs,
    unsigned int input_channel,
    unsigned int intput_columns,
    unsigned int output_channel,
    unsigned int output_columns,
    unsigned int filter_rows,
	unsigned int filter_cols,
    unsigned int stride_row,
	unsigned int stride_col,
	float *weights,
    float *biases,
	char  relu_activation
) {
	//	*inputs
	//	*outputs
	//	input_channel
	//  intput_columns
	//	output_channel
	//  output_columns
	//	filter_row
	//	filter_col
	//	stride_row
	//	stride_col
	//	*weights,
	//	*biases,

	unsigned int out_ch;
    unsigned int in_ch;
    unsigned int current_filter_row;
    unsigned int current_filter_col;
    float current_weight;
    float current_biase;
    float current_input;
    float current_result;
    float kernel_result;
    unsigned int kernel_output_addr;

    for (current_filter_row = 0; current_filter_row < filter_rows; current_filter_row++) {
        for (current_filter_col = 0; current_filter_col < filter_cols; current_filter_col++) {
            for (in_ch = 0; in_ch < input_channel; in_ch++) {
                current_input = ((float*)inputs)[  ((stride_row + current_filter_row) * intput_columns * input_channel)
                                                 + ((stride_col + current_filter_col) * input_channel)
                                                 + in_ch];
                for (out_ch = 0; out_ch < output_channel; out_ch++) {
                    current_weight = ((float*)weights)[  (current_filter_row * filter_cols * input_channel * output_channel)
                                                       + (current_filter_col * input_channel * output_channel)
                                                       + (in_ch              * output_channel)
                                                       + out_ch];
                    current_result = current_input * current_weight;
                    ((float*)outputs)[  (stride_row * output_columns * output_channel)
                                      + (stride_col * output_channel)
                                      + out_ch]
                    += current_result;
                }

            }
        }
    }

    for (out_ch = 0; out_ch < output_channel; out_ch++) {
        current_biase = ((float*)biases)[out_ch];
        kernel_output_addr = (stride_row * output_columns * output_channel) + (stride_col * output_channel) + out_ch;
        kernel_result = ((float*)outputs)[kernel_output_addr];
        kernel_result += current_biase;
        if (relu_activation) {
        	kernel_result = relu(kernel_result);
        }
    	((float*)outputs)[kernel_output_addr] = kernel_result;
    }

    return 0;
}
#endif

#ifdef CNN_CONV_3

#define CONV3_Eng_X  ((volatile unsigned char *) (0x800FFFE0))
#define CONV3_Eng_Y  ((volatile unsigned char *) (0x800FFFE4))
#define CONV3_Eng_Z  ((volatile unsigned char *) (0x800FFFE8))

int convolution_filter3(
    unsigned int stride_row,
	unsigned int stride_col
) {
	//	stride_row
	//	stride_col
#if 1
    float kernel_result;
    unsigned int current_filter_row;
    unsigned int current_filter_col;

    for (current_filter_row = 0; current_filter_row < con_filter_filter_rows; current_filter_row++) {
        for (current_filter_col = 0; current_filter_col < con_filter_filter_cols; current_filter_col++) {
        	*CONV3_Eng_X = stride_row;
        	*CONV3_Eng_Y = stride_col;
        	kernel_result = stride_row * stride_col;
        	*CONV3_Eng_Z = kernel_result;
        	kernel_result = *CONV3_Eng_Z;
        }
    }
    return kernel_result;
#else
	unsigned int out_ch;
    unsigned int in_ch;
    unsigned int current_filter_row;
    unsigned int current_filter_col;
    float current_weight;
    float current_biase;
    float current_input;
    float current_result;
    float kernel_result;
    unsigned int kernel_output_addr;

    for (current_filter_row = 0; current_filter_row < con_filter_filter_rows; current_filter_row++) {
        for (current_filter_col = 0; current_filter_col < con_filter_filter_cols; current_filter_col++) {
            for (in_ch = 0; in_ch < con_filter_input_channel; in_ch++) {
                current_input = ((float*)con_filter_inputs)[  ((stride_row + current_filter_row) * con_filter_intput_columns * con_filter_input_channel)
                                                 + ((stride_col + current_filter_col) * con_filter_input_channel)
                                                 + in_ch];
                for (out_ch = 0; out_ch < con_filter_output_channel; out_ch++) {
                    current_weight = ((float*)con_filter_weights)[  (current_filter_row * con_filter_filter_cols * con_filter_input_channel * con_filter_output_channel)
                                                       + (current_filter_col * con_filter_input_channel * con_filter_output_channel)
                                                       + (in_ch              * con_filter_output_channel)
                                                       + out_ch];
                    current_result = current_input * current_weight;
                    ((float*)con_filter_outputs)[  (stride_row * con_filter_output_columns * con_filter_output_channel)
                                      + (stride_col * con_filter_output_channel)
                                      + out_ch]
                    += current_result;
                }

            }
        }
    }

    for (out_ch = 0; out_ch < con_filter_output_channel; out_ch++) {
        current_biase = ((float*)con_filter_biases)[out_ch];
        kernel_output_addr = (stride_row * con_filter_output_columns * con_filter_output_channel) + (stride_col * con_filter_output_channel) + out_ch;
        kernel_result = ((float*)con_filter_outputs)[kernel_output_addr];
        kernel_result += current_biase;
        if (con_filter_relu_activation) {
        	kernel_result = relu(kernel_result);
        }
    	((float*)con_filter_outputs)[kernel_output_addr] = kernel_result;
    }
    return 0;
#endif
}
#endif

int convolution_conv2(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
) {
    unsigned int out_ch;
    unsigned int in_ch;
    unsigned int stride_row;
    unsigned int stride_col;
    unsigned int filter_row;
    unsigned int filter_col;
    float current_weight;
    float current_biase;
    float current_input;
    float current_result;
    float kernel_result;

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {
            for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {
                ((float*)outputs)[((stride_row * lay->output_columns * lay->output_channel)
                                                +(stride_col * lay->output_channel)
                                                + out_ch)]
                = 0.0f;
            }
        }
    }

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {

        	//	*inputs
        	//	*outputs
        	//	input_channel
        	//  intput_columns
        	//	output_channel
        	//  output_columns
        	//	filter_rows
        	//	filter_cols
        	//	stride_row
        	//	stride_col
        	//	*weights,
        	//	*biases,
        	convolution_filter2(
    			(float*)inputs,
    			(float*)outputs,
				lay->input_channel,
				lay->input_columns,
				lay->output_channel,
				lay->output_columns,
        		lay->filter_rows,
				lay->filter_columns,
				stride_row,
				stride_col,
				(float*)weights,
				(float*)biases,
				lay->relu_activation
        	);
        }
    }

    return 0;
}

int convolution_conv3(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
) {
    unsigned int out_ch;
    unsigned int in_ch;
    unsigned int stride_row;
    unsigned int stride_col;
    unsigned int filter_row;
    unsigned int filter_col;
    float current_weight;
    float current_biase;
    float current_input;
    float current_result;
    float kernel_result;

#ifdef CNN_CONV_3
    con_filter_inputs  = (float*)inputs;
    con_filter_outputs  = (float*)outputs;
    con_filter_input_channel = lay->input_channel;
    con_filter_intput_columns = lay->input_channel;
    con_filter_output_channel = lay->output_channel;
    con_filter_output_columns = lay->output_columns;
    con_filter_filter_rows = lay->filter_rows;
    con_filter_filter_cols = lay->filter_columns;
    con_filter_weights = (float*)weights;
    con_filter_biases = (float*)biases;
    con_filter_relu_activation = lay->relu_activation;
#endif

/*
    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {
            for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {
                ((float*)outputs)[((stride_row * lay->output_columns * lay->output_channel)
                                                +(stride_col * lay->output_channel)
                                                + out_ch)]
                = 0.0f;
            }
        }
    }
*/

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {

#ifdef CNN_CONV_3
        	//	stride_row
        	//	stride_col
        	convolution_filter3(
				stride_row,
				stride_col
        	);
#endif

        }
    }

    return 0;
}

int convolution(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
) {
    unsigned int out_ch;
    unsigned int in_ch;
    unsigned int stride_row;
    unsigned int stride_col;
    unsigned int filter_row;
    unsigned int filter_col;
    float current_weight;
    float current_biase;
    float current_input;
    float current_result;
    float kernel_result;

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {
            for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {
                ((float*)outputs)[((stride_row * lay->output_columns * lay->output_channel)
                                                +(stride_col * lay->output_channel)
                                                + out_ch)]
                = 0.0f;
            }
        }
    }

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {
            for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {
                for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {
                    for (in_ch = 0; in_ch < lay->input_channel; in_ch++) {
                        current_input = ((float*)inputs)[  ((stride_row + filter_row) * lay->input_columns * lay->input_channel)
                                                         + ((stride_col + filter_col)                      * lay->input_channel)
                                                         + in_ch];
                        for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {
                            current_weight = ((float*)weights)[  (filter_row * lay->filter_columns * lay->input_channel * lay->output_channel)
                                                               + (filter_col                       * lay->input_channel * lay->output_channel)
                                                               + (in_ch                                                 * lay->output_channel)
                                                               + out_ch];
                            current_result = current_input * current_weight;
                            ((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)
                                              + (stride_col                       * lay->output_channel)
                                              + out_ch]
                            += current_result;
                        }
                    }
                }
            }
        }
    }

    for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {
        for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {
            for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {
                current_biase = ((float*)biases)[out_ch];
                ((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)    // [x][ ][ ]
                                  + (stride_col                       * lay->output_channel)    // [ ][x][ ]
                                  + out_ch]                                                       // [ ][ ][x]
                += current_biase;

                if (lay->relu_activation == 1) {
                    kernel_result = ((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)    // [x][ ][ ]
                                                      + (stride_col                       * lay->output_channel)    // [ ][x][ ]
                                                      + out_ch];                                                       // [ ][ ][x]
                    ((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)    // [x][ ][ ]
                                      + (stride_col                       * lay->output_channel)    // [ ][x][ ]
                                      + out_ch]                                                       // [ ][ ][x]
                    = relu(kernel_result);
                }
            }
        }
    }

    return 0;
}

int max_pooling(
    layer_structure *lay,
    float *inputs,
    float *outputs
) {
    unsigned int ch;
    unsigned int input_row;
    unsigned int input_col;
    unsigned int output_row;
    unsigned int output_col;
    unsigned int filter_row;
    unsigned int filter_col;
    float current_max;
    float current_value;

    for (ch = 0; ch < lay->input_channel; ch++) {
        output_row = 0;
        for (input_row = 0; input_row < lay->input_rows; input_row += lay->filter_rows) {
            output_col = 0;
            for (input_col = 0; input_col < lay->input_columns; input_col += lay->filter_columns) {

                for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {
                    for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {
                        current_value = ((float*)inputs)[((input_row + filter_row) * lay->input_columns * lay->input_channel)
                                                        + ((input_col + filter_col) * lay->input_channel)
                                                        + ch];
                        if (filter_row == 0 && filter_col == 0) {
                            current_max = current_value;
                        }
                        else if (current_max < current_value) {
                            current_max = current_value;
                        }
                    }
                }

                ((float*)outputs)[(output_row * lay->output_columns * lay->output_channel)
                                  + (output_col * lay->output_channel)
                                  + ch] = current_max;

                output_col++;
            }
            output_row++;
        }
    }

    return 0;
}

// keras_lay[6]
// (Channel:512)
// (Channel:128)
// (Activation:ReLU, Channel:128)
// (Array[input_channel:512][output_channel:128]), (Array[output_channel:128])
//
// keras_lay[8]
// (Channel:128)
// (Channel:10)
// (Activation:Softmax, Channel:10)
// (Array[input_channel:128][output_channel:10]), (Array[output_channel:10])
//
int fully_connected(
    layer_structure *lay,
    float *inputs,    // inputs[lay->input_channel]
    float *outputs,   // outputs[lay->output_channel]
    float *weights,   // weights[lay->filter_rows][lay->filter_columns]
    float *biases     // biases[lay->output_channnel]
) {
    unsigned int o;
    unsigned int i;
    float current_weight;
    float current_biase;
    float current_input;
    float current_out;

    for (o = 0; o < lay->output_channel; o++) {    // (keras_lay[6]=128, keras_lay[8]=10)
        current_biase = ((float*)biases)[o];
        current_out = 0.0f;
        for (i = 0; i < lay->input_channel; i++) {    // (keras_lay[6]=512, keras_lay[8]=128)
            current_input = ((float*)inputs)[i];
            current_weight = ((float*)weights)[(i * lay->output_channel) + o];
            current_out += (current_input * current_weight);
        }
        current_out +=  current_biase;
        if (lay->relu_activation == 1) {
            current_out = relu(current_out);
        }
        ((float*)outputs)[o] = current_out;
    }

    return 0;
}

int mnist_pre_proc(
    unsigned int *test_images,    // test_images[IMAGE_ROWS][IMAGE_COLUMNS]
    float *outputs                // output[IMAGE_ROWS][IMAGE_COLUMNS]
) {
    unsigned int current_input;
    float normalized;
    unsigned int row, col;

    for (row = 0; row < MNIST_IMAGE_ROWS; row++) {
        for (col = 0; col < MNIST_IMAGE_COLUMNS; col++) {
            current_input = ((unsigned int*)test_images)[row * MNIST_IMAGE_COLUMNS + col];
            normalized = (float)current_input / 255.0;
            ((float*)outputs)[(row * MNIST_IMAGE_COLUMNS + col)] = normalized;
        }
    }

    return 0;
}

int post_proc(
    float *outlay,
    unsigned int channel
) {
    unsigned int idx, idx_max, idx_max_2nd;
    float conf, conf_max, conf_max_2nd, average, std, stdiv;

	printf("    prob [");

    idx_max = 0;
    conf_max = 0.0f;
    idx_max_2nd = 11;
    conf_max_2nd = 5;

    average = 0.0f;
    stdiv = 0.0f;
    std = 0.0f;

    for( idx = 0; idx < channel; idx++ ){
        conf = ((float*)outlay)[idx] /0x1000000;
    	printf(" %.0f,", conf);
        average = average + conf;
    	if(conf_max < conf){
            idx_max = idx;
            conf_max = conf;
        }
    }

    average /= 10;
	// Standard Deviation
    for( idx = 0; idx < channel; idx++ ){
        conf = ((float*)outlay)[idx] /0x1000000;
        if (
        	(idx != idx_max) &&
			(conf_max_2nd > (conf_max-conf))
		) {
        	conf_max_2nd = conf_max-conf;
        	idx_max_2nd =idx;
        }
        if (conf > average) {
        	std = conf-average;
        	std *= std;
        	stdiv += std;
        }
    }
    stdiv /= 10;
	stdiv = sqrt(stdiv);
	printf("], avg/std: %.0f/%.0f\n", average, stdiv);

	if ( conf_max_2nd < 3 ) {
		printf("        %d maybe another answer!!! \n", idx_max_2nd);
		printf("        Try to inference [%d] & [%d] again\n", idx_max, idx_max_2nd);
	}

    return idx_max;
}
