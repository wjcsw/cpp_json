# cpp_json

本项目是基于 [json-tutorial](https://github.com/miloyip/json-tutorial) 教程 个人学习后开发的c++版本  


## 项目特点  
- 通过cmake构建跨平台使用的代码  
- 相较于原教程使用指针管理内存的方法，本项目使用C++的容器来管理动态内存，在可读性和安全性上都更高  
- 本项目相较于原项目在某些处理上有所不同  
  - **在字符串处理上**：本项目由于使用内置的string而不解析\0字符  
  - **在数字溢出方面**：本项目采用与C++本身一致的处理机制，对于溢出的数字，令其为对应的极限值，不进行报错  
  - 本项目使用标准库的 map 容器实现了JSON_OBJECT，因此可以方便的直接调用map的函数来**获取键值情况**、**查找key**等  
  - 使用 vector 容器实现了JSON_ARRAY 进行**动态内存的管理**  
  - 同时 本项目还通过重载运算符的方法实现了**错误情况打印**, **JSON对象打印内容**和**JSON对象拷贝构造**，**拷贝赋值运算符**，以及直接使用运算符进行JSON对象的**比较运算**  
  - 定义了JSON对象的swap操作

## 项目结构
