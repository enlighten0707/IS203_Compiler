## Syntax
```
make parser
./parser test/test1.seal
```
- [x] 在make之后查看生成的seal.output，不能有冲突
- [x] 优先级中，单目取负
- [x] break/continue是否写反？
- [x] 是否能通过测试样例
- [x] 根据文法检查代码是否有遗漏，以及代码里的typo
- [x] 对比语法手册其他对语法的说明细节以及样例
- [x] 对比一下参考代码
- [x] 构造样例里没有出现的情况
- [x] 附带代码包里的说明

  
```
对比参考代码
1 y
2 y
3 y
4 y
5 y
6 y
7,8 这里写法主要借鉴了seal-3，空语句
9 y
10 y
11 y
12 y
13 y
14 y
15 y
16,17 这里根据参考代码改动了一下，但和参考代码不完全相同，因为参考代码有冲突
```

## Semant
### 命令
```
构建语义分析器:
make semant

测试某个样例
./semant < test.seal

清理临时文件
make clean

测试样例
./judge.sh
bash judge.sh
```
### 考察点
- [x] 是否有`main`函数
- [x] `main`函数返回值为Void
- [x] `main`函数没有参数
  
- [x] 没有重复的函数声明
  
- [x] `printf`不可重定义
- [x] `printf`必须有至少一个参数，且第一个参数类型必须为`String`
  
- [x] 返回值仅限于`Int` `Void` `String` `Float` 和`Bool`（自然地）
- [x] `return`语句返回的类型和声明的一致
- [x] 每一个函数声明在最外层逻辑结束前（即函数体的最外层右大括号闭合前）一定要有一个`return`语句，重要！！
  
- [x] 形参没有重复命名的
- [x] 形参类型不可以为`Void`
  
- [x] 全局变量没有重复命名的 <局部变量也不可以重复命名>
- [x] 变量类型不可以为`Void` <函数形参，全局变量声明，函数体中变量声明>
- [x] 局部变量一定先声明再使用,Assign class
  
- [x] install_calls函数将预先配置所有函数的声明（这样写在前面的函数也可以调用后面才声明的函数）
- [x] install_globalVars函数将预先配置所有全局变量的声明（这样全局变量在文件哪里定义就无所谓了）
  
- [x] 赋值语句右值和左值类型相符,Assign class
- [x] 函数调用时的实参一定要和声明的形参一致，且数目相同
- [x] `if`、`while`条件部分的类型一定是`Bool`
- [x] `for`的条件部分可以置空语句
- [x] 运算符的操作数必须满足类型约束，即四则运算和比较运算可以在`Int`和`Float`之间混合发生，布尔运算只能在`Bool`之间发生，位运算只能在`Int`之间发生
- [x] `continue`和`break`必须位于某一个循环体内
- [x] 不能调用未被声明的变量
- [x] 每个函数最多有6个参数


### 石墨文档中提问的考察点+语义分析作业要求文档+others
- [x] 3.10 函数内新声明同名的变量，可以重复命名，新加一层变量作用域
- [x] 变量作用域
- [ ] 需要看一下，没有报错的例子中，type是否正确,写一些正确的程序，对比语法树，看一看语法树
- [x] one uncertain case
  

### check list
- [x] install_calls, 函数名重定义
- [x] check_main
  - [x] 存在main函数
  - [x] main函数声明返回值为void，报错方式的表述问题
  - [x] main函数声明不应该传参数
  - [x] 不能有一个函数声明为printf
- [x] install_globalVars
  - [x] 全局变量重复定义
  - [x] 全局变量不能是Void，其他四种都可以
- [x] check_calls
- [x] VariableDecl_class::check()
  - [x] 变量不能声明为Void
  - [x] 变量重复定义
- [x] CallDecl_class::check()
  - [x] param type should not by Void-------?
  - [x] param name duplicated
  - [x] param count <=6 
  - [x] 每一个函数声明在最外层逻辑结束前（即函数体的最外层右大括号闭合前）一定要有一个`return`语句，这里做了修改，不是只出现return语句就行，而且不一定出现在最后一个,这里需要注意
  - [x] return 之后是否还要继续检查，需要的
- [x] StmtBlock_class::check(Symbol type)
  - [x] vars
  - [x] stmts
- [x] IfStmt_class::check(Symbol type)
  - [x] 条件必须为bool
- [x] WhileStmt_class::check(Symbol type)
  - [x] condition Bool
- [x] ForStmt_class::check(Symbol type)
- [x] ReturnStmt_class::check(Symbol type)
  - [x] 返回类型和声明的一样
- [x] ContinueStmt_class::check(Symbol type), in loop
- [x] BreakStmt_class::check(Symbol type), in loop
- [x] Call_class::checkType()
  - [x] printf,至少有一个参数，第一个参数类型必须为String
  - [x] 实参和形参数目不一样
  - [x] 实参和形参类型不一样，每个都要报错，和参考程序不一样
  - [x] 调用了没有声明的函数
- [x] Actual_class::checkType(),就是expr
- [x] Assign_class::checkType()
  - [x] 左右变量类型不一样
  - [x] 把int赋值给float,不会特意考察这一点的-----
  - [x] 左值没有被定义，考虑作用域
- [x] Add_class::checkType(),Minus_class::checkType(),Multi_class::checkType(),Divide_class::checkType()
- [x] Mod_class::checkType(),Int%Int
- [x] Neg_class::checkType()
- [x] Lt_class::checkType()<, Le_class::checkType()<=,Ge_class::checkType()>=, Gt_class::checkType()>
  - [x] 大小比较可以在bool中发生吗?不能
  - [x] 如果semant，返回什么type?参考程序是返回bool
  - [x] 和参考程序字符串一样()
- [x] Equ_class::checkType()
- [x] Neq_class::checkType()
- [x] And_class::checkType()
- [x] Or_class::checkType()
- [x] Xor_class::checkType()
- [x] Not_class::checkType()
- [x] Bitand_class::checkType()
- [x] Bitor_class::checkType()
- [x] Bitnot_class::checkType()
- [x] Object_class::checkType()
- [x] No_expr_class::checkType()


### Uncertain cases
```
func max(x Float,y Void) Float{
	if x>y{
        	return x;
	}
	return y;
}

func main() Void{
	var a Float;
	var b Float;
	var max_num Float;

	a=22.2;
	b=33.3;
	max_num=max(a,b);
	return c;
}

```