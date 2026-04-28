# prevent-dis

Phòng chống tấn công DIS flooding bằng cách tính tỉ lệ gói DIS so với tổng số gói và so sánh với ngưỡng. Nếu tỉ lệ vượt quá ngưỡng, nút sẽ bỏ qua các gói DIS tiếp theo đến khi tỉ lệ giảm xuống dưới ngưỡng hoặc qua chu kỳ tiếp theo. Việc đếm tỉ lệ được tạo thành một process riêng chạy định kỳ để không bị tiến trình chính làm ảnh hưởng.

