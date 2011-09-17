static const uint32_t crypt_buf[0x500] = {
	0x55c636e2, 0x02be0170, 0x584b71d4, 0x2984f00e, 0xb682c809, 0x91cf876b,
	0x775a9c24, 0x597d5ca5, 0x5a1afeb2, 0xd3e9ce0d, 0x32cdcdf8, 0xb18201cd,
	0x3cce05ce, 0xa55d13be, 0xbb0afe71, 0x9376ab33, 0x848f645e, 0x87e45a45,
	0x45b86017, 0x5e656ca8, 0x1b851a95, 0x2542dbd7, 0xab4df9e4, 0x5976ae9b,
	0x6c317e7d, 0xcddd2f94, 0x3c3c13e5, 0x335b1371, 0x31a592ca, 0x51e4fc4c,
	0xf7db5b2f, 0x8abdbe41, 0x8beaa674, 0x20d6b319, 0xde6c9a9d, 0xc5ac84e5,
	0x445a5feb, 0x94958cb0, 0x1e7d3847, 0xf35d29b0, 0xca5cceda, 0xb732c8b5,
	0xfdcc41dd, 0x0edcec16, 0x9d01feae, 0x1165d38e, 0x9ee193c8, 0xbf33b13c,
	0x61bc0dfc, 0xef3e7be9, 0xf8d4d4c5, 0xc79b7694, 0x5a255943, 0x0b3dd20a,
	0x9d1ab5a3, 0xcfa8ba57, 0x5e6d7069, 0xcb89b731, 0x3dc0d15b, 0x0d4d7e7e,
	0x97e37f2b, 0xfefc2bb1, 0xf95b16b5, 0x27a55b93, 0x45f22729, 0x4c986630,
	0x7c666862, 0x5fa40847, 0xa3f16205, 0x791b7764, 0x386b36d6, 0x6e6c3fef,
	0xc75855db, 0x4abc7dc7, 0x4a328f9b, 0xcef20c0f, 0x60b88f07, 0xf7bb4b8f,
	0x830b5192, 0x94f711ec, 0x20250752, 0x399d21a3, 0xe5c0840d, 0xe76cffa5,
	0x624fab29, 0x5df133e6, 0x83e0b9b8, 0xc5796bfb, 0x4a7ab2d0, 0xba59a821,
	0x03a81e4c, 0xcd3adfdb, 0x32b26b8c, 0x8e35c533, 0x9e6300e9, 0x8cf92ac5,
	0x880d18eb, 0x131a53b3, 0x2ed2dc64, 0xb23257c1, 0xa06450c1, 0x1b92cb8e,
	0x72ed730e, 0x19a685f0, 0x82836483, 0x42d94e8a, 0xee9bd6f6, 0x556d0b6a,
	0xba65589a, 0xde24cce4, 0x53329f6c, 0xc754fe8b, 0x503d2dc7, 0x10027ba4,
	0xd3b60a8b, 0x68e68d83, 0x0a9128a9, 0x595fa35f, 0x0b03b5be, 0x150a45c4,
	0xb1629cce, 0xe5f7497b, 0x8a7098a4, 0xb8233e69, 0x8ea0f978, 0x5b579970,
	0xeab14318, 0x4b28b263, 0xb6766cef, 0x06782877, 0x155c6dd0, 0xc711333c,
	0xf819cedf, 0x00eb1d68, 0xd6fffa6e, 0x439e5962, 0xd765d6db, 0xcb0bcee9,
	0x6d3c5647, 0x965466f3, 0x0ca983c9, 0x74ecc1ce, 0xfc0563b6, 0x42b08fee,
	0xc5b38853, 0xfe502ceb, 0x7b432faf, 0xc309e610, 0x2c3997d8, 0x43774654,
	0x15bd9d2c, 0xed6a420d, 0xc7ff520c, 0xb8a97fd1, 0x5e4d60cc, 0xb9738d11,
	0xda2181ff, 0x73ac2597, 0x3a8eec8d, 0xac85e779, 0xf3f975d6, 0xb9fe7b91,
	0x0f155d1e, 0x2860b6dd, 0x835977cb, 0xb0607436, 0x9cab7f6b, 0x8ab91186,
	0xc12b51e9, 0x20084e8b, 0x44ba8ead, 0xa542b130, 0x82bcd5c4, 0xcc747f4e,
	0x0f1909d8, 0xda242e1c, 0x6f7d1aa0, 0xd2626486, 0x88d0781e, 0xab695ccd,
	0xfa569145, 0xb4feb55c, 0xbe47e896, 0xe70a7a88, 0xd56185a2, 0xacf4c871,
	0x09282332, 0x1ddeeaa8, 0x590c7adb, 0xf4a97667, 0xbfd85705, 0x0ea77ccc,
	0xa9f85364, 0x83195869, 0x8bfb041a, 0xdb842f5c, 0xd6f0f315, 0xa7756ea7,
	0x0a51b439, 0xa9edf8a3, 0xd9084e2f, 0x827407f8, 0xd4ac8284, 0x09739d0d,
	0xb3bb6cfc, 0xd539c77d, 0x6bbc9ac0, 0x35c641aa, 0x934c96b0, 0xd17af317,
	0x29c6baef, 0xb275cdac, 0xd72662de, 0x9f5c2544, 0xc1a98f75, 0xd98e8f9a,
	0x47bd5c86, 0x70c610a6, 0xb5482ed4, 0x23b9c68c, 0x3c1bae66, 0x69556e7f,
	0xd902f5e0, 0x653d195b, 0xde6541fb, 0x07bcc6ac, 0xc6ee7788, 0x801534d4,
	0x2c1f35c0, 0xd9de614d, 0xbdccac85, 0xb4d4a0da, 0x242d549b, 0x9d964796,
	0xb9ceb982, 0x59fa99a9, 0xd8986cc1, 0x9e90c1a1, 0x01bbd82f, 0xd7f1c5fd,
	0xdd847eba, 0x883d305d, 0x25f13152, 0x4a92694d, 0x77f1e601, 0x8024e6e7,
	0x02a5f53d, 0x9c3ef4d9, 0xaf403ccc, 0xe2ad03c0, 0x46edf6ec, 0x6f9bd3e6,
	0xcc24ad7a, 0x47afab12, 0x82298df7, 0x708c9eec, 0x76f8c1b1, 0xb39459d2,
	0x3f1e26d9, 0xe1811be7, 0x56ed1c4d, 0xc9d18af8, 0xe828060e, 0x91cada2e,
	0x5ccbf9b7, 0xf1a552d4, 0x3c9d4343, 0xe1008785, 0x2adfeebf, 0xf90240a0,
	0x3d08cce7, 0x426e6fb0, 0x573c984f, 0x13a843ae, 0x406b7439, 0x636085d9,
	0x5000ba9a, 0xad4a47ab, 0xaf001d8d, 0x419907ae, 0x185c8f96, 0xe5e9ed4d,
	0x61764133, 0xd3703d97, 0xac98f0c6, 0xdbc3a37c, 0x85f010c4, 0x90491e32,
	0xf12e18bf, 0xc88c96e1, 0xd3fbd6d9, 0xe3c28b08, 0xd5bf08cc, 0xb1e78859,
	0x2546ddcf, 0xb030b200, 0xaafd2811, 0x55b22d21, 0xd38bf567, 0x469c7a2b,
	0x5ad05792, 0xa1a5981e, 0x7dfb8384, 0x34d1ca0a, 0x7eb0dbe0, 0xd61ce0f6,
	0x398068b7, 0xe6406d1f, 0x95ae6b47, 0xe4281230, 0xb0843061, 0xa70a3a68,
	0xe340f625, 0x72dcbffd, 0x8eb8afcd, 0x18b6661f, 0x17ef5a5c, 0x000c5b22,
	0x6ba13836, 0x6165e383, 0x74481c5b, 0xe56f0711, 0xa26f5024, 0x5ff22e60,
	0x31a5e829, 0xa1094bf0, 0xc680ec6c, 0x8cf327d7, 0xebf1348a, 0x6a227d2f,
	0x74065184, 0x8df65112, 0x2bbd05ee, 0xe4d00ed6, 0x2980ee1a, 0x6ae1da73,
	0xe84614da, 0x6c9906ab, 0xcf8e02db, 0xd3723e97, 0x92f66caf, 0xac8491c7,
	0xaec65696, 0xb98997cf, 0xfa16c762, 0x6d73c65f, 0x205d22a6, 0x4dd3aaa5,
	0x2deb6bc0, 0x9f37686c, 0x71a5282b, 0x376bb9e0, 0x7fff2a1b, 0xde67982f,
	0x9cbf33ce, 0x2e6dab37, 0x6e3424b9, 0x0ee143bc, 0x832a60d9, 0xbb6329e1,
	0x13f6befd, 0x5965fb84, 0xf60b233c, 0x3d695183, 0x433224a1, 0xb5d9cae5,
	0x82459bab, 0x9f21b311, 0xaf6c5247, 0xb447b13a, 0x7b2676c3, 0xc38979cd,
	0x8526ae25, 0xc550ad5b, 0x685099a7, 0x65e9c2bd, 0xe5c6dc36, 0xe10b37a9,
	0x88016878, 0xce81d4e4, 0x24d6fc80, 0x4106152d, 0x6d4f5f90, 0xc4dc74be,
	0xdb48676c, 0x6cb569b7, 0xf3bf598f, 0x042b08d9, 0x02ccb2de, 0xb1056f65,
	0x47994af4, 0xfa141ba4, 0x9376ab2e, 0x07a76737, 0x75e7e6fc, 0x449d80a1,
	0x03b7259d, 0xf6df358a, 0x5a75d5b9, 0x47286923, 0x3b1a30ef, 0xeebe3d6a,
	0x9db1aa00, 0x007a90d9, 0x24667071, 0x019c73cf, 0x69039bcd, 0x95900744,
	0x6518b1eb, 0x6905f202, 0xee3951b2, 0xe141fca9, 0x797fa832, 0x5a95e55b,
	0xd6263b15, 0x5b61f394, 0x897acb1c, 0x005f83a9, 0x22420f71, 0xf495176e,
	0x7e138f3d, 0x1392e384, 0x373bf7aa, 0x8e512816, 0xa960b3ca, 0x0474d74c,
	0xffacd6d7, 0x2ef5ed9e, 0x60992aaa, 0x7e690e99, 0x23c0749d, 0xd8e29105,
	0x555d5909, 0x15631bfe, 0xa69c5a1c, 0x501017ca, 0x99438048, 0x38733ac7,
	0xe682e2c8, 0xd4655fd6, 0x956e4c04, 0x347df643, 0x2f4b177b, 0x93ed3aa4,
	0xa77e1dd5, 0x7ae55702, 0xd2a52fd9, 0xef8ba18c, 0xb7d3c1ee, 0x8078ba8d,
	0xab5aaadb, 0x752be08f, 0x068b31c1, 0x078aae3c, 0xaa5a8343, 0x123d9268,
	0x2ceaee43, 0x8ebdb239, 0x650251f3, 0x04883648, 0x8c62e12e, 0x12b32167,
	0xe5112e9a, 0x10002548, 0x3e7a818d, 0x077e5327, 0xf140cc21, 0x6ce7d75d,
	0x9b99f9a5, 0x3215741c, 0xb6aadbae, 0x738768dc, 0x82a3742f, 0x76517020,
	0xdd872ad8, 0x9d0902b2, 0x7d1a6b04, 0x49381592, 0x63a652a5, 0x0c15e626,
	0xe22f70d6, 0x01e84385, 0xb29de134, 0x20c5000e, 0xe961f443, 0x2d31662e,
	0x3ce6bc28, 0x34f9dd94, 0xfa45de53, 0x497588bd, 0x9468215b, 0x0777fa5c,
	0x6f7114c0, 0xe0e82694, 0xe4371986, 0x57112de2, 0xe0cac289, 0xf2a3cee0,
	0x6a41e1b9, 0xbfcea77d, 0xf927fd52, 0x69747d98, 0xbea76cdb, 0x8dd39557,
	0x04db5ece, 0x2a0885c8, 0x3be4e8ee, 0x21d785dc, 0x09de7c0e, 0x3258ea33,
	0x51922982, 0xee8dd024, 0x3df6965d, 0x30c1237b, 0xf7f6686a, 0x9faca186,
	0x7c400076, 0x85acef8a, 0xf4b6d220, 0xddc3481c, 0x439eaec4, 0x717bbe63,
	0x8259faa7, 0xd682bd68, 0x932a8610, 0x38bf0a7f, 0x6212e2c7, 0x88ee3168,
	0xb3c27047, 0x6133cb1e, 0x15295506, 0x5ae66246, 0x1d208ddd, 0xa91d3dba,
	0xc315968d, 0x6aa2664b, 0x716d0cca, 0x891f4956, 0x80866bff, 0xbd56c847,
	0x9093425a, 0x28dd9e87, 0x84ef3e08, 0x690a49d6, 0x6a7eff82, 0xabcfe400,
	0x3d3be5ca, 0x381b650c, 0x4b7c8622, 0x3e0246f3, 0xa3561654, 0x9488865c,
	0x3aef1bf2, 0x5e5d68a2, 0xd32f1ddc, 0x51972bf0, 0x177a213b, 0x469375c2,
	0x37640bd0, 0xfc3324c8, 0x07091a09, 0x2d63d3fb, 0x2153f023, 0x48223875,
	0x61a55826, 0x8c136538, 0x49f71d98, 0x84c7d51e, 0x85551a73, 0x13d604c5,
	0xd701a626, 0x87b844ca, 0x741eb29d, 0x2a2c977c, 0xc797ca03, 0x6c4085d7,
	0x2dacf79b, 0x734fa2eb, 0xcc290557, 0xfa1e75e4, 0x06b29a27, 0xbece2a7a,
	0x70a4554b, 0xc935942e, 0xa764bbc1, 0x1fe391d6, 0x7807f0c2, 0x40606ed9,
	0xe5153086, 0xe91d7dd2, 0xed5d3ba9, 0xaa14b64a, 0x83b24dd9, 0xec1ff5cd,
	0xba33ead3, 0xe4ef735c, 0xbc062438, 0xd8bfd523, 0x473d1e04, 0x2007f8a7,
	0xb02903ed, 0x86ea8ada, 0x95ab69cf, 0xfd1f9809, 0x9cb3d8bb, 0x51f45958,
	0x9cdd4276, 0xc245865e, 0x8f0c836b, 0x4ee7dc07, 0xf6368d9d, 0xef2c1dc1,
	0xee56b54b, 0xbd62ce2f, 0xf4916aad, 0xc81cb594, 0x41729f49, 0x24bef0a4,
	0xdef487a9, 0x222e05b8, 0x8d3bf5c6, 0x11b55009, 0xad09d2b3, 0x19db9fd1,
	0xd7427085, 0x33dbfc8b, 0x526b9378, 0x790e1bc8, 0xb2998a00, 0xa5641703,
	0x0676d249, 0x6b9185cc, 0x30e4348f, 0x82c52f65, 0x57c7dc24, 0x489c1ecd,
	0x9fcab02a, 0x56d61117, 0xfe869cac, 0x55fc5140, 0x7fbbb382, 0x9e5afc79,
	0x10047c99, 0xfc9f5984, 0x56587e2d, 0xb98193f0, 0x98fe5e8e, 0x29b15b6b,
	0x9561f055, 0xbb0caa25, 0x1e4ecc15, 0x23f5393b, 0x0845b458, 0xceff67ca,
	0xb099900c, 0x00b1564f, 0x39eef3d1, 0xfcc1bf84, 0xac8893b5, 0x6484bf0e,
	0x91c02ab3, 0x8c0c0c70, 0x686fa8c6, 0xe171bed6, 0xdfae37df, 0xd5a1a4e7,
	0xe3eb49a1, 0x5e6014e0, 0x205b21ac, 0xfd58b3da, 0x2e7c07cd, 0xef2cc85a,
	0xd7587b46, 0xf417847d, 0x8a30cec1, 0x70984f6c, 0xf0b63388, 0xc220c98d,
	0xede62936, 0x92c0a7b3, 0x1ef371e8, 0x2005f7af, 0x91a47265, 0xb0cf5504,
	0xd500aba8, 0xcb5c4bd3, 0x9b3bcbc3, 0xcf6644b5, 0xce9488ef, 0x003fc96e,
	0xaa42222f, 0x4844f3d0, 0x4db89d77, 0x08681aae, 0x662f3a28, 0x761552db,
	0x1df7a17a, 0x93feed9a, 0xcc496a4f, 0xa217cfcd, 0x3ba3c930, 0x268f7e77,
	0x0797b4a1, 0x8bebfc51, 0x068930c4, 0x16c874e2, 0xc242da24, 0xfb229f76,
	0xa0795b02, 0x689fc036, 0x17a73732, 0xd21aec00, 0xac00a692, 0x5b217f18,
	0xae421624, 0x2bc05cc0, 0x48c1db7a, 0x4f4e63b4, 0x1667f04e, 0x34020f94,
	0x972b2555, 0x9a07355b, 0x01665970, 0x7db60c6f, 0x3ad7103b, 0x5c3d09c0,
	0xeea3dada, 0x88c21c10, 0x102436d7, 0x6a3b3400, 0xeb523c4c, 0xfb97d896,
	0x964cb86b, 0xdd878038, 0x0529da4d, 0x0b1468a5, 0x18739ac8, 0xf7f26668,
	0xf64f4471, 0x5c14f5c3, 0x44a081fb, 0x39ac7e37, 0x8a17c26b, 0x868f5e67,
	0x3931978d, 0x6edf7817, 0x4951cc67, 0x943407f3, 0xcc5e748f, 0x2b7ee729,
	0xcbb320f0, 0x11fec8e7, 0xfccfc658, 0x03454354, 0x373aa1ec, 0x1d58fe9a,
	0x064710ae, 0xa88aa0ba, 0xd183a23e, 0x40d150a3, 0xf531b8d1, 0xa7d99f85,
	0x11838cd5, 0xb19e64b3, 0x3d67a5e9, 0xb02c5ac6, 0x99b9b9e8, 0x4c202b7a,
	0x15f261d3, 0xa84c2d0d, 0x50f185a6, 0x33ba41d5, 0x39791013, 0x4baff44e,
	0xeeeeaa1c, 0xe0488314, 0x559ccd2b, 0xa104f445, 0x636f37c4, 0x264d5e3b,
	0x75c17f35, 0x75424131, 0xbb115739, 0x74fe755a, 0x7d3a7aa6, 0x2d8be784,
	0x83ed154a, 0xfc2673d8, 0x44dd4a7f, 0x79056cc8, 0x82cc8831, 0x9d3c1b7c,
	0xe9453bfa, 0x24315694, 0x661f3253, 0x75549f5c, 0xbb2b63ed, 0x67e00d96,
	0xf48966c7, 0x0d7bea56, 0xc25f92ef, 0xa947a79d, 0xde4adf6f, 0xac0f0342,
	0xd3eb246b, 0xa4aa118e, 0x3c3e6a46, 0x457f4441, 0xa50a406f, 0x6c508d9f,
	0xe9ac18e7, 0x1ecdb4ba, 0x39ac7e3a, 0x7fb304fa, 0x6f38f8e8, 0x4aecea6d,
	0x61035e73, 0x81708907, 0xebc07205, 0x90fd7614, 0xb52d217f, 0x6c4de195,
	0x1dd49084, 0x64ee482c, 0x94c7a521, 0x540c09d8, 0x75df8dd5, 0x414131f7,
	0x3698fd76, 0xf784db4f, 0xf8c97a03, 0x048f39b9, 0x3bf4f0bd, 0x8cb50992,
	0x9b58d9ee, 0xe5ab79cc, 0x9a5f6052, 0xbd9591b0, 0xfad2232b, 0x5a632254,
	0x0286e618, 0x8ad3c8f7, 0xe4060176, 0x754c4617, 0x5c10490b, 0x6f7d6fff,
	0x2187b42a, 0x5775095b, 0x02f4c663, 0x5a5dca06, 0xfe4ad4c7, 0x53e19f7d,
	0x59ff46b5, 0xbcc42ba5, 0xfd2f4a97, 0xbed6d905, 0x95629b6b, 0x21a1c0db,
	0xaa10b45d, 0xe6ef6d58, 0x2892cf4d, 0x9fed6c10, 0x1e386bf7, 0x9be0c6e8,
	0x2b2f15ef, 0x19f5ac7b, 0x7aff0e72, 0x31da576f, 0x30252cb4, 0x577960ac,
	0x166e9e5a, 0xa9374a61, 0x71369c96, 0x7ff826ae, 0xe8175326, 0xcabbfd33,
	0x0191190e, 0x699d3c3e, 0x36b40b22, 0xb3950513, 0x9b889bfa, 0xa52a5007,
	0xac290fed, 0x3b4e4a4f, 0xb753d8d6, 0x3c531f22, 0x582f6427, 0xa9cd93a9,
	0x546e39ae, 0x242faad2, 0xd2e0f747, 0x09f6325d, 0x59d48719, 0xad7eb66e,
	0xd5512878, 0x56debf9d, 0x5107e5a5, 0xf1c00aa4, 0x814ccca8, 0x600d90f0,
	0x9be97619, 0x915fa5f2, 0x2b5628dd, 0xa33d5f5a, 0x595df7c1, 0x6966215d,
	0x50ec8337, 0xf1d21372, 0x0ee2eefb, 0xad9e70b7, 0xab0d2fe4, 0xcf277b5d,
	0x62585a2c, 0x835a7844, 0x74b1fa6b, 0x49baffd5, 0x2ea9c864, 0x129311a8,
	0xbdfa1867, 0x83ca5997, 0x9d1db719, 0x84bb79e6, 0x9e3f99f2, 0x313f6101,
	0x1b99245b, 0xd15d8fb2, 0xcef90f81, 0x2945268d, 0xdbbcf573, 0xb1021886,
	0x9ee7ec1d, 0x1cf824f7, 0x7eaa2e32, 0x69c0a2b5, 0x7494419c, 0xe253d7d3,
	0x48da3d12, 0x45b8b571, 0xdb4d147a, 0xd82d8dde, 0x265d10a2, 0xb0a6eb9a,
	0x7e1c93a6, 0x36fe2f46, 0xdcad6b00, 0x05439191, 0xb0ce5484, 0x61d1c309,
	0x8da62a03, 0x06d0fe2f, 0xbac6dd3c, 0xca2006f3, 0x8321b1af, 0x0411a6f3,
	0xe8918eac, 0x21a2c152, 0x91c0d54f, 0x6aaa14fa, 0xdd22a440, 0x88cb2075,
	0x7a4eb813, 0x67afa071, 0xd8d98c9c, 0x31f10d47, 0x6ff1a8a8, 0x2faaf0a1,
	0x48a221bb, 0x3be6948b, 0xaa79e79b, 0x0ea7278c, 0x7a3857ef, 0x49b7fe55,
	0xd51cb931, 0x041c018d, 0x00b90501, 0x45ea7881, 0x8fc1dbcf, 0xb80b32a9,
	0xabacd2e9, 0x677bdc40, 0xecace542, 0x6d6514eb, 0x31c09ff7, 0x5e6c1abd,
	0x1c391d0f, 0x0e9d77f1, 0x7119392d, 0x6be9b0ba, 0x6194fa77, 0x45e62148,
	0x42234af2, 0xc3239d66, 0x939cbdbc, 0x56200d9c, 0x6b275208, 0x001a61f3,
	0xccc2a546, 0x4b722be0, 0xee25f2b7, 0x6d86cf9e, 0xaa6be0cd, 0x4dcda7b6,
	0x78d4aa13, 0x36ea7ad9, 0x3f29d700, 0xdeea2d84, 0x6a6af5bd, 0x18afb81c,
	0xd8e4e73c, 0x8aa708ba, 0x658b94d9, 0xa676478c, 0xcfa10c22, 0x25593c74,
	0x8d962235, 0x5f980270, 0x3df6ebc0, 0x8e7d92fa, 0xc3ee55e1, 0xd5f72447,
	0x02b0fa95, 0x52b0b520, 0x70d2c11f, 0x3a6fdd6c, 0x193aa698, 0x5496f7d5,
	0x4208931b, 0x7a4106ec, 0x83e86840, 0xf49b6f8c, 0xba3d9a51, 0x55f54ddd,
	0x2de51372, 0x9afb571b, 0x3ab35406, 0xad64ff1f, 0xc77764fe, 0x7f864466,
	0x416d9cd4, 0xa2489278, 0xe30b86e4, 0x0b5231b6, 0xba67aed6, 0xe5ab2467,
	0x60028b90, 0x1d9e20c6, 0x2a7c692a, 0x6b691cdb, 0x9e51f817, 0x9b763dec,
	0x3d29323f, 0xcfe12b68, 0x754b459b, 0xa2238047, 0xd9c55514, 0x6bdcffc1,
	0x693e6340, 0x82383fe7, 0x1916ea5f, 0xec7bcd59, 0x72de165a, 0xe79a1617,
	0x8ec86234, 0xa8f0d284, 0x20c90226, 0x7bf98884, 0x28a58331, 0x3ec3fa6e,
	0x4ce0895b, 0xc353b4d0, 0x33ef064f, 0x21e5e210, 0xc8bb589d, 0xe85dcab2,
	0xac65829f, 0xa7bf92d0, 0x05a6174d, 0x25a50c2e, 0xe5c78777, 0x3d75021f,
	0x4baa9c98, 0x23bdc884, 0x9653bbd7, 0xbadce7f5, 0xc283a484, 0xc040df2e,
	0x9370a841, 0x2f316022, 0x36eed231, 0xac2cbc0c, 0x13c0a49b, 0xcdd12997,
	0x07fe91b2, 0xcd7eabcd, 0x2c01271d, 0x18432df8, 0x599c6bc7, 0x75e93d5a,
	0xb67a6ee2, 0x8e738e16, 0xff9073fd, 0xaf77026a, 0xf86ea2fc, 0x91509ea3,
	0x33a78dc6, 0x4f79234a, 0x3a7535bc, 0x3539fcb1, 0x3103ee52, 0x4f6f1e69,
	0x6bb3ebbc, 0x4cb77555, 0x8dd1e999, 0x2ade439d, 0x11521fae, 0xb94d2545,
	0x8dde9abd, 0x1909393f, 0xb792a23d, 0x749c455b, 0xb5b60f2c, 0x380459ce,
	0x0dad5820, 0xb130845b, 0x291cbd52, 0xde9a5bb7, 0x51def961, 0x515b6408,
	0xca6e823e, 0x382e6e74, 0xeebe3d71, 0x4c8f0c6a, 0xe676dcea, 0x14e1dc7c,
	0x6f7fc634, 0xcf85a943, 0xd39ea96e, 0x136e7c93, 0x7164b304, 0xf32f1333,
	0x35c34034, 0xde39d721, 0x91a87439, 0xc410111f, 0x29f17aac, 0x1316a6ff,
	0x12f194ee, 0x420b9499, 0xf72db0dc, 0x690b9f93, 0x17d14bb2, 0x8f931ab8,
	0x217500bc, 0x875413f8, 0x98b2e43d, 0xc51f9571, 0x54cebdca, 0x0719cc79,
	0xf3c7080d, 0xe4286771, 0xa3eab3cd, 0x4a6b00e0, 0x11cf0759, 0x7e897379,
	0x5b32876c, 0x5e8cd4f6, 0x0cedfa64, 0x919ac2c7, 0xb214f3b3, 0x0e89c38c,
	0xf0c43a39, 0xeae10522, 0x835bce06, 0x9eec43c2, 0xea26a9d6, 0x69531821,
	0x6725b24a, 0xda81b0e2, 0xd5b4ae33, 0x080f99fb, 0x15a83daf, 0x29dfc720,
	0x91e1900f, 0x28163d58, 0x83d107a2, 0x4eac149a, 0x9f71da18, 0x61d5c4fa,
	0xe3ab2a5f, 0xc7b0d63f, 0xb3cc752a, 0x61ebcfb6, 0x26ffb52a, 0xed789e3f,
	0xaa3bc958, 0x455a8788, 0xc9c082a9, 0x0a1bef0e, 0xc29a5a7e, 0x150d4735,
	0x943809e0, 0x69215510, 0xef0b0da9, 0x3b4e9fb3, 0xd8b5d04c, 0xc7a023a8,
	0xb0d50288, 0x64821375, 0xc260e8cf, 0x8496bd2c, 0xff4f5435, 0x0fb5560c,
	0x7cd74a52, 0x93589c80, 0x88975c47, 0x83bda89d, 0x8bcc4296, 0x01b82c21,
	0xfd821dbf, 0x26520b47, 0x04983e19, 0xd3e1ca27, 0x782c580f, 0x326ff573,
	0xc157bcc7, 0x4f5e6b84, 0x44ebfbfb, 0xda26d9d8, 0x6cd9d08e, 0x1719f1d8,
	0x715c0487, 0x2c2d3c92, 0x53faaba9, 0xbc836146, 0x510c92d6, 0xe089f82a,
	0x4680171f, 0x369f00de, 0x70ec2331, 0x0e253d55, 0xdafb9717, 0xe5dd922d,
	0x95915d21, 0xa0202f96, 0xa161cc47, 0xeacfa6f1, 0xed5e9189, 0xdab87684,
	0xa4b76d4a, 0xfa704897, 0x631f10ba, 0xd39da8f9, 0x5db4c0e4, 0x16fde42a,
	0x2dff7580, 0xb56fec7e, 0xc3ffb370, 0x8e6f36bc, 0x6097d459, 0x514d5d36,
	0xa5a737e2, 0x3977b9b3, 0xfd31a0ca, 0x903368db, 0xe8370d61, 0x98109520,
	0xade23cac, 0x99f82e04, 0x41de7ea3, 0x84a1c295, 0x09191be0, 0x30930d02,
	0x1c9fa44a, 0xc406b6d7, 0xeedca152, 0x6149809c, 0xb0099ef4, 0xc5f653a5,
	0x4c10790d, 0x7303286c
};