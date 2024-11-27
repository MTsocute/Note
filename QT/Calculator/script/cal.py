import os
import sys


def py_cal(something=""):
    print(f"Expression: {something}")
    return eval(something)


if __name__ == "__main__":
    # 获取外部传入的参数
    param_from_out_side = str(sys.argv[1])
    # 调用函数并打印返回值到标准输出
    result = py_cal(param_from_out_side)
    print(result)
