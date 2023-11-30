Họ và tên: Nguyễn Trọng Trí
Mã số sinh viên: 21120344

# Project Paint

# Các chức năng đã làm được:

## Technical details
- Design patterns: Singleton, Factory, Abstract factory, prototype

# Core requirements
1. Dynamically load all graphic objects that can be drawn from external DLL files
2. The user can choose which object to draw
3. The user can see the preview of the object they want to draw
4. The user can finish the drawing preview and their change becomes permanent with previously drawn objects
5. The list of drawn objects can be saved and loaded again for continuing later
6. Save and load all drawn objects as an image in bmp/png/jpg format (rasterization)

# Basic graphic objects
1. Line: controlled by two points, the starting point, and the endpoint
2. Rectangle: controlled by two points, the left top point, and the right bottom point
3. Ellipse: controlled by two points, the left top point, and the right bottom point
4. Square: controlled by two points, the left top point, and the right bottom point
5. Circle: controlled by two points, the left top point, and the right bottom point

# Improvements
1. Bonus shapes: square, circle.
2. "New File" button to quickly start over, the application will of course ask the user for confirmation if any new changes is detected
3. Objects in drawable list have their own icons instead of plain text
4. Allow the user to change the color, pen width, and stroke type (dash, dot, dash dot,...)
5. Adding image to the canvas
6. Undo and Redo
7. Used Fluent.Ribbon to obtain MS Paint-like user interface
8. Select a single shape to move around, rotate, resize, and delete
9. Select a single shape to copy and paste
10. Bulk interaction: hold control key to select multiple shapes to move, copy, paste, or delete

# Hướng dẫn sử dụng:
- Chạy file Paint.exe trong thư mục Release

# Một số lưu ý:
- Thư mục "shapes" sẽ chứa tất cả các hình dạng cho chương trình chạy. Nếu muốn thêm hình dạng thì copy file .dll của hình dạng đó và dán vào thư mục này, tương tự nếu muốn xóa hình dạng thì xóa file .dll của hình dạng đó trong thư mục này
- Thư mục "shapes" có thể để rỗng hoăc không tồn tại, tuy nhiên nếu như vậy thì chương trình không vẽ được gì cả

# Tự đánh giá: 10 điểm
