# CH32V003 Project Template for PlatformIO (ch32fun)

Một dự án mẫu (Template) gọn nhẹ, kiến trúc module hóa độc lập hoàn toàn dành cho dòng vi điều khiển RISC-V siêu rẻ
**CH32V003**. Phát triển dựa trên framework **ch32fun**, tối ưu sẵn cho **PlatformIO** và tự động liên kết mượt mà với
các thư viện độc lập từ kho tập trung bên ngoài.

---

## 🚀 Tính năng nổi bật

- **Kiến trúc tách biệt hoàn toàn (Decoupled)**: Mã nguồn ứng dụng (Repo template này) độc lập 100% với các driver phần
  cứng (Repo thư viện chung). Bạn chỉ cần sửa driver một nơi, tất cả các dự án khác sẽ tự động cập nhật ngay trong lần
  Build tiếp theo.
- **Tự động nhận diện cấu hình (`__has_include`)**: Các driver phần cứng trong kho chung sẽ tự động quét và kiểm tra xem
  dự án hiện tại có file `sys_config.h` và `funconfig.h` hay không để tự cấu hình trước khi biên dịch. Hoàn toàn không
  lo lỗi biên dịch nếu thiếu file cấu hình.
- **Tự động ánh xạ chân theo gói vỏ (Pinout)**: Hệ thống tự động chuyển đổi bảng sơ đồ chân vật lý thực tế (`MCU_PIN1`,
  `MCU_PIN3`,...) linh hoạt theo môi trường (`env`) chip bạn chọn trong file cấu hình (`SOP8`, `SOP16`, hoặc `TSSOP20`)
  nhờ cờ `-DCH32V003_...`.
- **Hỗ trợ Full API chuẩn phong cách Arduino**: Cho phép điều khiển GPIO cực kỳ trực quan thông qua các hàm quen thuộc
  (`pinMode`, `digitalWrite`, `digitalRead`, `digitalToggle`) truyền thẳng mã chân vật lý nhờ cơ chế giải mã Bit-packing
  (`decodeHardwarePin`) chạy ngầm siêu tốc.
- **Nạp code và Monitor chỉ với 1 cú click**: Tận dụng script Python custom chạy ngầm. Chỉ cần gọi một lệnh duy nhất là
  hệ thống tự động nạp chương trình xuống chip và mở ngay cửa sổ gỡ lỗi `printf` tốc độ cao qua chân nạp SWIO (1-wire).

---

## 📂 Cấu trúc thư mục dự án

```text
├── .pio/                         # Thư mục build nội bộ của PlatformIO (Tự động sinh ra)
├── include/                      # Nơi chứa các file cấu hình tầng ứng dụng (Application)
│   ├── funconfig.h               # Cấu hình phần cứng gốc framework (Nguồn xung nhịp, bật/tắt SWIO printf)
│   ├── sys_config.h              # Cấu hình tính năng driver (Bật/tắt module thư viện, chỉnh kích thước buffer)
│   └── user_config.h             # Nơi người dùng tự định nghĩa macro và chân chức năng riêng cho ứng dụng
├── src/                          # Mã nguồn chính của ứng dụng
│   └── main.c                    # File code chính (Tổ chức theo cấu trúc setup/loop quen thuộc)
├── ch32v003_extra_scripts.py     # Script Python tự động gộp tác vụ Upload + SDI Monitor
├── platformio.ini                # Cấu hình môi trường chip (SOP8, SOP16, TSSOP20) & liên kết thư viện ngoài
├── LICENSE                       # Bản quyền mã nguồn (MIT License)
└── README.md                     # Tài liệu hướng dẫn này
```

> 💡 *Lưu ý: Toàn bộ các driver phần cứng độc lập (`ch32v003_gpio`, `ch32v003_delay`, `ch32v003_ir`,...) sẽ nằm ở thư mục
quản lý tập trung bên ngoài (Ví dụ: `../../libraries/ch32v003/`) và được PlatformIO tự tìm kiếm, nạp vào dự án thông qua
cơ chế LDF khi biên dịch.*

---

## 🛠️ Hướng dẫn bắt đầu

### Yêu cầu hệ thống

1. Cài đặt **VS Code** kèm extension **PlatformIO IDE** (hoặc sử dụng **CLion / Neovim** kết hợp với `clangd`).
2. Clone kho lưu trữ thư viện dùng chung của bạn về máy. Đảm bảo đường dẫn thực tế trùng khớp với khai báo
   `lib_extra_dirs` trong file `platformio.ini` (Mặc định cấu hình đang đi ngược ra 2 tầng:
   `../../libraries/ch32v003/`).
3. Kết nối mạch nạp **WCH-LinkE** với chân nạp của chip CH32V003.

### Cấu hình dự án

- **`include/funconfig.h`**: Bật/tắt các tính năng phần cứng của framework gốc (Ví dụ: đặt `FUNCONF_USE_DEBUGPRINTF 1`
  để in log debug trực tiếp qua chân nạp SWIO).
