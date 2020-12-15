一、本次作业测试方法
测试方法：执行下面的命令
./test.sh <到semant的路径>
得到的提示如

--------Test using continue-break.seal --------
passed
--------Test using dup-funcdec.seal --------
5: Function ifstmt was previously defined.
9: Main function should not have any parameters.
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!
--------Test using generator.seal --------
passed
--------Test using globals.seal --------
passed
--------Test using if-while.seal --------
passed
--------Test using local-variables.seal --------
13: Cannot use xor(^) between Float and Float.
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!
--------Test using main.seal --------
Main function is not defined.
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!
--------Test using must-return.seal --------
1: Function main must have an overall return statement.
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!
--------Test using parameters.seal --------
10: Function test1, the 1 parameter should be Int but provided a Bool.
11: Function test1 called with wrong number of arguments.
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!
--------Test using type.seal --------
8: Cannot minus a Int by a Bool.
9: Cannot use bit-and(&) between Bool and Bool.
10: Cannot add a String and a Int.
20: Cannot compare a Int and a Bool(less).
21: Cannot compare a Int and a Bool(equal).
22: Cannot compare a Int and a Bool(less than or equal).
23: Cannot compare a Bool and a Int(greater than or equal).
24: Cannot compare a Bool and a Bool(greater).
Compilation halted due to static semantic errors.
DIFFERENCE OCCURED!

二、本次作业批阅标准
每个输出结果一致按照1分。

三、其他说明
1.请以后的作业务必不要修改文件名称，这次仍然有若干同学存在乱码文件名，请上传后
下载下来看看是不是乱码
2.请以后作业上传按照格式，即所有代码放在一个名字为【学号】的文件夹内，然后打包上传，请不要嵌套多层以及放其他各种不需要的文件，作业要求什么的不用放置
3.谢谢！