#include <Python.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string scriptPath = "D:/Code/QT/Calculator/script/";

void config_path(const std::string& scriptPath) {
    if (fs::exists(scriptPath)) {
        // 动态添加路径到 sys.path
        std::string customPath = scriptPath;
        std::string pythonCode = "import sys; sys.path.append('" + customPath + "')";
        PyRun_SimpleString(pythonCode.c_str());
    }
    else { std::cout << "File Path doesn't exist" << std::endl; }
}


int main() {
    // 初始化 Python 解释器
    Py_Initialize();

    // 检查 Python 是否初始化成功
    if (!Py_IsInitialized()) {
        std::cerr << "Python 初始化失败！" << std::endl;
        return -1;
    }

    try {
        // 动态添加路径到 sys.path
        config_path(scriptPath);

        // 导入 Python 模块 (cal.py)
        PyObject* pModule = PyImport_ImportModule("cal");
        if (!pModule) {
            std::cerr << "Unable to load 'cal'" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
            return -1;
        }

        // 获取 py_cal 函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "py_cal");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            std::cerr << "'py_cal' unable to use" << std::endl;
            Py_XDECREF(pModule);
            return -1;
        }

        std::string expr = "4 * 2";
        // 构造参数
        PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString(expr.c_str()));

        // 调用函数，并获取返回值
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        // 检查返回值并获取结果
        if (pValue != nullptr) {
            long result = PyLong_AsLong(pValue);
            std::cout << "Result: " << result << std::endl;
            Py_DECREF(pValue);
        } else {
            std::cerr << "Function failed to use！" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
        }

        // 释放资源
        Py_XDECREF(pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);

    } catch (...) {
        std::cerr << "Error！" << std::endl;
    }

    // 关闭 Python 解释器
    Py_Finalize();
    return 0;
}
