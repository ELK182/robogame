# RG2024-组委会嵌入式例程
test
作者: Tang-yucheng (QQ: 3143961287)

## 代码架构

本例程采用嵌入式分层代码架构，以便于代码维护与移植，建议各队伍亦使用类似架构进行参赛机器人嵌入式代码编写。\
下面将由高到低依次介绍各层功能。

- **MIL 中间件层 (Middleware Layer)**

        一些高层次（脱离硬件）可复用组件。（网上对于中间件的理解有很多，此仅为作者个人理解）
        e.g. 算法实现，数据结构实现...

- **APL 应用层 (Application Layer)**

        实现程序执行逻辑。  
        e.g. 主函数实现（初始化、循环），回调函数实现。

- **FML 功能模块层 (Function Module Layer)**

        通过调用 HDL、HAL 层封装的外设接口，组合成对应功能模块，实现相应功能。   
        e.g. 底盘功能模块实现，通讯功能模块实现。

- **HDL 硬件驱动层 (Hardware Driver Layer)**

        通过调用 HAL 层封装片上外设接口，实现对片外外设驱动。  
        e.g. 板级支持包实现，电机驱动实现...

        - IMU (惯导模块驱动)
        - Motor (电机驱动)

- **HAL 硬件抽象层 (Hardware Abstract Layer)**

        实现对于片上外设再次封装。  
        e.g. CAN外设封装，USART外设封装...

## 代码仓库介绍

下面将介绍此代码仓库的内容和使用方法。

### Communication.x

#### 文件内容

此文件为用户自定义定长数据包串口通讯实现，可支持自定义包头，包数据类型，包数据。

包结构：

| 包头 | 包类型 | 数据 | CRC校验位 |
|:--------:|:-------:|:------:|:-------:|
| uint32_t | uint8_t | void * | uint8_t |

e.g. 例程中实际向鲁班猫上位机发送数据包整体为：

```c++
/* 包头 */
uint32_t Pack_Head;                 /*!< 包头 (4byte) */

/* 包类型 */
uint8_t Pack_Type;                  /*!< 包类型（例程中规定 上行: 0x0x 下行: 0xFx） */

/* 数据 */
Enum_ChassisState Chassis_State;    /*!< 底盘设定状态 */
float Chassis_Vel_X;                /*!< 底盘X轴速度 (m/s) */
float Chassis_Vel_Y;                /*!< 底盘Y轴速度 (m/s) */
float Chassis_Omega;                /*!< 底盘旋转角速度 (rad/s) */

/* CRC校验位 */
uint8_t CRC_8;                      /*!< 整包CRC校验（例程中采用CRC8-ROHC校验） */
```

#### 使用方法

1. 定义 Tx、Rx 包数据结构体。（注意：考虑是否使用 `__packed` 关键字解除结构体内部对齐）
2. **实现 Tx、Rx 回调函数，进行包 Tx、Rx 回调处理。（用户逻辑均在此实现）**
3. 例化 `Class_CustomCOM` 类对象，在构造函数中绑定 `Tx回调函数`，`Rx回调函数`，`UART处理对象`。
4. 调用 `Init()` 函数初始化此对象。
5. 在HAL库中断处理回调函数 `HAL_UARTEx_RxEventCallback()` 中，该UART外设下，调用 `DataProcess()` 函数进行Rx数据解析。（注意：别忘了开启新一次串口接收 DMA-IDLE ）
6. 可调用 `DataSend()` 函数进行数据发送。

### Motor.x

#### 文件内容

此文件为电机驱动实现，可实现对直流有刷电机（BDC）、步进电机（Step）、舵机（Servo）的驱动。\
其中直流有刷电机驱动内置斜坡变速算法，平滑变速，避免过流烧毁驱动板。

接口函数参数均为国际单位制。

#### 使用方法

- 直流有刷电机（BDC）

    1. 例化 `Class_Motor_BDC` 类对象。
    2. 调用 `Init()` 函数初始化 BDC 电机对象。（进行硬件绑定，参数配置）
    3. 调用 BDC 电机对象中 `Class_PID` 类 `Init()` 函数初始化角速度 PID 对象。
    4. 调用 BDC 电机对象中 `Class_Gear_Slope` 类 `Init()` 函数初始化斜坡变速对象。
    5. 在HAL库中断处理回调函数 `HAL_TIM_PeriodElapsedCallback` 中，系统心跳TIM外设下（例程中为TIM6），调用 `Control()` 函数进行电机控制。（注意：修改系统心跳定时器频率后，要记得修改类中常量 `Heartbeat_Period`）
    6. 可调用 `MotionSet()`, `StopSet()` 函数进行电机启停控制。

- 步进电机（Step）

    1. 

- 舵机（Servo）

    1. 例化 `Class_Motor_Servo` 类对象。
    2. 调用 `Init()` 函数初始化 Servo 电机对象。（进行硬件绑定，参数配置）
    3. 可调用 `AngleSet()` 函数进行舵机相对角度控制。

## 调试工具
1. Keil-Debug
2. CubeMonitor 使用方法请参阅 [CubeMonitor入门教程](https://shequ.stmicroelectronics.cn/thread-626119-1-1.html)，[CubeMonitor社区](https://community.st.com/t5/stm32cubemonitor-mcus/bd-p/stm32-mcu-cubemonitor-forum)