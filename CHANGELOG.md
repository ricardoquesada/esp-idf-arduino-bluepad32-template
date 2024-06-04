# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.1.0] - 2024-06-03
- Bluepad32 v4.1.0
- BTstack v1.6.1
- Arduino Core 2.0.17

## [4.0.4] - 2024-04-20
- Bluepad32 v4.0.4
- Arduino 2.0.15
- BTstack cb92fd343998b0ca7a4cb5700654429febb21b44
- Arduino API:
  - `Controller.isAnyKeyPressed()` added

## [4.0.3] - 2024-04-09
- Bluepad32 / BTstack: upstream v4.0.3

## [4.0.2] - 2024-04-03
- Bluepad32 / BTstack: upstream v4.0.2

## [4.0.1] - 2024-04-02
- Bluepad32 / BTstack: upstream v4.0.1

## [4.0] - 2024-03-24
- Bluepad32 / BTstack: upstream v4.0

## [4.0-rc0] - 2024-03-20
- Bluepad32 / BTstack: using Bluepad32 v4.0-rc0
- Arduino API:
  - `Controller.playDualRumble(...)` added
  - `Controller.setRumble(...)` deprecated. Internally it calls `playDualRumble()`

## [4.0-beta2] - 2024-03-09
- Arduino core: 03da972492323ca35382c7a966cf41a992fa4bb1
- Arduino API:
    - `BP32.enableBLEService(bool)` to enable/disable the BLE service
    - `Cosole.write()` and `Console.println(void)` added.
- Enable Single Large APP partition by default. Before it was small
- Bluepad32 Arduino: Add missing functionality to `setup()` and `loop()` regarding WDT.

## [4.0-beta1] - 2024-02-11
- Bluepad32 v4.0-beta1 upstream
- Arduino API:
    - `BP32.update()` returns `true` if data was received.
    - `Controller.hasData()` return `true` if data was received since
      `BP32.update()` was called.
- Arduino Sketch: updated to use new API

## [4.0-beta0] - 2024-02-04
- Bluepad32 v4.0-beta0
- BTstack develop branch: 4b3f8617054370b0e96650ee65edea1c23591ed4

## [3.10.2] - 2023-11-14
- Bluepad32 v3.10.2
- BTstack develop branch: d30de50130b2838470ab89e3ae004198f0188810

## [3.10.1] - 2023-11-05
- Bluepad32 v3.10.1

## [3.9.0] - 2023-09-17
- Bluepad32 v3.9.0
- Arduino Core v2.0.12
- BTstack develop branch: 6d7084813262308410501e1cf0c602a6ec2b0471

## [3.8.2] - 2023-08-02
- Bluepad32 v3.8.2
- Arduino Core v2.0.11

## [3.8.1] - 2023-07-30
- Bluepad32 v3.8.1
- BTstack v1.5.6.3

## [3.7.3] - 2023-06-17
- Bluepad32 v3.7.3

## [3.7.2] - 2023-05-20
- Bluepad32 v3.7.2
- Arduino Core v2.0.9

## [3.7.1] - 2023-04-30
- Bluepad32 v3.7.1
- Arduino Core v2.0.8

## [1.5.3] - 2023-04-04
- Bluepad32 v3.6.2
- Arduino Core v2.0.7

## [1.5.2] - 2023-02-04
- Bluepad32 v3.6.0

## [1.5.1] - 2023-01-30
- Bluepad32 v3.6.0-rc1
- Update sdkconfig.defaults to enable BLE

## [1.5.0] - 2023-01-16
- Bluepad32 v3.6.0-rc0
- ArduinoCore v2.0.6
- BTStack v1.5.5

## [1.4.4] - 2022-12-05
### Changed
- ArduinoCore v2.0.5
- Bluepad32 v3.5.2

## [1.4.3] - 2022-10-01
### Changed
- Bluepad32 v3.5.1

## [1.4.2] - 2022-07-10
### Changed
- Bluepad32 v3.5.0

## [1.4.1] - 2022-07-04
### Changed
- Bluepad32 v3.5.0-rc0

## [1.4.0] - 2022-06-17
### Changed
- ESP-IDF Arduino Core v2.0.3
- Bluepad32 v3.5.0-beta1
- BTstack hash: a45da34ed9d3615c94c57ef08760939900575afb

## [1.3.1] - 2022-04-24
### Changed
- Bluepad32 v3.1.1

## [1.3.0] - 2022-04-14
### Changed
- Bluepad32 v3.1.0
- BtStack hash: 0a8cccd5b3b9a07be7fb40663735e37a9a8a0040

## [1.2.3] - 2022-03-15
### Changed
- Bluepad32 v3.0.1
- BtStack hash: 45e937e1ce34444aabbc21ec35504e1c50ad528d
- Compiler config: Use "optimize for performance" by default
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
