#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "Serial.h"


uint16_t ADValue;            // 定义AD值变量
float Voltage;               // 定义电压变量

// 均值滤波函数（10次采样）
uint16_t AD_GetValue_Filtered(void) {
    uint16_t sum = 0;
    for (uint8_t i = 0; i < 10; i++) {
        sum += AD_GetValue();  // 累计10次采样值
        Delay_ms(1);           // 间隔1ms采样（根据实际情况调整）
    }
    return sum / 10;          // 返回平均值
}

int main(void) {
    /*模块初始化*/
    OLED_Init();            // OLED初始化
    AD_Init();              // AD初始化
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "ADValue:");
    OLED_ShowString(2, 1, "Voltage:0.00V");
    OLED_ShowString(3, 1, "mA:0.00mA");
    
    while (1) {
        ADValue = AD_GetValue_Filtered();           // 使用滤波后的AD值
        Voltage = (float)ADValue / 4095 * 3.3;      // 转换为电压值
        
        // 显示AD值
        OLED_ShowNum(1, 9, ADValue, 4);
        
        // 显示电压值（整数部分+两位小数）
        OLED_ShowNum(2, 9, (uint16_t)Voltage, 1);
        OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
        
        // 计算并显示电流值（假设977是转换系数）
        float current = Voltage / 977 * 1000;       // 转换为电流（mA）
        OLED_ShowNum(3, 4, (uint16_t)current, 1);   // 整数部分
        OLED_ShowNum(3, 6, (uint16_t)(current * 100) % 100, 2); // 小数部分
        
        Delay_ms(100);      // 主循环延时
    }
}