## 目标代码生成
### 备忘手册
* emit_mov: 
  movq	%rsp, %rbp
* emit_rmmov: 
  s << MOV << source_reg << COMMA << offset << "(" << base_reg << ")" << endl;
* emit_mrmov: 
  movq	-64(%rbp), %rdi
* emit_irmov: 
  mov	$0, %eax
* emit_irmovl: 
  movl	$0, %eax <printf>
* emit_immov: 
  s << MOV << "$" << immidiate << COMMA << "(" << offset << ")" << base_reg << endl;

### 指令
```bash
make cgen
make clean
./cgen test/helloworld.seal -o tmp-answer/helloworld.s  
./cgen test/helloworld.seal -o tmp-answer/helloworld__.s
gcc tmp-answer/helloworld.s -no-pie -o tmp-answer/main  
gcc tmp-answer/helloworld__.s -no-pie -o tmp-answer/main
./tmp-answer/main
```