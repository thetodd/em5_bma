# Change Log
All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
### Fixed
- Event creation fails if no BMA is available
- Debug command executes on right click on vehicle
- Crash if try to reset BMA when not all fires are extinguished in BMA-Event

## [0.1.1] 2016-07-07
### Added
- Debug command which triggers a BMA event
- BMA fire event
- BMA fire event factory
- BMA reset command and action
- BMA Component to represent a BMA
- Testmod for easy including into EM5

### Fixed
- Debug command blocks MoveCommand of vehicle
