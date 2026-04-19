# RPL-UDP-ATTACKS

Repo này chứa các ví dụ về tấn công RPL bằng cách sử dụng UDP, cùng với các biện pháp phòng chống. 

## Các loại tấn công RPL được triển khai trong repo này:

- DIS Flooding Attack (Phân loại: Tấn công tài nguyên): Tấn công này làm ngập mạng với các gói DIS, khiến các nút phải liên tục cập nhật bảng định tuyến, dẫn đến tiêu tốn tài nguyên, năng lượng và giảm hiệu suất truyền tin.

- DAG Version Attack (Phân loại: Tấn công tài nguyên): Tấn công này liên tục làm tăng phiên bản DAG một cách giả mạo, khiến các nút phải liên tục cập nhật bảng định tuyến và tiêu tốn tài nguyên.

- Rank Decrease Attack (Phân loại: Tấn công traffic): Tấn công này làm hạ rank của một nút, hút traffic về nút đó. Thường khiến đường đi dữ liệu không tối ưu và có thể tạo vòng lặp, làm giảm hiệu suất truyền tin và tiêu tốn năng lượng. Rank attack cũng có thể được sử dụng để phục vụ cho việc nghe lén hoặc tấn công blackhole (sẽ nêu ở ngay dưới).

- Blackhole Attack (Phân loại: Tấn công topo/hình trạng mạng): Tấn công này làm cho một nút trở thành "hố đen" (nhận tất cả traffic nhưng không chuyển tiếp đi đâu). Tấn công này có thể cô lập các nút kết nối trực tiếp đến nó, làm mất dữ liệu và gấy gián đoạn dịch vụ.

## Các biện pháp phòng chống tương ứng được triển khai trong repo này:

- Với DIS Flooding Attack: Tính tỉ lệ (DIS + 1) / (Tổng số gói + 2) theo chu kỳ và so sánh với ngưỡng để xác định xem có đang bị tấn công hay không. Nếu tỉ lệ vượt quá ngưỡng, nút sẽ bỏ qua các gói DIS tiếp theo đến khi tỉ lệ giảm xuống dưới ngưỡng hoặc qua chu kỳ tiếp theo.
    - Trạng thái: Đã triển khai.

- Với DAG Version Attack: (Cân nhắc) Sử dụng VeRA (\[Ve\]rsion and \[R\]nk \[A\]uthentication).
    - Trạng thái: Ý tưởng khả thi. Chưa thực hiện.

- Với Rank Decrease Attack: N/A
    - Trạng thái: Chưa có ý tưởng.

- Với Blackhole Attack: Đặt ngưỡng số lượng gói tin bị drop
    - Trạng thái: Chưa có ý tưởng.

## Cấu trúc thư mục:

### Các thư mục chứa mã nguồn tấn công:
- no-rank-attack: Chứa mã nguồn cho các nút không được lập trình để tấn công rank, bao gồm các nút bình thường (root, server, client) và các nút tấn công khác (DIS flooding, DAG version).
- rank-attack: Chứa mã nguồn cho các nút được lập trình để tấn công rank. Cần tách riêng do sử dụng thử viện RPL được chỉnh sửa.

## Các thư mục chứa mã nguồn phòng chống:
- prevent-dis: Chứa mã nguồn cho các nút được lập trình để phòng chống DIS flooding attack.
- prevent-dagver: Chứa mã nguồn cho các nút được lập trình để phòng chống DAG version attack. (Hiện chưa có)
- prevent-rank: Chứa mã nguồn cho các nút được lập trình để phòng chống rank decrease attack. (Hiện chưa có)
- prevent-blackhole: Chứa mã nguồn cho các nút được lập trình để phòng chống blackhole attack. (Hiện chưa có)

## Cách sử dụng repo này:
1. Clone repo này về máy.
2. Mở các thư mục con tưởng ứng, mở Makefile và sửa giá trị CONTIKI để trỏ đến thư mục Contiki-NG trên máy của bạn.