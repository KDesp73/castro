#ifndef test_pawn_moves_h
#define test_pawn_moves_h

#define TEST_test_pawn_moves\
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "a2", "a3", "a4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "b2", "b3", "b4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "c2", "c3", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "d2", "d3", "d4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "g2", "g3", "g4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "h2", "h3", "h4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "e4", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "f2", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "a7", "a6", "a5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "b7", "b6", "b5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "d7", "d6", "d5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "g7", "g6", "g5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "h7", "h6", "h5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "c7", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "e5", NULL), \
	test_pawn_moves("r1bqk2r/pppp1ppp/2n2n2/4p3/2b1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "f7", NULL), \
	test_pawn_moves("rnb1kb1r/ppp1p1pp/5n2/3pPp2/7q/2N5/PPPP1PPP/R1BQKBNR w KQkq d6 0 1", "e5", "d6", "e6", "f6", NULL), \
	test_pawn_moves("rnb1kb1r/ppp1p1pp/5n2/3pPp2/7q/2N5/PPPP1PPP/R1BQKBNR w KQkq d6 0 1", "f2", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "c7", "c6", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "b7", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "a7", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "e7", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "f7", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "g7", NULL), \
	test_pawn_moves("rnbqkbnr/ppp1pppp/8/1B1P4/8/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1", "h7", NULL), \
	test_pawn_moves("rnbq1bnr/ppp1kppp/4p3/3P4/1B6/8/PPPP1PPP/RNBQK1NR b KQha - 0 1", "c7", "c5", NULL), \
	test_pawn_moves("rnbq1bnr/ppp1kppp/4p3/1B1P4/8/8/PPPP1PPP/RNBQK1NR w KQ - 0 1", "d5", "d6", "e6", NULL)

#endif
