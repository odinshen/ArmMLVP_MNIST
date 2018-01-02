/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.
 Simple CNN Application for Inference only
==================================================================
*/

// #define CIFAR_EVAL_BASE			0x80100000

#define SECURE_BUFFER			0x80100000
#define CIFAR_WORKSPACE_BASE	0x60000

// keras_lay[0]
// biases{Array[32]}, weights{Array[3][3][3][32]}
//  biases  S:0x80100000 - S:0x80100080
//  weights S:0x80100080 - S:0x80100e00
#define CIFAR_KERASLAYER0_BIASES		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x0)
#define CIFAR_KERASLAYER0_WEIGHTS		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x80)
// keras_lay[3]
// biases{Array[64]}, weights{Array[3][3][32][64]}
//  biases  S:0x80100e00 - S:0x80100f00
//  weights S:0x80100f00 - S:0x80112f00
#define CIFAR_KERASLAYER3_BIASES 		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0xe00)
#define CIFAR_KERASLAYER3_WEIGHTS 		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0xf00)
// keras_lay[8]
// biases{Array[1024]}, weights{Array[4096][1024}
//  biases  S:0x80112f00 - S:0x80113f00
//  weights S:0x80113f00 - S:0x81113f00
#define CIFAR_KERASLAYER8_BIASES 		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x12f00)
#define CIFAR_KERASLAYER8_WEIGHTS 		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x13f00)
// keras_lay[8]
// biases{Array[10]}, weights{Array[1024][10]}
//  biases  S:0x81113f00 - S:0x81113f28
//  weights S:0x81113F28 - S:0x8111df28
#define CIFAR_KERASLAYER10_BIASES		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x1013f00)
#define CIFAR_KERASLAYER10_WEIGHTS 		(CIFAR_EVAL_BASE + CIFAR_PARAMETER_BASE + 0x1013f28)


// MNIST image[image_num][IMAGE_ROWS][IMAGE_COLUMNS]
typedef struct {
    unsigned int input_channel, input_rows, input_columns;
    unsigned int filter_rows, filter_columns;
    unsigned int output_channel, output_rows, output_columns;
    char relu_activation;
} layer_structure;

int cifar10_cnn_eval(
		unsigned int *test,
		unsigned long idx,
		unsigned int *result
);
