
@{{BLOCK(thing)

@=======================================================================
@
@	thing, 64x32@4, 
@	+ palette 16 entries, not compressed
@	+ 32 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 1024 = 1056
@
@	Time-stamp: 2023-10-18, 19:03:52
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global thingTiles		@ 1024 unsigned chars
thingTiles:
	.word 0x00000000,0x75000000,0xDA500000,0xDD700000,0x22500000,0x00000000,0x11750000,0xDDDB0000
	.word 0x00000000,0x77777777,0xDDDDDDDD,0xDDDDDDDD,0xDDD62222,0xEB750000,0x0000005B,0x0000052D
	.word 0xDDD90000,0xDDD80000,0xDDD65000,0xDDDDB000,0xDDDD9000,0xDDDD8000,0xDDDDC000,0xDDDDA500
	.word 0x000007DD,0x0000056D,0x000000CD,0x000000ED,0x500000BD,0xAEB50056,0xDDD6C8AD,0xDDDDDDDD
	.word 0x00000000,0x57777777,0x6DDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDD2,0xDDDDDD95,0xDDDDD250
	.word 0x00000000,0x00000000,0x005C2222,0x004DDDDD,0x00CDDDDD,0x056DDDDD,0x01DDDDDD,0x08DDDDDD
	.word 0xDDDDD900,0xDDDDD400,0xADDDDA50,0x9DDDDD95,0x58DDDDDE,0x058DDDDD,0x0051ADDD,0x0000516D
	.word 0x5ADDDD4D,0x1DDDDA59,0x2DDDD105,0xDDDDA500,0xDDDD4000,0xDDDDB000,0xDDDA5000,0xDDD40000

	.word 0x00000000,0x00000000,0x50000000,0x45000000,0xD9500000,0xDDB00000,0xDDA50000,0xDDD85000
	.word 0x00000000,0x05777750,0x26DDDDAE,0xDDDDDDDD,0xDDDDDDDD,0xAA66DDDD,0xDDDDCCDD,0xDDDD15AD
	.word 0xDDDDE500,0x2DDDD950,0x5ADDDD15,0x0BDDDDD2,0x059DDDDD,0x005EDDDD,0x00054DDD,0x000058DD
	.word 0xDDDD405A,0xDDDD6505,0xDDDDD700,0xDDDDD700,0xDDDDDE00,0xDDDDD800,0xDDDDD650,0xDDDDDD70
	.word 0x00000000,0x57777775,0xADDDDDD6,0xDDDDDDDD,0xDDDDDDDD,0xDD6CC82A,0x215005AD,0x500005DD
	.word 0x00000000,0x00000000,0x00000514,0x005B86DD,0x054DDDDD,0x54DDDDDD,0x7DDDDDDD,0x4DDDDDD8
	.word 0x0000056D,0x0000004D,0x5000009D,0x450000BD,0xD215051D,0xDDD6844D,0xDDDDDDDD,0xDDDDDDDD
	.word 0xDDDDDDD1,0x4DDDDDD1,0x5ADDDDD2,0x01DDDDDD,0x058DDDDD,0x0058DDDD,0x00051ADD,0x0000051A

	.word 0xDDDDD700,0x9999B500,0x00000000,0x00000000,0x00000000,0x00000000,0xB5000000,0xDAC4B500
	.word 0xE8C26DDD,0x00005B19,0x00000000,0x00000000,0x00000000,0x77500000,0x6D22C849,0xDD0DDDDD
	.word 0xDDDDDA50,0xDDDDDD40,0xDDDDA950,0xC8175000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0xDD0DDDDD,0xC06DDDDD,0x0CDDDDDD,0xDDDDDD62,0x88441750,0x00000000,0x00000000,0x00000000
	.word 0x0000005B,0x00000000,0x00000000,0x00000000,0x00000000,0x4444E99B,0xDDDDDDDD,0xD0DDDD0D
	.word 0xDDDB0000,0xDDA50000,0x6D900000,0x54500000,0x51500000,0xADA88444,0xDDDDDDDD,0xDD000DDD
	.word 0xD0DDDD02,0xD0DDDDD0,0xD0DDDDDA,0xDDDDDDDD,0x622222C8,0x50000000,0x00000000,0x00000000
	.word 0x60490DDD,0xA0F80DDD,0xD2000DDD,0xDD626DDD,0xDDDDDDDD,0x9999999B,0x00000000,0x00000000

	.word 0x000005CD,0x0000005A,0x00000005,0x00000000,0x00000000,0x44444488,0xDDDDDDDD,0xDD00ADDD
	.word 0xDDDDDDB0,0x99991B50,0x00000000,0x00000000,0x00000000,0x005B9E44,0x89CDDDDD,0xDC00ADDD
	.word 0xDDD40DDD,0xDD000DDD,0xDDDC0DDD,0xDD00DDDD,0x6292DDDD,0x5005B999,0x00000000,0x00000000
	.word 0xC0E90CDD,0x0EF810DD,0xC01102DD,0xDC002DDD,0xDDDDD26D,0x00000057,0x00000000,0x00000000
	.word 0x71826DDD,0x000057B1,0x00000000,0x00000000,0x00000000,0x00000000,0x0571E488,0x8ADDDDDD
	.word 0x00000005,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000574
	.word 0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xBE882ADD,0x000005B9,0x00000000,0x00000000,0x00000000
	.word 0x00005CDD,0x0000EDDD,0x0000518A,0x00000005,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global thingPal		@ 32 unsigned chars
thingPal:
	.word 0x40CD0000,0x1C5F6955,0x0C235911,0x308A7978,0x48EF5D33,0x38AB7157,0x7D796534,0x65D55110

@}}BLOCK(thing)