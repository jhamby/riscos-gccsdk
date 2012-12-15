
	COMMON cNoArg		; Size 0, alignment 4, STV_DEFAULT
	ASSERT ?cNoArg = 0

	COMMON cSize16 , 16
	ASSERT ?cSize16 = 16

	COMMON cAlign32 , 8  , 32
	ASSERT ?cAlign32 = 8

	COMMON cVis0, 0, 4    DYNAMIC
	COMMON cVis1, 0, 4 INTERNAL
	COMMON cVis2, 0, 4   HIDDEN
	COMMON cVis3, 0, 4  PROTECTED

	AREA	Data, DATA

	DCD	cNoArg, cSize16, cAlign32
	DCD	cVis0, cVis1, cVis2, cVis3

	END
