/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.
 Cortex-A v8 CNN Application demo
==================================================================
*/

// mnist

#define MNIST_IMAGE_ROWS		28
#define MNIST_IMAGE_COLUMNS	28

#define MNIST_EVAL_BASE			0x80100000   // CA55/CA53_CA73
#define MNIST_PARAMETER_BASE	0x0
#define MNIST_TESTIMAGE_BASE	0x50000   // CA55/CA53_CA73
#define MNIST_WORKSPACE_BASE	0x60000

#define CIFAR_EVAL_BASE			0x80100000   // CA55/CA53_CA73
#define CIFAR_PARAMETER_BASE	0x0
#define CIFAR_TESTIMAGE_BASE	0x50000
#define CIFAR_WORKSPACE_BASE	0x60000


#define HOST_CONFIG_AUTO_BASE        0x800FFFFF // CA55/CA53_CA73
#define HOST_CONFIG_CNN_BASE         0x800FFFFB // CA55/CA53_CA73

#define TESTMODE_AUTO  0
#define TESTMODE_IMAGE 1

#define TESTMODE_AUTO  0
#define TESTMODE_IMAGE 1

#define TESTMODE_IMAGE_NUM 6

#define TESTMODEL_MNIST  0
#define TESTMODEL_CIFAR 1

#define TEST_IMAGE_0 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x0)		// (size 0xC40)   7
#define TEST_IMAGE_1 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x1000)	// (size 0xC40)	  3
#define TEST_IMAGE_2 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x2000)	// (size 0xC40)   4
#define TEST_IMAGE_3 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x3000)	// (size 0xC40)   6
#define TEST_IMAGE_4 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x4000)	// (size 0xC40)   1
#define TEST_IMAGE_5 (MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x5000)	// (size 0xC40)   8

#define TEST_IMAGE_X(X) 	(MNIST_EVAL_BASE + MNIST_TESTIMAGE_BASE + 0x1000 * (X))		// (size 0xC40)
#define TEST_IMAGE_RES(X) 	((volatile unsigned char *) (TEST_IMAGE_X(X) + 0xFFF))

#define WORK_IMAGE_X(X) 	(MNIST_EVAL_BASE + MNIST_WORKSPACE_BASE + 0x18000 * (X))

//#define AUTOTESTIMG ((volatile unsigned char *) (MNIST_EVAL_BASE + 0xFFFFF))
//#define AUTOTESTIMG ((volatile unsigned char *) (MNIST_EVAL_BASE + 0x300FFFF))
#define AUTOTESTIMG  ((volatile unsigned char *) (HOST_CONFIG_AUTO_BASE))
#define CNNSELECTING ((volatile unsigned char *) (HOST_CONFIG_CNN_BASE))


//#define CNN_CONV_1     1    // Original
#define CNN_CONV_2     1	// API
//#define CNN_CONV_3     1	// API w/ static register
//#define CNN_CONV_4     1	// API w/ Engine