- **`include/sys_config.h`**: Bật/tắt các module tính năng của thư viện (Ví dụ: `DELAY_ARDUINO_STYLE_ENABLE`,
  `IR_SUPPORT_NEC`) để tối ưu hóa, tiết kiệm dung lượng Flash/RAM một cách triệt để.
- **`platformio.ini`**: Thay đổi thông số `default_envs` để khớp với kiểu đóng gói con chip thực tế bạn đang cắm trên
  mạch (`CH32V003J4M6` cho bản SOP8, `CH32V003A4M6` cho bản SOP16, hoặc `CH32V003F4P6` cho bản TSSOP20).

---

## 📝 Code mẫu triển khai (`src/main.c`)

```c
#include "ch32fun.h"
#include <ch32v003_delay.h>
#include <ch32v003_gpio.h>

// Định nghĩa chân LED dựa trên số thứ tự chân vật lý của IC thực tế
// Sơ đồ chân vật lý sẽ tự động thay đổi theo môi trường cấu hình trong platformio.ini
#if defined(CH32V003_J4M6)
  #define LED_PIN  MCU_PIN3 // Bản SOP8: Chọn chân vật lý số 3 (PA2)
#elif defined(CH32V003_A4M6)
  #define LED_PIN  MCU_PIN1 // Bản SOP16: Chọn chân vật lý số 1 (PC1)
#else
  #define LED_PIN  MCU_PIN8 // Bản TSSOP20: Chọn chân vật lý số 8 (PD0)
#endif

static void setup()
{
    // Gọi hàm cấu hình chân theo phong cách Arduino từ thư viện ch32v003_gpio
    // Hệ thống tự động giải mã chân thông qua hàm decodeHardwarePin chạy ngầm
    pinMode(LED_PIN, OUTPUT); 
    printf("Vọc Vạch IoT: Hệ thống đã khởi tạo xong!\n");
}

static void loop()
{
    // Ghi trạng thái logic sử dụng bộ enum HIGH/LOW chuẩn xác của thư viện
    digitalWrite(LED_PIN, HIGH);
    delayMs(500); // Hàm delay chính xác tuyệt đối tính theo chu kỳ thạch anh
    
    digitalWrite(LED_PIN, LOW);
    delayMs(500);
    
    // Hoặc có thể sử dụng hàm đảo trạng thái nhanh gọn:
    // digitalToggle(LED_PIN);
    // delayMs(500);
}

int main()
{
    SystemInit();  // Hàm khởi tạo phần cứng gốc của framework ch32fun
    delayMs(1000); // Khoảng trống an toàn tránh brick chip, giúp mạch nạp dễ dàng can thiệp
    
    setup();
    while (1)
    {
        loop();
    }
}
```

---

## 🛠️ Quy trình biên dịch & Nạp chương trình nâng cao

Thay vì phải bấm nút Biên dịch, nút Nạp, rồi mở Terminal Monitor thủ công bằng nhiều thao tác, bạn hãy sử dụng Target
Custom đã được gộp sẵn:

1. **Biên dịch & Nạp tiêu chuẩn**: Sử dụng các công cụ mặc định trên thanh status bar của PlatformIO.
2. **Combo Tự động Nạp và Bật Monitor SDI**:
    - Mở Terminal tại thư mục dự án và chạy lệnh:
      ```bash
      pio run -t upload_and_sdi_monitor
      ```
    - *Kết quả*: PlatformIO tự động quét thay đổi code, liên kết các driver từ kho chung, biên dịch, nạp xuống chip qua
      `minichlink`, và ngay lập tức biến terminal đó thành luồng đọc dữ liệu debug `printf` gửi ngược từ chân SWIO lên
      máy tính.

### 💡 Mẹo nhỏ cho LSP (Neovim / CLion Auto-complete)

Nếu bạn vừa thêm driver mới hoặc chỉnh sửa file cấu hình trong kho chung mà trình gợi ý code (`clangd`) của IDE chưa
nhận ngay lập tức (bị báo đỏ giả lập), hãy chạy lệnh sau dưới terminal dự án để ép sinh lại cơ sở dữ liệu biên dịch:

```bash
pio run -t compiledb
```

---

## 📜 Bản quyền (License)

Dự án này tuân thủ theo các điều khoản của **MIT License** - xem chi tiết tại file [LICENSE](LICENSE).

## 🤝 Lời cảm ơn

- Xin gửi lời cảm ơn đặc biệt đến Charles Lohr ([@cnlohr](https://github.com)) cùng cộng đồng vì đã phát triển framework
  **ch32fun** siêu nhẹ và tuyệt vời này.
- Hệ thống module hóa và kiến trúc quản lý tập trung được cấu trúc và duy trì bởi **Vọc Vạch IoT**.
