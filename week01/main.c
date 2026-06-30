// HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP

#include <stdio.h>
#include <stdint.h>

// TASK 1: POINTERS & MEMORY

void parse_config(const uint8_t *config_packet, int16_t *high_threshold) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // check if config_packet is null
    if (!config_packet) {
        *high_threshold = 0;
        return;
    }
    
    // check if high_threshold is null
    if (!high_threshold) return;

    // concatenate value
    *high_threshold = (int16_t)(*config_packet | (*(config_packet + 1) << 8));
    
    return;

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    
    // check if hw_sensor reg is null
    if (!hw_sensor_reg) return 0;

    // return volatile casted value
    return *(volatile int16_t*)hw_sensor_reg;

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    
    // check if control_reg if null
    if (!control_reg) return;

    // change bit 0 base on fan_enable
    if (fan_enable) *control_reg |= (1 << 0);
    else *control_reg &= ~(1 << 0);
    
    // change bit 1 base on alarm_enable
    if (alarm_enable) *control_reg |= (1 << 1);
    else *control_reg &= ~(1 << 1);

    return;
    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm, chỉ thay đổi input nếu cần)

int main() {

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); // Có thể thay mock_packet thành NULL để test chống crash khi gặp NULL
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105; 
    int16_t current_temp = read_temperature_reg((void*)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold) {
        // Yêu cầu: fan = 1, alarm = 0
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}