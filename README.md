# ArmV82_Keras_CNN_Inference
This is `ArmV82_Keras_CNN_Inference`, a Arm machine learning prototyping software project, optimized for Arm Cortex v8 CPU arch.

1. Based on Keras mnist training database
2. Will leverage Arm neon instruction set, http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0489e/CJAJIIGG.html
3. Will apply dot product
4. Executing CA73x2 + CA53x4 FVP
5. Enable DS5 profiling tools: streamline


Step:
1) Training model
2) Translate to C code
3) DS5 compiler
4) Simualtion:
	A) Debug script:
		Load training model into DDR (json file or binary)
		Load data into FVP DDR space
		Some data pre-operation
	B) Start simulation
	C) Display result


	
DDR:
	
Test memeory map:

	0x0			Parameter
		0x00  ~ 0040: Conv-1
		0x680 ~ 0700: Conv-2
		0xCF00 ~ D100: FullConn-1
		0xD100 ~ D128: FullConn-2
		
	0x50000 mnist test image
		0x50000 : image 0
		0x51000 : image 1
		0x52000 : image 2
		0x53000 : image 3
		0x54000 : image 4
		0x55000 : image 5

	0x60~0xEF	workspace
		0x60000 ~ 0x77000
		0x78000 ~ 0x8F000
		0x90000 ~ 0xA7000
		0xA8000 ~ 0xBF000
		0xC0000 ~ 0xD7000
		0xD8000 ~ 0xEF000

Workspace memory layout map: (a75_a55)


1) 0x80370000: Test data,
   0x80371000: Test data,
   0x80372000: Test data,
   0x80373000: Test data,
   0x80374000: Test data,
   0x80375000: Test data,
   
   
   work space
2) 0x80350000:	Normalized, 
				unsign int / 255 --> float			   
3) 0x80351000:	Conv-1,
				[28,28,1] --> [24,24,1] x16
				[5,5] filter
				16 biase
				Relu

4) 0x8035B000: MaxPool-1
				[24,24,1] x16 --> [12,12,1] x16
				[2,2] filter

5) 0x8035E000: Conv-2
				[12,12,1] x16 --> [8,8,1] x32
				[5,5] filter
				32 biase
				Relu

6) 0x80360000: MaxPool-2
				[8,8,1] x32 --> [4,4,1] x32
				[2,2] filter

7) 0x80361000: FullConn-1
				[512] --> [128]
				
8) 0x80361300: FullConn-2
				[128] --> [10]


API:
	PMU



ToDo:
	NEON
	Cifar-10
	

