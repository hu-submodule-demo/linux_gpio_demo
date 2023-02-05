/**
 * @file      : main.c
 * @brief     : 程序入口文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-02 19:25:41
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-02 huenrong        创建文件
 *
 */

#include <stdio.h>

#include "./user_gpio/user_gpio.h"

/**
 * @brief  : 程序入口
 * @param  : argc: 输入参数, 参数个数
 * @param  : argv: 输入参数, 参数列表
 * @return : 成功: 0
 *           失败: 其它
 */
int main(int argc, char *argv[])
{
    if (!led_init())
    {
        perror("init led fail");

        return -1;
    }

    if (!beep_init())
    {
        perror("init beep fail");

        return false;
    }

    if (!key_init_with_poll())
    {
        perror("init key fail");

        return false;
    }

    while (true)
    {
        gpio_value_e key_value;

        if (0 == key_read_value_with_poll(&key_value, 1000))
        {
            // 按键按下
            if (E_GPIO_LOW == key_value)
            {
                LED_ON();
                BEEP_ON();
            }
            // 按键释放
            else
            {
                LED_OFF();
                BEEP_OFF();
            }
        }
    }

    return 0;
}
