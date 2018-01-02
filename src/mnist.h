/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.
 Simple CNN Application for Inference only
==================================================================
*/

#define SECURE_BUFFER			0x80100000
#define MNIST_WORKSPACE_BASE	0x60000

// keras_lay[0]
// biases{Array[16]}, weights{Array[5][5][1][16]}
//  biases  S:0x80100000 - S:0x80100040
//  weights S:0x80100040 - S:0x80100680
#define KERASLAYER0_BIASES		(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x0)
#define KERASLAYER0_WEIGHTS		(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x40)
// keras_lay[2]
// biases{Array[32]}, weights{Array[5][5][16][32]}
//  biases  S:0x80100680 - S:0x80100700
//  weights S:0x80100700 - S:0x8010cf00
#define KERASLAYER2_BIASES 		(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x680)
#define KERASLAYER2_WEIGHTS 	(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x700)
// keras_lay[6]
// biases{Array[128]}, weights{Array[512][128]}
//  biases  S:0x8010cf00 - S:0x8010d100
//  weights S:0x8010d100 - S:0x8014d100
#define KERASLAYER6_BIASES 		(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0xcf00)
#define KERASLAYER6_WEIGHTS 	(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0xd100)
// keras_lay[8]
// biases{Array[10]}, weights{Array[128][10]}
//  biases  S:0x8014d100 - S:0x8014d128
//  weights S:0x8014d128 - S:0x8014e528
#define KERASLAYER8_BIASES		(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x4d100)
#define KERASLAYER8_WEIGHTS 	(MNIST_EVAL_BASE + MNIST_PARAMETER_BASE + 0x4d128)


// MNIST image[image_num][IMAGE_ROWS][IMAGE_COLUMNS]
typedef struct {
    unsigned int input_channel, input_rows, input_columns;
    unsigned int filter_rows, filter_columns;
    unsigned int output_channel, output_rows, output_columns;
    char relu_activation;
} layer_structure;

int mnist_cnn_eval(
		unsigned int *test,
		unsigned long idx,
		unsigned int *result
);
