# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [0.3.1] - 2026-03-12 

### Added

- Using tinydocs for documentation
- Null move testing

### Changed

- Better tests

### Fixed

- Hash corrections
- Null move methods name


## [0.3.0] - 2026-02-22 

### Added

- Add move capture detection and order legal moves for improved search efficiency
- Benchmarking
- Implement hash table capacity management and add decrement method for position tracking
- Implement pseudo-legal perft and enhance bishop/rook attack calculations
- Occupancy tracking for pieces and update methods for board state management
- Optimize perft stack management and improve legality context initialization

### Changed

- Enhance move legality checks for pawns, knights, and bishops
- Partial zobrist hash updates
- Bumped version

### Removed

- Unused move types


## [0.2.2] - 2026-02-15 

### Added

- Prefixed all methods with `castro_`

### Changed

- Bumped version


## [0.2.1] - 2025-07-13 

### Changed

- Faster perft
- Bumped version to 0.2.1
- Better testing interface


## [0.2.0] - 2025-07-04 

### Added

- Documented castro.h

### Changed

- bumped version to 0.2.0
- Castro now using a single header


## [0.1.0] - 2025-02-06 

### Added

- Added bitboard representation
- Introduced Color type
- Added history
- Do/Undo move mechanism
- Started UCI protocol
- Added gui
- Correct move generation

### Changed

- Pre-computing piece masks
- Using hybrid board representation

### Fixed

- Fixed movegen bugs (passing all perft tests)


[0.1.0]: https://github.com/KDesp73/castro//releases/tag/v0.1.0
[0.2.0]: https://github.com/KDesp73/castro//releases/tag/v0.2.0
[0.2.1]: https://github.com/KDesp73/castro//releases/tag/v0.2.1
[0.2.2]: https://github.com/KDesp73/castro//releases/tag/v0.2.2
[0.3.0]: https://github.com/KDesp73/castro//releases/tag/v0.3.0
[0.3.1]: https://github.com/KDesp73/castro//releases/tag/v0.3.1

