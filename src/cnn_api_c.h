/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/

float relu(float value);
int convolution(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
);
int convolution_conv2(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
);
int convolution_conv3(
    layer_structure *lay,
    float *inputs,
    float *outputs,
    float *weights,
    float *biases
);
int max_pooling(
    layer_structure *lay,
    float *inputs,
    float *outputs
);
int fully_connected(
    layer_structure *lay,
    float *inputs,    // inputs[lay->input_channel]
    float *outputs,   // outputs[lay->output_channel]
    float *weights,   // weights[lay->filter_rows][lay->filter_columns]
    float *biases     // biases[lay->output_channnel]
);
int pre_proc(
    unsigned int *test_images,    // test_images[IMAGE_ROWS][IMAGE_COLUMNS]
    float *outputs                // output[IMAGE_ROWS][IMAGE_COLUMNS]
);
int post_proc(
    float *outlay,
    unsigned int channel
);
