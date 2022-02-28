# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Changed
- Bluetooth config: Reduce heap size by changing:
  - Max ACL connections: from 7 to 4
  - Max SCO connections: from 3 to 0
  - Max BLE connections: from 3 to 1

## [1.2.2] - 2022-02-27
### Changed
- Bluepad32 v3.0.0

## [1.2.1] - 2022-02-06
### Changed
- Bluepad32 v3.0.0-rc1
- ESP-IDF Arduino Core reverted back to v2.0.2

## [1.2.0] - 2022-01-31
### Changed
- Bluepad32 v3.0.0-rc0
- ESP-IDF Arduino Core hash: ef35baffb0ebc6c2f471767bb6f29ad4537a6a50
- ESP-IDF v4.4, hash: eb3797dc3ffebd9eaf873a01df63aed89fad58b6)


## [1.1.0] - 2022-01-08
### Changed
- Bluepad32 v2.5.0
- BTStack v1.5.1, hash: d778e7488c62d361fc176e8ae40c0d6bc8b00089


## [1.0.0] - 2021-12-26
### New
- Initial release, includes:
  - Bluepad32 v2.4.0
  - BTStack from 2021/12/03, hash: 0f2a810173d1d70943d1c915bffd6f9b1171e8f6
  - ESP-IDF Arduino core: v2.0.2
