; Labels in an absolute AREA need to be able to decay to integer types.

	ORG &a000
	AREA Data, DATA

	DCD &00
Lbl1	DCD &10
	DCD &20
Lbl3	DCD &30

	ASSERT Lbl1 = &a004 :LAND: Lbl3 = &a00c
	ASSERT Lbl1 < Lbl3
	ASSERT Lbl1 <= Lbl3
	ASSERT Lbl3 > Lbl1
	ASSERT Lbl3 >= Lbl1
	ASSERT Lbl1 != &8000

	ASSERT Lbl1*2 = &14008
	ASSERT Lbl1/2 = &5002
	ASSERT Lbl1 :MOD: 0x10 = 4
	ASSERT Lbl1 + 4 = &a008
	ASSERT Lbl1 - 4 = &a000
	ASSERT Lbl1 & &F = 4
	ASSERT Lbl1 | 3 = &a007
	ASSERT Lbl1 ^ 4 = &a000
	ASSERT Lbl1 >>> 1 = &5002
	ASSERT Lbl1 >> 1 = &5002
	ASSERT Lbl1 << 1 = &14008
	ASSERT Lbl1 :ROR: 1 = &5002
	ASSERT Lbl1 :ROL: 1 = &14008

	ASSERT ~Lbl1 = &FFFF5FFB
	ASSERT -Lbl1 = &FFFF5FFC
	ASSERT +Lbl1 = &a004
	ASSERT :INDEX:Lbl1 = &a004

	END
