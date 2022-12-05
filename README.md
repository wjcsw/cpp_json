# cpp_json

本项目是基于 [json-tutorial](https://github.com/miloyip/json-tutorial) 教程 个人学习后开发的c++版本 

通过cmake构建跨平台使用的代码

相较于原教程使用指针管理内存的方法  本项目使用C++的容器来管理动态内存  在可读性和安全性上都更高  但是牺牲了一定的性能

同时 本项目还通过重载运算符的方法实现了错误情况打印, JSON对象打印内容 和 JSON对象拷贝构造，拷贝赋值运算符，以及直接使用运算符进行JSON对象的比较运算

并且定义了JSON对象的swap操作