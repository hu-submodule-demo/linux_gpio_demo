## linux_gpio_demo

### 介绍

该仓库代码是[ARM-Linux平台下的GPIO驱动](https://github.com/hu-submodule-1/linux_gpio)的测试代码

### 使用说明

输出GPIO使用时, 具体可参考代码中的LED和BEEP. 大体步骤如下:
1. 导出GPIO
2. 设置GPIO方向(设置为输出)

作为输入GPIO使用时, 具体可参考代码中的KEY. 推荐使用poll方式读取按键键值, 大体步骤如下:
1. 导出GPIO
2. 设置GPIO方向(设置为输入)
3. 设置GPIO边沿
4. 打开GPIO设备
5. 使用poll函数读取GPIO值(参考函数: read_key_value_with_poll)
