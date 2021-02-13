# 例程
* 推荐调试器：J-Link
* 所有例程均支持使用HID IAP下载
* 核心板出厂自带HID IAP程序，按住KEY键上电即可进入HID IAP，使用`Soft/GD32/GD32_MCU_HID_IAP_Programmer_V2.0.3.5957.rar`上位机即可实现例程程序烧录

> 如何进入串口下载
1. `按住BOOT0键上电` 或者 `按住BOOT0键复位`
2. PA9->USB RXD,PA10->USB TXD
3. 使用`Soft/GD32/GD32All_In_OneProgrammer_V1.2.8.9234.rar`烧录，串口波特率`115200`

> 如何进入HID下载
1. 确认已烧录HID IAP固件
2. `按住KEY键上电` 或者 `按住KEY键复位`
3. 确认APP固件地址为`0x8002000`,中断地址偏移为`0x2000`
4. 使用`Soft/GD32/GD32_MCU_HID_IAP_Programmer_V2.0.3.5957.rar`下载

> 无法找到SWD设备时的操作
* 进入串口下载模式，看下可否找到
* 进入串口下载模式，使用串口擦除整个Flash

# Example
* Recommended debugger: J-Link
* All routines are supported using HID IAP downloads
* The core board comes with HID IAP program. You can enter HID IAP by holding down the KEY and then using 'Soft/GD32/ GD32_mcu_hid_iap_programmer_v2.0.3.5957. rar' host computer to achieve routine program burning

> How to enter serial port download
1. `Hold BOOT0 key to power` or `Hold BOOT0 key to reset`
2. PA9->USB RXD,PA10->USB TXD
3. Use `Soft/GD32/ GD32all_in_oneprogrammer_v1.2.8.9234.rar` to burn, serial port baud rate '115200'

> How does enter HID download
1. Confirm that HID IAP firmware has been burned
2. `Hold the KEY to power` or `hold the KEY to reset`
3. Confirm that APP firmware address is `0x8002000`and interrupt address offset is `0x2000`
4. Use `Soft/GD32/ gd32_mcu_hid_iap_programmer_v2.0.3.5957. rar` to download

> The operation when could not find the SWD device
* Enter serial port download mode, see if you can find it
* Enter serial download mode and use the serial port to erase the entire Flash
