/**
 * @file      : user_gpio.h
 * @brief     : 用户GPIO头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-02 19:27:35
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-02 huenrong        创建文件
 *
 */

#ifndef __USER_GPIO_H
#define __USER_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "../linux_gpio/gpio.h"

// GPIO引脚定义
// LED GPIO引脚
#define GPIO_LED 3
// BEEP GPIO引脚
#define GPIO_BEEP 129
// KEY GPIO引脚
#define GPIO_KEY 18

// LED状态控制
#define LED_ON() gpio_set_value(GPIO_LED, E_GPIO_LOW)
#define LED_OFF() gpio_set_value(GPIO_LED, E_GPIO_HIGH)

// BEEP状态控制
#define BEEP_ON() gpio_set_value(GPIO_BEEP, E_GPIO_LOW)
#define BEEP_OFF() gpio_set_value(GPIO_BEEP, E_GPIO_HIGH)

/**
 * @brief  初始化LED
 * @return true : 成功
 * @return false: 失败
 */
bool led_init(void);

/**
 * @brief  初始化蜂鸣器
 * @return true : 成功
 * @return false: 失败
 */
bool beep_init(void);

/**
 * @brief  初始化按键
 * @return true : 成功
 * @return false: 失败
 */
bool key_init(void);

/**
 * @brief  初始化按键(使用poll方式读取键值)
 * @return true : 成功
 * @return false: 失败
 */
bool key_init_with_poll(void);

/**
 * @brief  读取按键键值
 * @param  gpio_value: 输出参数, 按键键值
 * @return true : 成功
 * @return false: 失败
 */
bool key_read_value(gpio_value_e *gpio_value);

/**
 * @brief  poll方式阻塞读取按键键值
 * @param  gpio_value: 输出参数, 按键键值
 * @param  timeout_ms: 输入参数, 超时时间(单位: ms)
 * @return 成功: 0
 *         失败: -1
 *         超时: -2
 */
int key_read_value_with_poll(gpio_value_e *gpio_value, const int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // __USER_GPIO_H
