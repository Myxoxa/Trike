
#### The hoverboards with mainboards also come with 2 sideboards(not [splitboards](https://github.com/EFeru/hoverboard-firmware-hack-FOC/wiki/Firmware-Compatibility#split-boards)), check the following [wiki](https://github.com/EFeru/hoverboard-firmware-hack-FOC/wiki/Sideboards) about this firmware

#### For the FOC controller design, see the following repository:
 - [bldc-motor-control-FOC](https://github.com/EFeru/bldc-motor-control-FOC)

#### Videos:
<table>
  <tr>
    <td><a href="https://youtu.be/IgHCcj0NgWQ" title="Hovercar" rel="noopener"><img src="/docs/pictures/videos_preview/hovercar_intro.png"></a></td>
    <td><a href="https://youtu.be/gtyqtc37r10" title="Cruise Control functionality" rel="noopener"><img src="/docs/pictures/videos_preview/cruise_control.png"></a></td>
    <td><a href="https://youtu.be/jadD0M1VBoc" title="Hovercar pedal functionality" rel="noopener"><img src="/docs/pictures/videos_preview/hovercar_pedals.png"></a></td>
  </tr>
  <tr>
    <td><a href="https://youtu.be/UnlbMrCkjnE" title="Commutation vs. FOC (constant speed)" rel="noopener"><img src="/docs/pictures/videos_preview/com_foc_const.png"></a></td> 
    <td><a href="https://youtu.be/V-_L2w10wZk" title="Commutation vs. FOC (variable speed)" rel="noopener"><img src="/docs/pictures/videos_preview/com_foc_var.png"></a></td>       
    <td><a href="https://youtu.be/tVj_lpsRirA" title="Reliable Serial Communication" rel="noopener"><img src="/docs/pictures/videos_preview/serial_com.png"></a></td>
  </tr>
</table>


---
## Hardware
 
![mainboard_pinout](/docs/pictures/mainboard_pinout.png)

The original Hardware supports two 4-pin cables that originally were connected to the two sideboards. They break out GND, 12/15V and USART2&3 of the Hoverboard mainboard. Both USART2&3 support UART, PWM, PPM, and iBUS input. Additionally, the USART2 can be used as 12bit ADC, while USART3 can be used for I2C. Note that while USART3 (right sideboard cable) is 5V tolerant, USART2 (left sideboard cable) is **not** 5V tolerant.

Typically, the mainboard brain is an [STM32F103RCT6](/docs/literature/[10]_STM32F103xC_datasheet.pdf), however some mainboards feature a [GD32F103RCT6](/docs/literature/[11]_GD32F103xx-Datasheet-Rev-2.7.pdf) which is also supported by this firmware.

For the reverse-engineered schematics of the mainboard, see [20150722_hoverboard_sch.pdf](/docs/20150722_hoverboard_sch.pdf)

 
---
## FOC Firmware
 
In this firmware 3 control types are available, it can be set in config.h file via CTRL_TYP_SEL parameter:
- Commutation (COM_CTRL)
- Sinusoidal (SIN_CTRL)
- Field Oriented Control (FOC_CTRL) with the following 3 control modes that can be set in config.h file with parameter CTRL_MOD_REQ:
  - **VOLTAGE MODE(VLT_MODE)**: in this mode the controller applies a constant Voltage to the motors. Recommended for robotics applications or applications where a fast motor response is required.
  - **SPEED MODE(SPD_MODE)**: in this mode a closed-loop controller realizes the input speed RPM target by rejecting any of the disturbance (resistive load) applied to the motor. Recommended for robotics applications or constant speed applications.
  - **TORQUE MODE(TRQ_MODE)**: in this mode the input torque target is realized. This mode enables motor "freewheeling" when the torque target is `0`. Recommended for most applications with a sitting human driver.

#### Comparison between different control methods

|Control method| Complexity | Efficiency | Smoothness | Field Weakening | Freewheeling | Standstill hold |
|--|--|--|--|--|--|--|
|Commutation| - | - | ++ | n.a. | n.a. | + |
|Sinusoidal| + | ++ | ++ | +++ | n.a. | + |
|FOC VOLTAGE| ++ | +++ | ++ | ++ | n.a. | +<sup>(2)</sup> |
|FOC SPEED| +++ | +++ | + | ++ | n.a. | +++ |
|FOC TORQUE| +++ | +++ | +++ | ++ | +++<sup>(1)</sup> | n.a<sup>(2)</sup> |

<sup>(1)</sup> By enabling `ELECTRIC_BRAKE_ENABLE` in `config.h`, the freewheeling amount can be adjusted using the `ELECTRIC_BRAKE_MAX` parameter.<br/>
<sup>(2)</sup> The standstill hold functionality can be forced by enabling `STANDSTILL_HOLD_ENABLE` in `config.h`. 

In all FOC control modes, the controller features maximum motor speed and maximum motor current protection. This brings great advantages to fulfil the needs of many robotic applications while maintaining safe operation.


### Field Weakening / Phase Advance

 - By default the Field weakening is disabled. You can enable it in config.h file by setting the FIELD_WEAK_ENA = 1 
 - The Field Weakening is a linear interpolation from 0 to FIELD_WEAK_MAX or PHASE_ADV_MAX (depeding if FOC or SIN is selected, respectively)
 - The Field Weakening starts engaging at FIELD_WEAK_LO and reaches the maximum value at FIELD_WEAK_HI
 - The figure below shows different possible calibrations for Field Weakening / Phase Advance
 ![Field Weakening](/docs/pictures/FieldWeakening.png)
 
 ⚠️ If you re-calibrate the Field Weakening please take all the safety measures! The motors can spin very fast!
 Power consumption will be highly increase and you can trigger the overvoltage protection of your BMS ⚠️


### Parameters
 - All the calibratable motor parameters can be found in the 'BLDC_controller_data.c'. I provided you with an already calibrated controller, but if you feel like fine tuning it feel free to do so 
 - The parameters are represented in Fixed-point data type for a more efficient code execution
 - For calibrating the fixed-point parameters use the [Fixed-Point Viewer](https://github.com/EFeru/FixedPointViewer) tool
 - The controller parameters are given in [this table](https://github.com/EFeru/bldc-motor-control-FOC/blob/master/02_Figures/paramTable.png)


### FOC Webview

To explore the controller without a Matlab/Simulink installation click on the link below:

[https://eferu.github.io/bldc-motor-control-FOC/](https://eferu.github.io/bldc-motor-control-FOC/)



- **Telegram Community:** If you are an enthusiast join our [Hooover Telegram Group](https://t.me/joinchat/BHWO_RKu2LT5ZxEkvUB8uw)


If you want to donate to keep this firmware updated, please use the link below:

[![paypal](https://www.paypalobjects.com/en_US/NL/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=CU2SWN2XV9SCY&currency_code=EUR&source=url)

---
