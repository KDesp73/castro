#ifndef test_king_moves_h
#define test_king_moves_h

#define TEST_test_king_moves\
	test_king_moves("rnb1kb1r/ppp1p1pp/5n2/3pPp2/7q/2N5/PPPP1PPP/R1BQKBNR w KQkq - 0 1", "e1", "e2", NULL), \
	test_king_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "e1", NULL), \
	test_king_moves("1R6/1P4k1/5p2/1r3K2/8/7P/6P1/8 w - - 5 55", "f5", "e6", "e4", "f4", "g4", NULL), \
	test_king_moves("r1bqrbk1/2p2pp1/p1np1n1p/1p2p3/4P3/PBNP1N1P/1PPB1PP1/R2Q1RK1 w - - 3 12", "g1", "h1", "h2", NULL), \
	test_king_moves("r1bqrbk1/2p2pp1/p1np1n1p/1p2p3/4P3/PBNP1N1P/1PPB1PP1/R2Q1RK1 b - - 3 12", "g8", "h8", "h7", NULL), \
	test_king_moves("r1bqk2r/pp3ppp/2n2n2/1B2p3/8/2bP1N2/PPP2PPP/R1BQK2R w KQkq - 0 1", "e1", "e2", "f1", NULL), \
	test_king_moves("r2qk2r/pp3ppp/2n2n2/1B2pb2/8/B1PP1N2/P1P2PPP/R2QK2R b KQkq - 0 1", "e8", "d7", NULL), \
	test_king_moves("r2qk2r/pp3ppp/2B2n2/4pb2/8/B1PP1N2/P1P2PPP/R2QK2R b KQkq - 0 1", "e8", NULL), \
	test_king_moves("r2qk2r/pp3ppp/2n2n2/1B2pb2/8/B1PP1N2/P1P2PPP/R2QK2R w KQkq - 0 1", "e1", "d2", "e2", "f1", "g1", NULL), \
	test_king_moves("r2qk2r/pp3ppp/2n2n2/1B2pb2/8/B1PP1N2/P1P2PPP/R2QK2R w Qkq - 0 1", "e1", "d2", "e2", "f1", NULL), \
	test_king_moves("r2q3r/ppNk2pp/2n1Qpn1/1B6/8/2PP4/P1P2PPP/R1B1K2R b KQha - 0 1", "d7", "c7", NULL), \
	test_king_moves("r2q3r/ppNk2pp/1Bn1Qpn1/8/8/2PP4/P1P2PPP/R1B1K2R b KQ - 0 1", "d7", NULL), \
	test_king_moves("r2q3r/pp1k2pp/1Bn1Qpn1/3N4/8/2PP4/P1P2PPP/R1B1K2R b KQha - 0 1", "d7", "e6", NULL), \
	test_king_moves("8/8/kQ6/8/8/8/8/3K4 b - - 0 1", "a6", "b6", NULL), \
	test_king_moves("8/1Q6/k7/8/8/8/8/3K4 b - - 0 1", "a6", "b7", "a5", NULL), \
	test_king_moves("8/1Q6/k1K5/8/8/8/8/8 b - - 0 1", "a6", "a5", NULL), \
	test_king_moves("8/8/kQK5/8/8/8/8/8 b - - 0 1", "a6", NULL), \
	test_king_moves("8/8/k1K5/8/8/8/8/Q7 b - - 0 1", "a6", NULL), \
	test_king_moves("8/8/k1K3Q1/8/8/8/8/R7 b - - 0 1", "a6", NULL), \
	test_king_moves("8/8/k7/8/8/6K1/8/RQ6 b - - 0 1", "a6", NULL), \
	test_king_moves("8/8/k7/1P6/8/6K1/8/RQ6 b - - 0 1", "a6", "b6", "b7", NULL), \
	test_king_moves("8/1p6/k7/1P6/8/6K1/8/RQ6 b - - 0 1", "a6", "b6", NULL), \
	test_king_moves("8/1p6/k7/1PP5/8/6K1/8/RQ6 b - - 0 1", "a6", NULL), \
	test_king_moves("4Q3/1p6/k7/2P5/1P6/6K1/8/R7 b - - 0 1", "a6", NULL)

#endif
