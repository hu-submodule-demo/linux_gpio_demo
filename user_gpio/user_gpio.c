/**
 * @file      : user_gpio.c
 * @brief     : 用户GPIO源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-02 19:27:58
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-02 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#include "./user_gpio.h"

// 按键文件描述符
static int g_key_fd = -1;

/**
 * @brief  初始化LED
 * @return true : 成功
 * @return false: 失败
 */
bool led_init(void)
{
    // 导出GPIO
    if (!gpio_export(GPIO_LED))
    {
        return false;
    }

    // 设置GPIO方向
    if (!gpio_set_direction(GPIO_LED, E_GPIO_OUT))
    {
        return false;
    }

    LED_OFF();

    return true;
}

/**
 * @brief  初始化蜂鸣器
 * @return true : 成功
 * @return false: 失败
 */
bool beep_init(void)
{
    // 导出GPIO
    if (!gpio_export(GPIO_BEEP))
    {
        return false;
    }

    // 设置GPIO方向
    if (!gpio_set_direction(GPIO_BEEP, E_GPIO_OUT))
    {
        return false;
    }

    BEEP_OFF();

    return true;
}

/**
 * @brief  初始化按键
 * @return true : 成功
 * @return false: 失败
 */
bool key_init(void)
{
    // 导出GPIO
    if (!gpio_export(GPIO_KEY))
    {
        return false;
    }

    // 设置GPIO方向
    if (!gpio_set_direction(GPIO_KEY, E_GPIO_IN))
    {
        return false;
    }

    return true;
}

/**
 * @brief  初始化按键(使用poll方式读取键值)
 * @return true : 成功
 * @return false: 失败
 */
bool key_init_with_poll(void)
{
    // 导出GPIO
    if (!gpio_export(GPIO_KEY))
    {
        return false;
    }

    // 设置GPIO方向
    if (!gpio_set_direction(GPIO_KEY, E_GPIO_IN))
    {
        return false;
    }

    // 设置GPIO边沿
    if (!gpio_set_edge(GPIO_KEY, E_GPIO_BOTH))
    {
        return false;
    }

    // 打开设备
    g_key_fd = gpio_open(GPIO_KEY);
    if (-1 == g_key_fd)
    {
        return false;
    }

    return true;
}

/**
 * @brief  读取按键键值
 * @param  gpio_value: 输出参数, 按键键值
 * @return true : 成功
 * @return false: 失败
 */
bool key_read_value(gpio_value_e *gpio_value)
{
    if (!gpio_value)
    {
        return false;
    }

    return gpio_get_value(gpio_value, GPIO_KEY);
}

/**
 * @brief  poll方式阻塞读取按键键值
 * @param  gpio_value: 输出参数, 按键键值
 * @param  timeout_ms: 输入参数, 超时时间(单位: ms)
 * @return 成功: 0
 *         失败: -1
 *         超时: -2
 */
int key_read_value_with_poll(gpio_value_e *gpio_value, const int timeout_ms)
{
    int ret = -1;
    // 指定fds数组中的项目数
    nfds_t nfds = 1;
    // 指定要监视的文件描述符集
    struct pollfd fds[1] = {0};
    // 读取到的GPIO值
    char gpio_buf[2] = {0};

    // 设置需要监听的文件描述符
    // 监听gpio时, events需要使用POLLPRI(表示有紧急数据需要读取), 不可使用POLLIN
    memset(fds, 0, sizeof(fds));
    fds[0].fd = g_key_fd;
    fds[0].events = POLLPRI;

    ret = poll(fds, nfds, timeout_ms);
    // 发生错误
    if (ret < 0)
    {
        return -1;
    }
    // 超时
    else if (0 == ret)
    {
        return -2;
    }
    // 成功
    else
    {
        // 是期望的返回
        if (fds[0].revents & POLLPRI)
        {
            memset(gpio_buf, 0, sizeof(gpio_buf));

            // 从文件起始位置开始读数据
            lseek(fds[0].fd, 0, SEEK_SET);
            ret = read(fds[0].fd, gpio_buf, 1);
            if (1 == ret)
            {
                if (0 == memcmp("0", gpio_buf, 1))
                {
                    *gpio_value = E_GPIO_LOW;
                }
                else if (0 == memcmp("1", gpio_buf, 1))
                {
                    *gpio_value = E_GPIO_HIGH;
                }
                else
                {
                    return -1;
                }

                return 0;
            }
        }
    }

    return -1;
}
