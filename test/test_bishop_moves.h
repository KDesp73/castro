#ifndef test_bishop_moves_h
#define test_bishop_moves_h

#define TEST_test_bishop_moves\
	test_bishop_moves("rnb1kb1r/ppp1p1pp/5n2/3pPp2/7q/2N5/PPPP1PPP/R1BQKBNR w KQkq - 0 1", "c1", NULL), \
	test_bishop_moves("rnb1kb1r/ppp1p1pp/5n2/3pPp2/7q/2N5/PPPP1PPP/R1BQKBNR w KQkq - 0 1", "f1", "e2", "d3", "c4", "b5", "a6", NULL), \
	test_bishop_moves("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Kkq - 0 1", "a6", "b5", "c4", "d3", "e2", "b7", "c8", NULL), \
	test_bishop_moves("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Kkq - 0 1", "g7", "f8", "h6", NULL), \
	test_bishop_moves("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w Kkq - 0 1", "d2", "c1", "e3", "f4", "g5", "h6", NULL), \
	test_bishop_moves("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w Kkq - 0 1", "e2", "d1", "f1", "d3", "c4", "b5", "a6", NULL), \
	test_bishop_moves("r2q1rk1/2p1bppp/p1n1bn2/1p2p3/4P3/2P2N2/PPBN1PPP/R1BQR1K1 w - - 1 12", "c1", NULL), \
	test_bishop_moves("r2q1rk1/2p1bppp/p1n1bn2/1p2p3/4P3/2P2N2/PPBN1PPP/R1BQR1K1 w - - 1 12", "c2", "b1", "b3", "a4", "d3", NULL ), \
	test_bishop_moves("r2q1rk1/2p1bppp/p1n1bn2/1p2p3/4P3/2P2N2/PPBN1PPP/R1BQR1K1 b - - 1 12", "e7", "d6", "c5", "b4", "a3", NULL), \
	test_bishop_moves("r2q1rk1/2p1bppp/p1n1bn2/1p2p3/4P3/2P2N2/PPBN1PPP/R1BQR1K1 b - - 1 12", "e6", "d5", "c4", "b3", "a2", "d7", "c8", "f5", "g4", "h3", NULL), \
	test_bishop_moves("r1bqrbk1/2p2pp1/p1np1n1p/1p2p3/4P3/PBNP1N1P/1PPB1PP1/R2Q1RK1 w - - 3 12", "b3", "a2", "c4", "d5", "e6", "f7", "a4", NULL), \
	test_bishop_moves("r1bqrbk1/2p2pp1/p1np1n1p/1p2p3/4P3/PBNP1N1P/1PPB1PP1/R2Q1RK1 w - - 3 12", "d2", "c1", "e3", "f4", "g5", "h6", "e1", NULL), \
	test_bishop_moves("rnbqk1nr/pppp1ppp/8/4p3/1b2P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 0 1", "b4", "a3", "a5", "c3", "c5", "d2", "d6", "e7", "f8", NULL), \
	test_bishop_moves("rnbqk1nr/pppp1ppp/8/4p3/1b1PP3/2P2N2/PP3PPP/RNBQKB1R b KQkq - 0 1", "b4", "a3", "a5", "c3", "c5", "d6", "e7", "f8", NULL), \
	test_bishop_moves("rnbqk1nr/pppp1ppp/8/4Q3/1b1PP3/2P2N2/PP3PPP/RNB1KB1R b KQkq - 0 1", "b4", "e7", NULL), \
	test_bishop_moves("rnbqk1nr/pppp1ppp/8/4p3/1b1PP3/2P2N2/PP3PPP/RNBQKB1R b KQkq - 0 1", "c8", NULL), \
	test_bishop_moves("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "c1", NULL), \
	test_bishop_moves("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f1", NULL), \
	test_bishop_moves("r1b1kbnr/pppppppp/8/6n1/1q1P4/8/PPPBPPPP/RN1QKBNR w KQkq - 0 1", "d2", "c3", "b4", NULL), \
	test_bishop_moves("r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/6P1/PPPP1PBP/RNBQK1NR w KQkq - 0 1", "g2", "f1", "h3", "f3", NULL), \
	test_bishop_moves("r1bqkb1r/ppp2ppp/2B2n2/4p3/8/2N3P1/PPPP1P1P/R1BQK1NR b KQkq - 0 1", "c8", "d7", NULL), \
	test_bishop_moves("r1bqkb1r/ppp2ppp/2B2n2/4p3/8/2N3P1/PPPP1P1P/R1BQK1NR b KQkq - 0 1", "f8", NULL)

#endif
