/**
 * @file sys_config.h
 * @author Vọc Vạch IoT
 * @brief Tệp cấu hình hệ thống, quản lý kích hoạt các tính năng và tối ưu bộ
 * nhớ Flash cho CH32V003.
 * @version 1.1
 * @date 2026-05-26
 */

#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

// ============================================================================
// CẤU HÌNH CHO TÍNH NĂNG HỒNG NGOẠO (IR) - Hỗ trợ thu phát đa giao thức NEC,
// Sony, Samsung
// ============================================================================
#define IR_SUPPORT_NEC 1
#define IR_SUPPORT_SONY 1
#define IR_SUPPORT_SAMSUNG 1
#define IR_SUPPORT_TCL 1
// Cấu hình các thông số
#define IR_RAW_BUFFER_SIZE                                                              \
  256 /**< Kích thước bộ đệm lưu trữ dữ liệu xung thô khi thu tín hiệu IR (150 \
         xung) */
#define IR_TIMEOUT_US 15000 /**< Thời gian chờ tối đa chặn treo chip (15ms) */
#define IR_TOLERANCE_PERCENT                                                   \
  25 /**< Dung sai cho phép khi so khớp thời gian xung (25%) */

#endif
